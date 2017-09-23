#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"

#define IQS263_I2C_PORT          GPIOB
#define IQS263_SCL_PIN           GPIO_Pin_6
#define IQS263_SDA_PIN           GPIO_Pin_7

#define IQS263_SCL_HIGH()        GPIO_SetBits(IQS263_I2C_PORT, IQS263_SCL_PIN)
#define IQS263_SCL_LOW()         GPIO_ResetBits(IQS263_I2C_PORT, IQS263_SCL_PIN)

#define IQS263_SDA_HIGH() 	      GPIO_SetBits(IQS263_I2C_PORT, IQS263_SDA_PIN)
#define IQS263_SDA_LOW()         GPIO_ResetBits(IQS263_I2C_PORT, IQS263_SDA_PIN)

#define IQS263_SCL_STATE()       GPIO_ReadInputDataBit(IQS263_I2C_PORT, IQS263_SCL_PIN)
#define IQS263_SDA_STATE()       GPIO_ReadInputDataBit(IQS263_I2C_PORT, IQS263_SDA_PIN)
#define I2C_RDY_READ()           GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)


void IQS263_IO_Init(void);
void IQS263_I2cStartCondition(void);
void CommsIQSxxx_repeat_start(void);
void IQS263_I2cStopCondition(void);
uint8_t CommsIQSxxx_read_nack(void);
uint8_t CommsIQSxxx_read_ack(void);
uint8_t CommsIQSxxx_send(uint8_t send_data);



#endif
















