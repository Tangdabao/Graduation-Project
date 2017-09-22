#include "standby.h"
#include "led.h"
#include "delay.h"

void Sys_Standby(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//ʹ��PWR����ʱ��
    PWR_WakeUpPinCmd(ENABLE);  //ʹ�ܻ��ѹܽŹ���
    PWR_EnterSTANDBYMode();	  //���������STANDBY��ģʽ
}

//ϵͳ�������ģʽ
void Sys_Enter_Standby(void)
{
    RCC_APB2PeriphResetCmd(0X01FC, DISABLE);	//��λ����IO��
    Sys_Standby();
}






//���WKUP�ŵ��ź�
//����ֵ1:��������3s����
//      0:����Ĵ���
u8 Check_WKUP(void)
{
    u8 t = 0;
    u8 tx = 0; //��¼�ɿ��Ĵ���
    LED0 = 0; //����DS0
    while(1)
    {
        if(WKUP_KD)//�Ѿ�������
        {
            t++;
            tx = 0;
        }
        else
        {
            tx++; //����300ms��û��WKUP�ź�
            if(tx > 3)
            {
                LED0 = 1;
                return 0;//����İ���,���´�������
            }
        }
        delay_ms(30);
        if(t >= 100) //���³���3����
        {
            LED0 = 0;	 //����DS0
            return 1; //����3s������
        }
    }
}







//�ж�,��⵽PA0�ŵ�һ��������.
//�ж���0���ϵ��жϼ��
void EXTI0_IRQHandler(void)
{
    EXTI_ClearITPendingBit(EXTI_Line0); // ���LINE10�ϵ��жϱ�־λ
    if(Check_WKUP())//�ػ�?
    {
        Sys_Enter_Standby();
    }
}




//PA0 WKUP���ѳ�ʼ��
void WKUP_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);//ʹ��GPIOA�͸��ù���ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	 //PA.0
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);	//��ʼ��IO
    //GPIO_SetBits(GPIOA,GPIO_Pin_0);						 //PA,,ALL�����
    //ʹ���ⲿ�жϷ�ʽ
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);	//�ж���0����GPIOA.0

    EXTI_InitStructure.EXTI_Line = EXTI_Line0;	//���ð������е��ⲿ��·
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//�����ⲿ�ж�ģʽ:EXTI��·Ϊ�ж�����
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  //�����ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	// ��ʼ���ⲿ�ж�

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //��ռ���ȼ�2��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //�����ȼ�2��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

    if(Check_WKUP() == 0) Sys_Standby();  //���ǿ���,�������ģʽ

}









