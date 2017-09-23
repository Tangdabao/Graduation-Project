/*******************************************************************************
** 文件名: 	  main.c
** 版本：  	
** 工作环境:  KEIL5.14
** 作者:      唐国华
** 生成日期: 	2017-03-20
** 功能:      毕业设计
** 相关文件:	  使用了原子电子的底层文件，包括：#include "sys.h"、#include "delay.h"做出声明并表示感谢
** 更新日志:
2017-03-27:IQS263触摸部分初步配置完成；
2017-04-03:添加OLED显示，进一步优化IQS263
2017-04-20:添加RTC实时时钟，添加闹钟提醒
2017-04-21:调试LP5110，但是发现封装问题
2017-04-24:添加蜂鸣器程序

//////////////////新PCB板子////////////////////////////
2017-04-27:LP5110初始化、添加LP55231温度检测函数与测试函数、
           按键添加到三个、更新OLED底层程序
					 
2017-04-29:更改OLED代码，由之前的三行修改成5行；x,0-48,y,0-118;	
           添加LP55231代码,增加PWM输出模式与自编程模式输出模式
2017-04-30:添加LP55231注释，优化LP55231温度显示,添加6种显示模式；
           添加AT24C256代码,检测代码与读写测试代码
2017-05-01:添加DS18B20代码,温度有问题，未修改
           添加DH111代码
					 修复IQS263不能调光BUG，完成通过IQS263调节RGB功能；
					 基本功能完成，其他优化即可；
2017-05-02:DS18B20代码BUG修复，原因：最少需要470ms等待转换时间
           添加定时器中断代码，用于DHT11的检测；防止其读取时间影响IQS263；	
           重新调整OLED布局，修改万年历布局，简化代码数量；	
2017-05-03:修复RTC,添加待机唤醒；
           新BUG闹钟,待机每次唤醒都会执行一次头函数；
2017-05-04:修复RTC,修改调节日历机制三个按键完成调节年月日小时分钟等功能代码，更加完善简练；
           修复闹钟BUG，调节闹钟不在修改时间；
           新BUG：待机每次唤醒都会执行一次头函数依旧存在；待机代码需要完善
					 闹钟一旦响起，不停；
2017-05-05:什么也没修复
2017-05-06:什么也没修复
2017-05-07:修复待机唤醒模式，添加新思路：扫过点亮屏幕，此时可以调节亮度，调节完毕，自动
           进入待机模式；
					 
//////////////////IAP代码升级////////////////////////////
2017-05-08:添加IAP，空中升级;基本完善；修复BUG：只能执行一部分解决办法，，因为中断打断升级					 
2017-05-09:理解IAP原理
2017-05-10:大幅度优化代码，代码数量由30K减少到20K左右
*******************************************************************************/
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "myiic.h"
#include "IQS263.h"
#include "LP55231.h" 
#include "oled.h"
#include "rtc.h"
#include "key.h"
#include "beep.h"
#include "at24c256.h"
#include "dht11.h"
#include "timerdh11.h"
#include "standby.h"

/////IQS263参数////
extern uint8_t touch0,touch_val;

/////LP55231参数//////
extern uint8_t data_buffer_LP[10];

///////日历显示参数///////////	
u8 t;	


 int main(void)
 {
	 
	SCB->VTOR = FLASH_BASE|0x5000; 
	 
  // 关闭总中断,防止打断升级；
	__disable_irq();    
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();	    	 //延时函数初始化	
  TIM3_Int_Init(4999,7199);//计数到5000为500ms  	 
	LED_Init();
	KEY_Init();
	BEEP_Init();
	OLED_Init();			   
  RTC_Init();
  WKUP_Init();		 
	IQS263_Init();			
	LP55_I2c2Init();   
  IIC_AT24C256_Init();
	DHT11_Init();
	LPP55231_CLOSE();
	OLED_Clear(); 
	// 开启总中断
  __enable_irq();    

while(1)
	{
	
//////万年历调节部分////////	
  SET_shijian();
 	
////////IQS263读取////////	
	if(I2C_RDY_READ()==0)
	{
		handleEvents();
		OLED_ShowNum(18,48,touch_val,3,12);//显示ASCII字符的码值
		Breathing_LED_PWM_1(touch_val);	
		OLED_Refresh_Gram();	
	}
	
	
	

//////万年历显示部分////////	
	if(t!=calendar.sec)
		{
		   t=calendar.sec;		

			OLED_ShowNum(0,0,calendar.w_year,4,12);
			OLED_ShowChar(24,0,'/',12,1);
			OLED_ShowNum(30,0,calendar.w_month,2,12);
      OLED_ShowChar(42,0,'/',12,1);			
			OLED_ShowNum(48,0,calendar.w_date,2,12);
			
			
      OLED_ShowNum(0,12,calendar.hour,2,12);
			OLED_ShowChar(12,12,':',12,1);
			OLED_ShowNum(18,12,calendar.min,2,12);
      OLED_ShowChar(30,12,':',12,1);			
			OLED_ShowNum(36,12,calendar.sec,2,12);
			
			
			switch(calendar.week)
			{
				case 0:
					OLED_ShowString(66,0,"Sunday",12);
					break;
				case 1:
					OLED_ShowString(66,0,"Monday",12);
					break;
				case 2:
					OLED_ShowString(66,0,"Tuesday",12);
					break;
				case 3:
					OLED_ShowString(66,0,"Wednesday",12);
					break;
				case 4:
					OLED_ShowString(66,0,"Thursday",12);
					break;
				case 5:
					OLED_ShowString(66,0,"Friday",12);
					break;
				case 6:
					OLED_ShowString(66,0,"Saturday",12);
					break;  
			}
		}
		OLED_Refresh_Gram();	
	 	delay_ms(2);
		
	}
 }

 
 
 
