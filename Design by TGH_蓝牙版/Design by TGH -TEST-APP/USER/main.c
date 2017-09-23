/*******************************************************************************
** �ļ���: 	  main.c
** �汾��  	
** ��������:  KEIL5.14
** ����:      �ƹ���
** ��������: 	2017-03-20
** ����:      ��ҵ���
** ����ļ�:	  ʹ����ԭ�ӵ��ӵĵײ��ļ���������#include "sys.h"��#include "delay.h"������������ʾ��л
** ������־:
2017-03-27:IQS263�������ֳ���������ɣ�
2017-04-03:���OLED��ʾ����һ���Ż�IQS263
2017-04-20:���RTCʵʱʱ�ӣ������������
2017-04-21:����LP5110�����Ƿ��ַ�װ����
2017-04-24:��ӷ���������
      *************�ش����:��PCB����*****************	         
2017-04-27:LP5110��ʼ�������LP55231�¶ȼ�⺯������Ժ�����
           ������ӵ�����������OLED�ײ����					 
2017-04-29:����OLED���룬��֮ǰ�������޸ĳ�5�У�x,0-48,y,0-118;	
           ���LP55231����,����PWM���ģʽ���Ա��ģʽ���ģʽ
2017-04-30:���LP55231ע�ͣ��Ż�LP55231�¶���ʾ,���6����ʾģʽ��
           ���AT24C256����,���������д���Դ���
2017-05-01:���DS18B20����,�¶������⣬δ�޸�
           ���DH111����
					 �޸�IQS263���ܵ���BUG�����ͨ��IQS263����RGB���ܣ�
					 ����������ɣ������Ż����ɣ�
2017-05-02:DS18B20����BUG�޸���ԭ��������Ҫ470ms�ȴ�ת��ʱ��
           ��Ӷ�ʱ���жϴ��룬����DHT11�ļ�⣻��ֹ���ȡʱ��Ӱ��IQS263��	
           ���µ���OLED���֣��޸����������֣��򻯴���������	
2017-05-03:�޸�RTC,��Ӵ������ѣ�
           ��BUG����,����ÿ�λ��Ѷ���ִ��һ��ͷ������
2017-05-04:�޸�RTC,�޸ĵ���������������������ɵ���������Сʱ���ӵȹ��ܴ��룬�������Ƽ�����
           �޸�����BUG���������Ӳ����޸�ʱ�䣻
           ��BUG������ÿ�λ��Ѷ���ִ��һ��ͷ�������ɴ��ڣ�����������Ҫ����
					 ����һ�����𣬲�ͣ��
2017-05-05:ʲôҲû�޸�
2017-05-06:ʲôҲû�޸�
2017-05-07:�޸���������ģʽ�������˼·��ɨ��������Ļ����ʱ���Ե������ȣ�������ϣ��Զ�
           �������ģʽ��
      *************�ش����*****************					 
2017-05-08:���IAP����������;�������ƣ��޸�BUG��ֻ��ִ��һ���ֽ���취������Ϊ�жϴ������
2017-05-09:���IAPԭ��
2017-05-10:�Ż����룬�����������٣����������������
2017-05-11:��RAM���㣬Ѱ�������취������ʹ��AT24C256����EEPROM��
2017-05-12:���EEPROM���룬�������ڼ������⵼��δ����ӣ�Ѱ�������취
2017-05-13:����YModemЭ�飬���ô˷�����
2017-05-14:���YModem������BUG������ִ��֮ǰ�ĳ���
      *************�ش����*****************
2017-05-15:�޸�YModem����BUG������Ƿ���³���ѡ����û����о�����
*******************************************************************************/
#include "sys.h"
#include "delay.h"
#include "usart.h"	
#include "myiic.h"
#include "IQS263.h"
#include "LP55231.h" 
#include "oled.h"
#include "rtc.h"
#include "beep.h"
#include "at24c256.h"
#include "dht11.h"
#include "key.h"


///////������ʾ����///////////	
u8 t;	

///////��ʾ����///////////	
void Display(void);


///////RGB_LED��ʾ����/////////
void LP55231_Display(void);


 int main(void)
 {
	 
	SCB->VTOR = FLASH_BASE|0x5000; 
	
	 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init();	    	 //��ʱ������ʼ��	
	BEEP_Init();
	KEY_Init();
	OLED_Init();			   
  RTC_Init();	 
	IQS263_Init();			
	LP55_I2c2Init();   
  IIC_AT24C256_Init();
	DHT11_Init();
	LPP55231_CLOSE();
	OLED_Clear(); 
	 
////////������ʾ����///////// 
  Display();
 // LP55231_Display();

while(1){

//////��������ʾ����////////	
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
					OLED_ShowString(66,0,"Wedneday",12);
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
 
 
 void Display(){	 
///////****************��һ��_������ʾ��Ϣ*************///////////////////
	OLED_ShowString(0,0, "HAPPY",12);
	OLED_ShowString(0,12,"EVERY DAY",12);
 	OLED_ShowString(0,24,"BI YE SHE JI",12);
	OLED_ShowString(0,36,"BI YE SHE JI",12);
	OLED_ShowString(0,48,"BI YE SHE JI",12);
  OLED_Refresh_Gram();	
	delay_ms(1000);
	delay_ms(1000); 
  delay_ms(1000);
	delay_ms(1000); 
  OLED_Clear();


///////****************�ڶ���_������ʾ����ģ��ĳ�ʼ�����*************///////////////////
//////����IQS263
 while(IQS263_Test());
	OLED_ShowString(0,0, "IQS263_OK",12);
  OLED_Refresh_Gram();	
	delay_ms(1000);
	delay_ms(1000); 

//////����LP55231_Test
 while(LP55231_Test());
	OLED_ShowString(0,12,"LP55231_OK ",12);
  OLED_Refresh_Gram();	
	delay_ms(1000);
	delay_ms(1000);

////����RTC_Init
 while(RTC_Init());
	OLED_ShowString(0,24,"RTC OK",12); 
  OLED_Refresh_Gram();	
  delay_ms(1000);
	delay_ms(1000); 


////���24c0256
 while(AT24CXX_Check());
	OLED_ShowString(0 ,36,"AT24C256 OK",12); 
  OLED_Refresh_Gram();	
  delay_ms(1000);
	delay_ms(1000); 

//////���18B20
// while(DS18B20_Init());
//  OLED_ShowString(0 ,48,"18B20 OK",12); 
//  OLED_Refresh_Gram();	
//  delay_ms(1000);
//  delay_ms(1000); 

////���DHT11
//	while(DHT11_Init());
//	OLED_ShowString(64,24,"DHT11 OK",12); 
//	OLED_Refresh_Gram();	
//	delay_ms(1000);
//	delay_ms(1000); 
  OLED_Clear();	


 }
 
 void LP55231_Display(){
 
 
///LP55231�¶ȶ�ȡ��
//    Read_temperature();
//    OLED_ShowNum(55,24,data_buffer_LP[0],2,12);	
//    OLED_Refresh_Gram();
	
	
////LP55231��ʾ////////////////////////	
 Breathing_LED_PWM();	
 delay_ms(800);
 delay_ms(800);

 Breathing_LED_Blue();
 delay_ms(800);
 delay_ms(800);
 delay_ms(800);
 delay_ms(800);
 Breathing_LED_Red();
 delay_ms(800);
 delay_ms(800);
 delay_ms(800);
 delay_ms(800);
 Breathing_LED_Green_1();
 delay_ms(800);
 delay_ms(800);
 delay_ms(800);
 delay_ms(800);
 Breathing_LED_Green_2();
 delay_ms(800);
 delay_ms(800);
 delay_ms(800);
 delay_ms(800);
 Breathing_LED_Green_3();
 delay_ms(800);
 delay_ms(800);
 delay_ms(800);
 delay_ms(800);
 Breathing_LED_Green_4();
 delay_ms(800);
 delay_ms(800);
 delay_ms(800);
 delay_ms(800);
 
 ////̫���ˣ��°������ڹر�
 LPP55231_CLOSE();
 }
 
