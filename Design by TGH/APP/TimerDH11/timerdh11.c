#include "timerdh11.h"
#include "ds18b20.h"
#include "dht11.h"
#include "oled.h"

//////温湿度参数/////////
short temperature;
u8 temperature_DHT11;
u8 humidity;


void TIM3_Int_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);


    TIM_TimeBaseStructure.TIM_Period = arr;
    TIM_TimeBaseStructure.TIM_Prescaler = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE );


    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    TIM_Cmd(TIM3, ENABLE);
}


void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );

        ///////////湿度读取//////////
        DHT11_Read_Data(&temperature_DHT11, &humidity);	//读取温湿度值
        OLED_ShowNum(80 - 12, 12, temperature_DHT11, 2, 12);	//显示温度
        OLED_ShowChar(92 - 12, 12, 'C', 12, 1);

        OLED_ShowNum(98, 12, humidity, 2, 12);		//显示湿度
        OLED_ShowString(110, 12, "RH", 12);


        ///////////温度读取//////////

        /*temperature=DS18B20_Get_Temp();
        	if(temperature<0)
        	{
        		OLED_ShowChar(72,12,'-',12,1);			//显示负号
        		OLED_Refresh_Gram();
        		temperature=-temperature;					//转为正数
        	}else
        	  OLED_ShowChar(72,12,' ',12,1);			//去掉负号

          	OLED_ShowNum(78,12,temperature/10,2,12);	//显示正数部分
        	  OLED_ShowChar(90,12,'.',12,1);
        	OLED_ShowNum(96,12,temperature%10,1,12);	//显示小数部分
        	  OLED_ShowChar(102,12,'C',12,1);
             OLED_Refresh_Gram();
        	*/


    }
}












