/************************************************
 * main_mas.c
 * Tetsuya Kaku
 * =============================================
 * SPI通信(マスター)確認メインファイル
 * =============================================
 * ・ver2.00 二版 2015/12/23 Tetsuya Kaku
 * マイコン間のSPI通信をする
 * =============================================
 * PIC16F877A
 * MPLAB X IDE(v3.10/Win)
 * XC8 (v1.35/Win)
************************************************/

#include <xc.h>
#include <stdio.h>
#include "pic_clock.h"
#include "spi_master.h"

//コンフィグ設定
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

/****************************************************************************
* main()
****************************************************************************/
void main(){
    
    spi_init();                     //SPI通信マスター設定
    TRISA = 0b00000000;             //RA0～7を出力にする
    TRISA1 = 1;
    SS1 = 1;
    
    //初期動作確認LED
    RA0 = 1;
    __delay_ms(1000);
    RA0 = 0;
    __delay_ms(1000);
    
    //SPI送信の無限ループ
    while(1){
        
        //データ用意
        char w[] ="Hello world!";
        char t[] ="This is fabsat";

        //データ通信
        SS1 = 1;                //スレーブ側に受信待機させる
        data_transmint(w);      //w[]を送信
        __delay_ms(40);         
        SS1 = 1;                //スレーブ側に受信待機させる
        data_transmint(t);      //t[]を送信
        __delay_ms(40);
    }
}
