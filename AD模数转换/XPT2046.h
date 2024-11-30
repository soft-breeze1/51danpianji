#ifndef __XPT2046_H__
#define __XPT2046_H__

// 【通道宏定义】
// 控制字命令 【S , A2 , A1 , A0 , MODE , SER/DFR(上有横线) , PD1 , PD0 】
// 主要是改变【A2 , A1 , A0】
// MODE为 1，表示为 8 位精度，MODE为 0 ，表示为 12 位精度

// 8 位精度
#define XPT2046_XP_8		0x9C    // 【1 0 0 1 1 1 0 0】
#define XPT2046_YP_8		0xDC    // 【1 1 0 1 1 1 0 0】
#define XPT2046_VBAT_8		0xAC    // 【1 0 1 0 1 1 0 0】
#define XPT2046_AUX_8		0xEC    // 【1 1 1 0 1 1 0 0】

// 12 位精度
#define XPT2046_XP_12		0x94    // 【1 0 0 1 0 1 0 0】
#define XPT2046_YP_12		0xD4    // 【1 1 0 1 0 1 0 0】
#define XPT2046_VBAT_12		0xA4    // 【1 0 1 0 0 1 0 0】
#define XPT2046_AUX_12		0xE4    // 【1 1 1 0 0 1 0 0】

unsigned int XPT2046_ReadAD(unsigned char Command);

#endif