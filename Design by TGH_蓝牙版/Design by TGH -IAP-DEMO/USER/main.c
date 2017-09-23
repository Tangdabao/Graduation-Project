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
      *************重大更新:新PCB板子*****************	         
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
      *************重大更新*****************					 
2017-05-08:添加IAP，空中升级;基本完善；修复BUG：只能执行一部分解决办法，，因为中断打断升级
2017-05-09:理解IAP原理
2017-05-10:优化代码，代码数量减少，添加蓝牙升级机制
2017-05-11:因RAM不足，寻找其他办法；试想使用AT24C256增加EEPROM；
2017-05-12:添加EEPROM代码，但是由于技术问题导致未能添加；寻找其他办法
2017-05-13:看到YModem协议，拟用此方法；
2017-05-14:添加YModem代码新BUG：不能执行之前的程序；
      *************重大更新*****************
2017-05-15:修改YModem代码BUG，添加是否更新程序选项；由用户自行决定；
*******************************************************************************/
#include "sys.h"
#include "delay.h"
#include "usart.h"	
#include "oled.h"
#include "key.h"
#include "common.h"

///////日历显示参数///////////	
u8 t;	

//////////IAP参数//////////////
extern pFunction Jump_To_Application;
extern uint32_t JumpAddress;
void IAP_Handler(void);

 int main(void)
 {
	 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();	    	 //延时函数初始化	
	IAP_Init();
	KEY_Init();
	OLED_Init();			   
	OLED_Clear(); 
	 	 
/////////IAP服务函数////////
	IAP_Handler();
   while(1);
	 
}
 
 
//////用户请求更新///////////////////
 void IAP_Handler(void)
 {
	 u8 key=0;
	 OLED_ShowString(0,0,"update program",12);	 
   OLED_ShowString(0,12,"press S4",12);	
	 OLED_ShowString(0,24,"If not,ignore",12);
	 OLED_ShowString(0,36,"not power off",12);
	 OLED_Refresh_Gram();	
	 delay_ms(1000);
   delay_ms(1000);
	 delay_ms(1000);
	 key=KEY_Scan(1);
//Flash 解锁
    FLASH_Unlock();
 //按键是否按下
    while(key)
    {
        SerialPutString("\r\n======================================================================");
        SerialPutString("\r\n=                                                                    =");
        SerialPutString("\r\n=     In-Application Programming Application=V2.0 BYTANGgh           = ");
        SerialPutString("\r\n=                                                                    =");
        SerialPutString("\r\n======================================================================");
        SerialPutString("\r\n\r\n");
        Main_Menu ();
    }
        if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x20000000)
        {
            SerialPutString("Execute user Program\r\n\n");
            JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);
            Jump_To_Application = (pFunction) JumpAddress;
            __set_MSP(*(__IO uint32_t*) ApplicationAddress);
            Jump_To_Application();
        }
        else
        {
            SerialPutString("no user Program\r\n\n");
        }
 
 
 }
 
