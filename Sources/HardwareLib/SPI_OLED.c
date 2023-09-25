/*
 * SPI_OLED.c
 *
 *  Created on: 2023年8月7日
 *      Author: dengtongbei
 */
#include "SPI_OLED.h"
#include "OLED_FONT.h"
#include "latency.h"

uint8_t SPI_OLED_GRAM[144][8];


/* D0  使用SPI的时钟线 */
void SPI_OLED_SCLK_Clr(void) { PTD->PCOR |= (1UL << 10);  /*PD10*/}
void SPI_OLED_SCLK_Set(void) { PTD->PSOR |= (1UL << 10); }
/* D1  使用SPI的数据线 */
void SPI_OLED_SDIN_Clr(void) { PTD->PCOR |= (1UL << 11);  /*PD11*/ }
void SPI_OLED_SDIN_Set(void) { PTD->PSOR |= (1UL << 11); }
/* RES  复位（OLED上电后需要进行一次复位） */
void SPI_OLED_RST_Clr(void) { PTD->PCOR |= (1UL << 12);  /*PD12*/}
void SPI_OLED_RST_Set(void) { PTD->PSOR |= (1UL << 12); }
/* DC  数据/命令选择脚*/
void SPI_OLED_DC_Clr(void) { PTD->PCOR |= (1UL << 5);  /*PD5*/}
void SPI_OLED_DC_Set(void) { PTD->PSOR |= (1UL << 5); }
/* CS  片选（低电平有效，如果只有一块从设备，可以接地） */
void SPI_OLED_CS_Clr(void) { PTD->PCOR |= (1UL << 3);  /*PD3*/}
void SPI_OLED_CS_Set(void) { PTD->PSOR |= (1UL << 3); }

//SPI_IO的初始化
void SPI_IO_Init(void){
	PCC-> PCCn[PCC_PORTD_INDEX] |= PCC_PCCn_CGC_MASK; // 开启时钟模块
    PORTD->PCR[3] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;   // 配置引脚为GPIO，并设置驱动能力 1->强驱动, 0->普通驱动
    PORTD->PCR[5] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;   // 配置引脚为GPIO，并设置驱动能力
    PORTD->PCR[12] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;   // 配置引脚为GPIO，并设置驱动能力
    PORTD->PCR[11] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;   // 配置引脚为GPIO，并设置驱动能力
    PORTD->PCR[10] = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK;   // 配置引脚为GPIO，并设置驱动能力
    PTD->PDDR |= ((1 << 3)|(1 << 5)|(1 << 12)|(1 << 11)|(1 << 10));   //D3 D5 D12 D11 D10设置为输出模式
    PTD->PSOR |= ((1 << 3)|(1 << 5)|(1 << 12)|(1 << 11)|(1 << 10)); // 输出高电平
    
	PCC-> PCCn[PCC_PORTA_INDEX] |= PCC_PCCn_CGC_MASK; // 开启时钟模块
	PORTA->PCR[11] = PORT_PCR_MUX(1)| PORT_PCR_DSE(1); /* Port A11: MUX = GPIO 1 强驱动 */
	PORTA->PCR[17] = PORT_PCR_MUX(1)| PORT_PCR_DSE(1); /* Port A17: MUX = GPIO 1 强驱动 */
	PTA->PDDR |= ((1<<11)|(1<<17));
	PTA->PSOR |= (1<<17); // 输出高
	PTA->PCOR |= (1<<11); // 输出低

}
//OLED的上电初始化
void SPI_OLED_PowerOn_Init(void){
    SPI_OLED_RST_Set();
    later_ms(100);
    SPI_OLED_RST_Clr();
    later_ms(200);
    SPI_OLED_RST_Set(); 
                    
	SPI_OLED_Write_Byte(0xAE,OLED_CMD);//--turn off oled panel
	SPI_OLED_Write_Byte(0x00,OLED_CMD);//---set low column address
	SPI_OLED_Write_Byte(0x10,OLED_CMD);//---set high column address
	SPI_OLED_Write_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	SPI_OLED_Write_Byte(0x81,OLED_CMD);//--set contrast control register
	SPI_OLED_Write_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	SPI_OLED_Write_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	SPI_OLED_Write_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	SPI_OLED_Write_Byte(0xA6,OLED_CMD);//--set normal display
	SPI_OLED_Write_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	SPI_OLED_Write_Byte(0x3f,OLED_CMD);//--1/64 duty
	SPI_OLED_Write_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	SPI_OLED_Write_Byte(0x00,OLED_CMD);//-not offset
	SPI_OLED_Write_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	SPI_OLED_Write_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	SPI_OLED_Write_Byte(0xD9,OLED_CMD);//--set pre-charge period
	SPI_OLED_Write_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	SPI_OLED_Write_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	SPI_OLED_Write_Byte(0x12,OLED_CMD);
	SPI_OLED_Write_Byte(0xDB,OLED_CMD);//--set vcomh
	SPI_OLED_Write_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	SPI_OLED_Write_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	SPI_OLED_Write_Byte(0x02,OLED_CMD);//
	SPI_OLED_Write_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	SPI_OLED_Write_Byte(0x14,OLED_CMD);//--set(0x10) disable
	SPI_OLED_Write_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	SPI_OLED_Write_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	SPI_OLED_Write_Byte(0xAF,OLED_CMD);//--turn on oled panel
	
	SPI_OLED_Write_Byte(0xAF,OLED_CMD); /*display ON*/ 
	SPI_OLED_Clear();
	SPI_OLED_Set_Pos(0,0); 
}

//OLED的初始化
void SPI_OLED_Init(void){

	SPI_IO_Init();
	SPI_OLED_PowerOn_Init();
	
}



void SPI_OLED_Write_Byte(uint8_t data, uint8_t mode) {
    uint8_t i;
    if (mode)
        SPI_OLED_DC_Set();    // 高电平代表传输数据（Data）
    else
        SPI_OLED_DC_Clr();    // 低电平代表传输命令（Command）
    SPI_OLED_CS_Clr();        // CS线拉低
    for (i = 0; i < 8; i++) {
        SPI_OLED_SCLK_Clr();  // CLK线拉低
        if (data & 0x80)
            SPI_OLED_SDIN_Set();
        else
            SPI_OLED_SDIN_Clr();
        SPI_OLED_SCLK_Set(); // CLK线拉高 OLED在上升沿采集数据
        data <<= 1;
    }
    SPI_OLED_CS_Set();
    SPI_OLED_DC_Set();
}

void SPI_OLED_Set_Pos(uint8_t x, uint8_t y) 
{ 
	SPI_OLED_Write_Byte(0xb0+y,OLED_CMD);
	SPI_OLED_Write_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	SPI_OLED_Write_Byte((x&0x0f)|0x01,OLED_CMD); 
}   
//开启OLED显示    
void SPI_OLED_Display_On(void) {
	SPI_OLED_Write_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	SPI_OLED_Write_Byte(0X14,OLED_CMD);  //DCDC ON
	SPI_OLED_Write_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void SPI_OLED_Display_Off(void)
{
	SPI_OLED_Write_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	SPI_OLED_Write_Byte(0X10,OLED_CMD);  //DCDC OFF
	SPI_OLED_Write_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 

// 更新显存到OLED
void SPI_OLED_Refresh(void) {
	uint8_t i,n;
	for(i=0;i<8;i++) {
        SPI_OLED_Write_Byte(0xb0+i,OLED_CMD);    //设置页地址（0~7）
		SPI_OLED_Write_Byte(0x00,OLED_CMD);      //设置显示位置—列低地址
		SPI_OLED_Write_Byte(0x10,OLED_CMD);      //设置显示位置—列高地址 
        for(n=0;n<128;n++) {
            SPI_OLED_Write_Byte(SPI_OLED_GRAM[n][i], OLED_DATA);
		}
    }
}
//清屏函数
void SPI_OLED_Clear(void) {
	uint8_t i,n;
	for(i=0;i<8;i++) {
	   for(n=0;n<128;n++) {
			SPI_OLED_GRAM[n][i]=0;//清除所有数据
		}
    }
	SPI_OLED_Refresh();//更新显示
}

// 画点函数  x:0~127   y:0~63   t:1 填充 0,清空
void SPI_OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t) {
    uint8_t i, m, n;
    i = y / 8;
    m = y % 8;
    n = 1 << m;
    if(t) { SPI_OLED_GRAM[x][i] |= n;}
	else {
        SPI_OLED_GRAM[x][i] = ~SPI_OLED_GRAM[x][i];
        SPI_OLED_GRAM[x][i] |= n;
        SPI_OLED_GRAM[x][i] = ~SPI_OLED_GRAM[x][i];
    }
}

//在指定位置显示一个字符,包括部分字符 //x:0~127 //y:0~63
//size1:选择字体 6x8/6x12/8x16/12x24
//mode:0,反色显示;1,正常显示
void SPI_OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1,uint8_t mode)
{
	uint8_t i,m,temp,size2,chr1;
	uint8_t x0=x,y0=y;
	if(size1==8)size2=6;
	else size2=(size1/8+((size1%8)?1:0))*(size1/2);  //得到字体一个字符对应点阵集所占的字节数
	chr1=chr-' ';  //计算偏移后的值
	for(i=0;i<size2;i++) {
		if(size1==8)
		{temp=ASICII_0806[chr1][i];} //调用0806字体
		else if(size1==12)
        {temp=ASICII_1206[chr1][i];} //调用1206字体
		else if(size1==16)
        {temp=ASICII_1608[chr1][i];} //调用1608字体
		else if(size1==24)
        {temp=ASICII_2412[chr1][i];} //调用2412字体
		else return;
		for(m=0;m<8;m++) {
			if(temp&0x01) SPI_OLED_DrawPoint(x, y, mode);
			else SPI_OLED_DrawPoint(x, y, !mode);
			temp>>=1;
			y++;
		}
		x++;
		if((size1!=8)&&((x-x0)==size1/2))
		{x=x0;y0=y0+8;}
		y=y0;
    }
}

//显示字符串
//x,y:起点坐标  //size1:字体大小  //*chr:字符串起始地址
//mode:0,反色显示;1,正常显示
void SPI_OLED_ShowString(uint8_t x,uint8_t y,char *chr,uint8_t size1,uint8_t mode)
{
	while((*chr>=' ')&&(*chr<='~'))//判断是不是非法字符!
	{
		SPI_OLED_ShowChar(x,y,*chr,size1,mode);
		if(size1==8)x+=6;
		else x+=size1/2;
		chr++;
    }
    SPI_OLED_Refresh();
}

//m^n
uint32_t SPI_OLED_Pow(uint8_t m,uint8_t n){
	uint32_t result=1;
	while(n--){
	  result*=m;
	}
	return result;
}


//显示数字
//x,y :起点坐标 //num :要显示的数字 //len :数字的位数
//size:字体大小 8 / 12 / 16 / 24
//mode:0,反色显示;1,正常显示
void SPI_OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1,uint8_t mode)
{
    uint8_t t, temp, m = 0;
    if (size1 == 8)
        m = 2;
    for (t = 0; t < len; t++) {
        temp = (num / SPI_OLED_Pow(10, len - t - 1)) % 10;
        if (temp == 0) {
            SPI_OLED_ShowChar(x + (size1 / 2 + m) * t, y, '0', size1, mode);
        }else {
            SPI_OLED_ShowChar(x + (size1 / 2 + m) * t, y, temp + '0', size1, mode);
        }
    }
    SPI_OLED_Refresh();
}
// 显示图片 //x,y：起点坐标 //sizex,sizey,图片长宽
//BMP[]：要写入的图片数组 //mode:0,反色显示;1,正常显示
void SPI_OLED_ShowPicture(uint8_t x,uint8_t y,uint8_t sizex,uint8_t sizey,uint8_t BMP[],uint8_t mode)
{
    uint16_t j = 0;
    uint8_t i, n, temp, m;
    uint8_t x0 = x, y0 = y;
    sizey = sizey / 8 + ((sizey % 8) ? 1 : 0);
    for (n = 0; n < sizey; n++) {
        for (i = 0; i < sizex; i++) {
            temp = BMP[j];
            j++;
            for (m = 0; m < 8; m++) {
                if (temp & 0x01)
                    SPI_OLED_DrawPoint(x, y, mode);
                else
                    SPI_OLED_DrawPoint(x, y, !mode);
                temp >>= 1;
                y++;
            }
            x++;
            if ((x - x0) == sizex) {
                x = x0;
                y0 = y0 + 8;
            }
            y = y0;
        }
    }
}

