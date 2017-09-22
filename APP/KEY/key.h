#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

#define KEY0  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)
#define KEY1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)
#define KEY2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)

#define KEY0_PRES 	1	
#define KEY1_PRES	  2	
#define KEY2_PRES	  3	
void KEY_Init(void);
void SET_shijian(void);
#endif
