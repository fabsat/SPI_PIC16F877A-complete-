/************************************************
 * spi_slave.h
 * Tetsuya Kaku
 * =============================================
 * SPI通信(スレーブ側)ヘッダファイル
 * =============================================
 * ・ver2.00 二版 2015/12/23 Tetsuya Kaku
 * SPI通信をする
 * =============================================
 * PIC16F877A
 * MPLAB X IDE(v3.10/Win)
 * XC8 (v1.35/Win)
************************************************/

#ifndef SPI_SLAVE_H
#define	SPI_SLAVE_H

//SPIピン定義
#define SCK RC3
#define SDI RC4
#define SDO RC5
#define SS1 RA5         //マスターの検知ピン1
#define SS  RA1         //マスターへの確認信号ピン1

//SPI関連関数プロトタイプ宣言
void spi_init(void);               //SPI初期設定関数
static void interrupt isr(void);    //割り込みリセット関数
static void spi_reset(void);       //SPIリセット関数
static void timer_on(void);        //割り込み可能関数
static void timer_off(void);       //割り込み禁止関数
static char spi_receive(void);     //SPIデータ受信関数
void data_receuve(char *data);     //受信データ格納関数

#endif	/* SPI_SLAVE_H */
