/************************************************
 * spi_master.h
 * Tetsuya Kaku
 * =============================================
 * SPI通信(マスター側)ヘッダファイル
 * =============================================
 * ・ver2.00 二版 2015/12/23 Tetsuya Kaku
 * SPI通信をする
 * =============================================
 * PIC16F877A
 * MPLAB X IDE(v3.10/Win)
 * XC8 (v1.35/Win)
************************************************/


#ifndef SPI_MASTER_H
#define	SPI_MASTER_H

//SPI通信ピン定義
#define SCK RC3
#define SDI RC4
#define SDO RC5
#define SS  RA1           //スレーブの検知ピン
#define SS1 RA2           //スレーブへの確認信号ピン1

//SPI通信関数プロトタイプ宣言
void spi_init(void);                    //SPI初期設定関数
static void interrupt isr(void);         //割り込みリセット関数
static void spi_reset(void);            //SPIリセット関数
static void timer_on(void);             //割り込み可能関数
static void timer_off(void);            //割り込み禁止関数
static void spi_transmit(char data);    //SPIデータ送信関数
void data_transmint(char *data);        //データ送信関数

#endif	/* SPI_MASTER_H */

