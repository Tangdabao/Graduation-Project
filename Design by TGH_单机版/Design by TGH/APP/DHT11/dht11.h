#ifndef __DHT11_H
#define __DHT11_H 
#include "sys.h"   

//IO��������
#define DHT11_IO_IN()  {GPIOA->CRH&=0XFFFFFFF0;GPIOA->CRH|=8<<0;}
#define DHT11_IO_OUT() {GPIOA->CRH&=0XFFFFFFF0;GPIOA->CRH|=3<<0;}
////IO��������											   
#define	DHT11_DQ_OUT PAout(8) 
#define	DHT11_DQ_IN  PAin(8) 


u8 DHT11_Init(void);//��ʼ��DHT11
u8 DHT11_Read_Data(u8 *temp,u8 *humi);//��ȡ��ʪ��
u8 DHT11_Read_Byte(void);//����һ���ֽ�
u8 DHT11_Read_Bit(void);//����һ��λ
u8 DHT11_Check(void);//����Ƿ����DHT11
void DHT11_Rst(void);//��λDHT11    
#endif















