#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
#include "oled.h"
#include "rtc.h"


///////日历参数///////////	
extern int Set_min,Set_hour,Set_M,Set_H;
static u8 s1num=0;


void KEY_Init(void)
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_5|GPIO_Pin_9|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
		

}

/*
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY0==0)return KEY0_PRES;
		else if(KEY1==0)return KEY1_PRES;
		 else if(KEY2==0)return KEY2_PRES;
	}else if(KEY0==1&&KEY1==1&&KEY2==1)key_up=1; 	    
 	return 0;// 无按键按下
}
*/


void SET_shijian(void)
{
	
	if(KEY2==0)
	{
	 delay_ms(5);
		if(KEY2==0)  //调整选择
		{ 
			s1num++;
			while(!KEY2);
		if(s1num==1)  //调整分钟
	    {
			OLED_ShowString(18,12,"__",12);
		  OLED_ShowString(0,24,"SET_MIN",12);
      delay_ms(2);
		  }
		if(s1num==2)  //调整小时
	   {
		 OLED_ShowString(0,12,"__",12);
		 OLED_ShowString(0,24,"SET_HOUR",12);
		 delay_ms(2);
	   }
		if(s1num==3)  //调整闹钟分钟
	   {
		OLED_ShowString(0,24,"NAO_MIN",12);
		delay_ms(2);
     }
		if(s1num==4)  //调整闹钟小时
	   {
    OLED_ShowString(0,24,"NAO_HOUR",12);
    delay_ms(2);
	   }
	  if(s1num==5)  //还原
	   {
		 s1num=0;
			 
		 ///OLED_ShowString(0,24,"SET FOR S4",12);
		  OLED_Fill(0,24,100,30,0); 
		 delay_ms(2);
			 
     }
	 }
 }
     if(s1num!=0)  //调整数值
     {   
         if(KEY0==0)  //加数
         {
           delay_ms(5);
         if(KEY0==0)
          {
             while(!KEY0);

            if(s1num==1)  //调整分钟
           {
             Set_min++;
              if(Set_min==60)
              Set_min=0;
            }
            if(s1num==2)  //调整小时
           {  
             Set_hour++;
            if(Set_hour==24)
             Set_hour=0;
           }
					 
					  if(s1num==3)  //调整闹钟分钟
           {
             Set_M++;
              if(Set_M==60)
               Set_M=0;
            }
					 
            if(s1num==4)  //调整小时
           {  
             Set_H++;
            if(Set_H==24)
              Set_H=0;
           } 
         }
       }
				 
			 
			 
          if(KEY1==0)  //减数
         {
           delay_ms(5);
           if(KEY1==0)
           {
             while(!KEY1);
              if(s1num==1)  //调整分钟
              {
                Set_min--;
                if(Set_min==-1)
                   Set_min=59;
              }
					   if(s1num==2)  //调整小时
              {
                Set_hour--;
                if(Set_hour==-1)
                   Set_hour=24;
              }
						if(s1num==3)  //调整闹钟分钟
              {
                Set_M--;
                if(Set_M==-1)
                   Set_M=59;
              }
					   if(s1num==4)  //调整闹钟小时
              {
                Set_H--;
                if(Set_H==-1)
                   Set_H=24;
              }
						}
				 }
       }
		 
	 RTC_Init();	
   OLED_Refresh_Gram();		
			 
}
