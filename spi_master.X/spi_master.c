/************************************************
 * spi_master.c
 * Tetsuya Kaku
 * =============================================
 * SPI通信(マスター側)ソースファイル
 * =============================================
 * ・ver2.00 二版 2015/12/23 Tetsuya Kaku
 * SPI通信(マスター側)をする
 * =============================================
 * PIC16F877A
 * MPLAB X IDE(v3.10/Win)
 * XC8 (v1.35/Win)
************************************************/

#include <xc.h>
#include <stdio.h>
#include <pic16f877a.h>
#include "spi_master.h"
#include "pic_clock.h"


/*=====================================================
 * @breif
 *     SPIマスター設定関数
 * @param
 *     なし
 * @return
 *     なし
 * @note
 *     SPI通信の設定を要確認
 *===================================================*/
void spi_init(void) {
    
    ADCON1 = 7;             // Configure pins as digital
    TRISB  = 0b00000000;    // push button on RB7
    nRBPU  = 0;             // Set weak pullups in port B
    
    // Timer 1 prescaler 1:8
    T1CONbits.T1CKPS1 = 1;
    T1CONbits.T1CKPS0 = 1;
    
    INTCON = 0;         // Disable all interrupts
    TRISC3 = 0;         // SCK=RC3 is the serial clock
    TRISC4 = 1;         // SDI=RC4 is serial data input
    TRISC5 = 0;         // SDO=RC5 is serial data output
    SSPEN = 0;          // Allow Programming of serial port
    SMP = 0;            // Input data sampled at middle data output time
    CKP = 1;            // Idle state for clock is a HIGH level
    CKE = 1;            // Transmit occurs on idle to active clock state
    
    // SPI Master mode, clock = FOSC/4
    SSPM3 = 0;          
    SSPM2 = 0;          
    SSPM1 = 0;          
    SSPM0 = 0;          
    
    SSPIF = 0;          // The last bit is not latched
    SSPEN = 1;          // End programming and Start serial port
}


/*-----------------------------------------------------
 * @breif
 *     SPIリセット関数
 * @param
 *     なし
 * @return
 *     なし
 * @note
 *     spi_master.c内で使用する関数なのでstatic関数
 *---------------------------------------------------*/
static void spi_reset(void)
{
unsigned char dummy;
        SSPEN             = 0;  //  Reset SPI module
        SSPEN             = 1;  //  Reset SPI module
        dummy             = SSPBUF;
        SSPIF             = 0;
        SSPEN             = 0;  //  Reset SPI module
        SSPEN             = 1;  //  Reset SPI module
}


/*-----------------------------------------------------
 * @breif
 *     割り込みリセット関数
 * @param
 *     なし
 * @return
 *     なし
 * @note
 *     spi_master.c内で使用する関数なのでstatic関数
 *---------------------------------------------------*/
static void interrupt isr(void)
{
// Check all interupts - and reset them all !!
    if (PIR1bits.TMR1IF  == 1)  // Check for Timer overflow
    {
        PIR1bits.TMR1IF   = 0;  // Reset interupt flag
        spi_reset();
        timer_off();
    }
}


/*-----------------------------------------------------
 * @breif
 *     割り込み可能関数
 * @param
 *     なし
 * @return
 *     なし
 * @note
 *     spi_master.c内で使用する関数なのでstatic関数
 *---------------------------------------------------*/
static void timer_on(void)
{
        PIE1bits.TMR1IE   = 1;  // Timer 1 overflow interupt enabled
        T1CONbits.TMR1ON  = 1;  // Timer 1 on
        INTCONbits.PEIE   = 1;  // Enable Peripheral interupts
        INTCONbits.GIE    = 1;  // Global Interupt enable
}


/*-----------------------------------------------------
 * @breif
 *     割り込み禁止関数
 * @param
 *     なし
 * @return
 *     なし
 * @note
 *     spi_master.c内で使用する関数なのでstatic関数
 *---------------------------------------------------*/
static void timer_off(void)
{
        PIR1bits.TMR1IF   = 0;  // Reset interupt flag
        PIE1bits.TMR1IE   = 0;  // Timer 1 overflow interupt enabled
        T1CONbits.TMR1ON  = 0;  // Timer 1 on
        INTCONbits.PEIE   = 0;  // Enable Peripheral interupts
        INTCONbits.GIE    = 0;  // Global Interupt enable
}


/*-----------------------------------------------------
 * @breif
 *     SPIデータ送信関数
 * @param
 *     ss:スレーブ側からの確認ピン(High or Low)
 *     data:実際に送るchar型データ
 * @return
 *     なし
 * @note
 *     SPI通信を実際に行うstatic関数
 *-----------------------------------------------------*/
static void spi_transmit(char data){
    char dummy;
    SSPBUF = data;
    while(BF==0){
        timer_on();
    }
    timer_off();
    dummy = SSPBUF;             //reads SSPBUF
    //while(RA1 == 1){;}
    //ss = 0;
}


/*=====================================================
 * @breif
 *     データ送信関数
 * @param
 *     *data:実際に送るデータ(配列)
 * @return
 *     なし
 * @note
 *     データを終わりまで送信する関数
 *===================================================*/
void data_transmint(char *data){
    int i=0;
    SS1 = 0;                  //スレーブにマスターの送信準備完了信号送信
    while(SS==1){;}           //スレーブの受信準備完了まで待機
    while(data[i] != '\0'){    
        spi_transmit(data[i]);   // SS1を判別しw[]を送信
        i++;
    }
    spi_transmit('\0');
}
