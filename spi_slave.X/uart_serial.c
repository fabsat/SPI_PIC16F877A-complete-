/************************************************
 * uart_serial.c
 * Tetsuya Kaku
 * =============================================
 * uart通信(シリアル)定義ソースファイル
 * =============================================
 * ・ver1.00 初版 2015/12/09 Tetsuya Kaku
 * uartでPCとシリアル通信をする
 * =============================================
 * PIC16F877A
 * MPLAB X IDE(v3.10/Win)
 * XC8 (v1.35/Win)
************************************************/

#include <xc.h>
#include "uart_serial.h"
#include "pic_clock.h"


/*=====================================================
 * @breif
 *     uart通信初期設定関数
 * @param
 *     なし
 * @return
 *     なし
 * @note
 *     型番によってRXピンを確認する必要がある
 *===================================================*/
void uart_init()
{
    TRISC7 = 1;         // RX=RC7 is serial data input
    SPBRG = SPBRG_DATA;
    TXSTA = (TX9_RX9_DATA | BRGH_DATA | 0x20);
    RCSTA = (TX9_RX9_DATA | 0x90);
}

void putch(unsigned char byte)
{
    while(!TXIF){
    }
    TXREG = byte;
}
 
unsigned char getch()
{
    while(!RCIF){
    }
 
    return RCREG;
}
 
unsigned char getche()
{
    unsigned char c;
 
    c = getch();
    putch(c);
 
    return c;
}
