#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    

#define OLED_MODE 0
		    						  
//-----------------OLED端口定义----------------  					   
 		     
#define OLED_CS_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_1)//CS
#define OLED_CS_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_1)


#define OLED_RST_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_12)//RES
#define OLED_RST_Set() GPIO_SetBits(GPIOB,GPIO_Pin_12)

#define OLED_RS_Clr()  GPIO_ResetBits(GPIOA,GPIO_Pin_12)//DC
#define OLED_RS_Set() GPIO_SetBits(GPIOA,GPIO_Pin_12)

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_15)//D0
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_15)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_13)//D1
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_13)




/////////////////////////////////////////////////////////////////////

#define OLED_WR_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_15)
#define OLED_WR_Set() GPIO_SetBits(GPIOB,GPIO_Pin_15)

#define OLED_RD_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define OLED_RD_Set() GPIO_SetBits(GPIOB,GPIO_Pin_13)


//PC0~7,作为数据线
#define DATAOUT(x) GPIO_Write(GPIOC,x);//输出  
//使用4线串行接口时使用 



 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//OLED控制用函数
void OLED_WR_Byte(u8 dat,u8 cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Refresh_Gram(void);		   
							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void OLCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey);
#endif  
	 



