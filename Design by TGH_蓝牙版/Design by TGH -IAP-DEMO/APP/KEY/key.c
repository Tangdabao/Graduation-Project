#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"

void KEY_Init(void)
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Pin  =  GPIO_Pin_5|GPIO_Pin_9|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
		

}



u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)key_up=1;  //֧������		  
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY0==0)return KEY0_PRES;
		else if(KEY1==0)return KEY1_PRES;	
		 else if(KEY2==0)return KEY2_PRES;	
	}else if(KEY0==1&&KEY1==1&&KEY2==1)key_up=1; 	     
	return 0;// �ް�������
}
