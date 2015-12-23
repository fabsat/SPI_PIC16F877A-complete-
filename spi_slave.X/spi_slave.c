/************************************************
 * spi_slave.c
 * Tetsuya Kaku
 * =============================================
 * SPI通信(スレーブ側)ソースファイル
 * =============================================
 * ・ver2.00 二版 2015/12/23 Tetsuya Kaku
 * SPI通信(スレーブ側)をする
 * =============================================
 * PIC16F877A
 * MPLAB X IDE(v3.10/Win)
 * XC8 (v1.35/Win)
************************************************/

#include <xc.h>
#include <stdio.h>
#include <pic16f877a.h>
#include "spi_slave.h"
#include "pic_clock.h"


/*=====================================================
 * @breif
 *     SPIスレーブ設定関数
 * @param
 *     なし
 * @return
 *     なし
 * @note
 *     SPI通信の設定を要確認
 *===================================================*/
void spi_init(void) {
    
    ADCON1 = 7;         // SSP module in SPI Slave mode and SS enable
   
    // Timer 1 prescaler 1:1
    T1CONbits.T1CKPS1 = 0;
    T1CONbits.T1CKPS0 = 0;
    
    INTCON = 0;         // Disable all interrupts
    TRISC3 = 1;         // SCK=RC3 is the serial clock
    TRISA5 = 1;         // SS=RA4 is slave select input
    TRISC2 = 1;         // SDI=RC4 is serial data input
    //PORTCbits.RC5 = 1;  // 
    TRISC5 = 0;         // SDO=RC5 is serial data output
    SSPEN = 0;          // Allow Programming of serial port
    SMP = 0;            // Input data sampled at middle data output time
    CKP = 1;            // Idle state for clock is a HIGH level
    CKE = 1;            // Transmit occurs on idle to active clock state
    
    // SPI Slave mode, clock = SCK pin. SS pin control disabled. SS can be used as I/O pin.
    SSPM3 = 0;
    SSPM2 = 1;
    SSPM1 = 0;
    SSPM0 = 1;
    
    SSPIF = 0;          // The last bit is not latched
    SSPEN = 1;          // Enables serial port and configures SCK, SDO, SDI, and SS as serial port pins
}



/*-----------------------------------------------------
 * @breif
 *     割り込みリセット関数
 * @param
 *     なし
 * @return
 *     なし
 * @note
 *     spi_slave.c内で使用する関数なのでstatic関数
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
 *     SPIリセット関数
 * @param
 *     なし
 * @return
 *     なし
 * @note
 *     spi_slave.c内で使用する関数なのでstatic関数
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
 *     割り込み可能関数
 * @param
 *     なし
 * @return
 *     なし
 * @note
 *     spi_slave.c内で使用する関数なのでstatic関数
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
 *     spi_slave.c内で使用する関数なのでstatic関数
 *---------------------------------------------------*/
static void timer_off(void)
{
        PIR1bits.TMR1IF   = 0;  // Reset interupt flag
        PIE1bits.TMR1IE   = 0;  // Timer 1 overflow interupt disabled
        T1CONbits.TMR1ON  = 0;  // Timer 1 off
        INTCONbits.PEIE   = 0;  // Disable Peripheral interupts
        INTCONbits.GIE    = 0;  // Global Interupt disable
}


/*---------------------------------------------------
 * @breif
 *     SPIデータ受信関数
 * @param
 *     なし
 * @return
 *     SSPBUF
 * @note
 *     SPI通信を実際に行う関数
 *---------------------------------------------------*/
static char spi_receive(void){
    
    if(SS1 == 0){
        while(BF==0){                   // Wait for SSPBUF Reception complete  
            timer_on();
        }
        timer_off();   
    return SSPBUF;                      //データを受け取り終えたSSPBUFのデータを戻り値で返す
    }
    return 0;                           //受信しなかったら何も返さない
}


/*=====================================================
 * @breif
 *     データ受信関数
 * @param
 *     *data:受信データを格納する配列
 * @return
 *     なし
 * @note
 *     受信データを配列に格納する関数
 *===================================================*/
void data_receuve(char *data){
    int i = 0;
    SS = 0;                    //マスターに受信準備完了信号送信
    while(SS1==1){;}           //マスターの送信直前まで待機
    
    //受信してきたデータを最後まで配列に格納する
    while((data[i] = spi_receive()) != '\0' && i < 20){
       i++;
    }
}
