/************************************************
 * pic_clock.h
 * Tetsuya Kaku
 * =============================================
 * delay用のクロック定義ヘッダファイル
 * =============================================
 * ・ver1.00 初版 2015/12/09 Tetsuya Kaku
 * 20MHzで定義
 * =============================================
 * PIC16F877A
 * MPLAB X IDE(v3.10/Win)
 * XC8 (v1.35/Win)
************************************************/

#ifndef PIC_CLOCK_H
#define	PIC_CLOCK_H
#include <xc.h>

/* Define Clock Frequency for Delay */
#ifndef _XTAL_FREQ
#define _XTAL_FREQ 20000000
#endif

#endif	/* PIC_CLOCK_H */
