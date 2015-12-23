/************************************************
 * main_sla.c
 * Tetsuya Kaku
 * =============================================
 * SPI通信(スレーブ)確認メインファイル
 * =============================================
 * ・ver2.00 二版 2015/12/23 Tetsuya Kaku
 * マイコン間のSPI通信をシリアル通信でPCとつなげて確認する
 * =============================================
 * PIC16F877A
 * MPLAB X IDE(v3.10/Win)
 * XC8 (v1.35/Win)
************************************************/

#include <xc.h>
#include <stdio.h>
#include <pic16f877a.h>
#include "uart_serial.h"
#include "spi_slave.h"

//コンフィグ設定
#pragma config FOSC = HS, WDTE = OFF, PWRTE = ON,  BOREN = ON,  LVP = OFF
#pragma config CPD = OFF, WRT = OFF, CP = OFF

/****************************************************************************
* main()
****************************************************************************/
void main(void)
{
    TRISA = 0b00000000;     // PORTAの入出力初期化設定
    TRISA5 = 1;             //RA5を入力モード
    PORTA = 0x00;           // PORTAを初期化
    SS = 1;                 //マスターの不可送信信号送信
    
    //SPI,UART初期設定
	uart_init();             //「調歩同期式シリアル通信」の設定を行う関数
    spi_init();              // SPI初期設定関数
    char w[20]; 
    printf("Start_SPI:\r\n");		//PCのターミナル画面に「Start_SPI」を表示し、キャリッジリターン\rとラインフィード\n
   
    //SPI受信の無限ループ
    while(1){
        
        //SPI通信
        printf("ready...\r\n");            //ターミナルに「ready...」を表示
        printf("SSPBUF = "); 
        data_receuve(w);                    //データ受信
        SS = 1;                             //マスターの不可送信信号送信
        
        //データ結果表示
        int i = 0;
        while(w[i] != '\0'){
            putch(w[i]);
            i++;
        }
        putch('\r');
        putch('\n');
        
    }
        
}


