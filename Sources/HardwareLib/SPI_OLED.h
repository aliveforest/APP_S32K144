/*
 * SPI_OLED.h
 *
 *  Created on: 2023年8月7日
 *      Author: dengtongbei
 */

#ifndef SPI_OLED_H_
#define SPI_OLED_H_
#include "S32K144.h"

#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	 

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

// void SPI_OLED_SCLK_Clr(void); // D0
// void SPI_OLED_SCLK_Set(void);
// void SPI_OLED_SDIN_Clr(void); // D1
// void SPI_OLED_SDIN_Set(void);
// void SPI_OLED_RST_Clr(void);  // RES
// void SPI_OLED_RST_Set(void);
// void SPI_OLED_DC_Clr(void);   // DC
// void SPI_OLED_DC_Set(void);
// void SPI_OLED_CS_Clr(void);   // CS
// void SPI_OLED_CS_Set(void);

void SPI_OLED_PowerOn_Init(void);
void SPI_OLED_Init(void);
void SPI_OLED_Clear(void);                    // 清屏函数
void SPI_OLED_Refresh(void);
void SPI_OLED_Set_Pos(uint8_t x, uint8_t y); 
void SPI_OLED_Display_On(void);
void SPI_OLED_Display_Off(void);

void SPI_OLED_Write_Byte(uint8_t data, uint8_t mode);
void SPI_OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void SPI_OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1,uint8_t mode);
void SPI_OLED_ShowString(uint8_t x,uint8_t y,char *chr,uint8_t size1,uint8_t mode);
void SPI_OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1,uint8_t mode);
void SPI_OLED_ShowPicture(uint8_t x,uint8_t y,uint8_t sizex,uint8_t sizey,uint8_t BMP[],uint8_t mode);
#endif /* SPI_OLED_H_ */
