#include "IQS263.h" 
#include "delay.h"
#include "myiic.h"
#include "usart.h"	
/*****************************触摸参数 *******************************/

unsigned char touch0 = 0;                   
unsigned char touch_val = 0;                   
unsigned char prox = 0;                         
unsigned char sliderCoords;                     
unsigned char movement;                        
unsigned char tap = 0;                          
unsigned char leftFlick;                        
unsigned char rightFlick;                       
unsigned char events = 0;                       
unsigned char data_buffer[30];



//初始化IIC接口
void IQS263_Init(void)
{
	
	IQS263_IO_Init();
  i2c_event_mode_handshake();
	IQS263DeviceSettings();

	
}


///////*****************************************************///////////////////
void CommsIQS_Read(unsigned char i2c_addr, unsigned char read_addr, unsigned char *data, unsigned char NoOfBytes)
{
    unsigned char i;

    CommsIQSxxx_send((i2c_addr << 1) + 0x00);      
    CommsIQSxxx_send(read_addr);                   
    CommsIQSxxx_repeat_start();
    CommsIQSxxx_send((i2c_addr << 1) + 0x01);      
    if (NoOfBytes > 1)
    {
        for (i = 0; i < NoOfBytes - 1; i++)
         data[i] = CommsIQSxxx_read_ack();      
    }
    data[NoOfBytes-1] = CommsIQSxxx_read_nack();   

}



///////*****************************************************///////////////////

void CommsIQS_Write(unsigned char i2c_addr, unsigned char write_addr, unsigned char *data, unsigned char NoOfBytes)
{
    unsigned char i;
    CommsIQSxxx_send((i2c_addr << 1) + 0x00);      // device address + write
    CommsIQSxxx_send(write_addr);                  // IQS address-command
    for (i = 0; i < NoOfBytes; i++)
        CommsIQSxxx_send(data[i]);
}


///////*****************************************************///////////////////
void i2c_event_mode_handshake(void)
{

		data_buffer[0] = PROXSETTINGS0_VAL;
    data_buffer[1] = PROXSETTINGS1_VAL;
 
   IQS263_I2cStartCondition();
    CommsIQS_Write(IQS263_ADDR, PROX_SETTINGS, &data_buffer[0], 2);
    IQS263_I2cStopCondition();
}




///////*****************************************************///////////////////
void IQS263DeviceSettings(void)
{

  
    IQS263_I2cStartCondition();
    CommsIQS_Read(IQS263_ADDR, DEVICE_INFO, &data_buffer[0], 2);
    IQS263_I2cStopCondition();

 
    data_buffer[0] = SYSTEM_FLAGS_VAL;
    IQS263_I2cStartCondition();
    CommsIQS_Write(IQS263_ADDR, SYS_FLAGS, &data_buffer[0], 1);
    IQS263_I2cStopCondition();


    data_buffer[0] = ACTIVE_CHS;
   IQS263_I2cStartCondition();
    CommsIQS_Write(IQS263_ADDR, ACTIVE_CHANNELS, &data_buffer[0], 1);
    IQS263_I2cStopCondition();

  
    data_buffer[0] = PROX_THRESHOLD;
    data_buffer[1] = TOUCH_THRESHOLD_CH1;
    data_buffer[2] = TOUCH_THRESHOLD_CH2;
    data_buffer[3] = TOUCH_THRESHOLD_CH3;
    data_buffer[4] = MOVEMENT_THRESHOLD;
    data_buffer[5] = RESEED_BLOCK;
    data_buffer[6] = HALT_TIME;
    data_buffer[7] = I2C_TIMEOUT;
   IQS263_I2cStartCondition();
    CommsIQS_Write(IQS263_ADDR, THRESHOLDS, &data_buffer[0], 8);
    IQS263_I2cStopCondition();


		data_buffer[0] = LOW_POWER;  
    data_buffer[1] = ATI_TARGET_TOUCH;
    data_buffer[2] = ATI_TARGET_PROX;
   IQS263_I2cStartCondition();
    CommsIQS_Write(IQS263_ADDR, TIMINGS_AND_TARGETS, &data_buffer[0], 3);
    IQS263_I2cStopCondition();


    data_buffer[0] = MULTIPLIERS_CH0;
    data_buffer[1] = MULTIPLIERS_CH1;
    data_buffer[2] = MULTIPLIERS_CH2;
    data_buffer[3] = MULTIPLIERS_CH3;
   IQS263_I2cStartCondition();
    CommsIQS_Write(IQS263_ADDR, MULTIPLIERS, &data_buffer[0], 4);
    IQS263_I2cStopCondition();

 
    data_buffer[0] = PROXSETTINGS0_VAL;
    data_buffer[1] = PROXSETTINGS1_VAL;
    data_buffer[2] = PROXSETTINGS2_VAL;
    data_buffer[3] = PROXSETTINGS3_VAL;
    data_buffer[4] = EVENT_MASK_VAL;
   IQS263_I2cStartCondition();
    CommsIQS_Write(IQS263_ADDR, PROX_SETTINGS, &data_buffer[0], 5);
    IQS263_I2cStopCondition();

 
    data_buffer[0] = COMPENSATION_CH0;
    data_buffer[1] = COMPENSATION_CH1;
    data_buffer[2] = COMPENSATION_CH2;
    data_buffer[3] = COMPENSATION_CH3;
   IQS263_I2cStartCondition();
    CommsIQS_Write(IQS263_ADDR, COMPENSATION, &data_buffer[0], 4);
    IQS263_I2cStopCondition();

 
    data_buffer[0] = LOW_POWER; 
   IQS263_I2cStartCondition();
    CommsIQS_Write(IQS263_ADDR, TIMINGS_AND_TARGETS, &data_buffer[0], 1);
    IQS263_I2cStopCondition();

   
    data_buffer[0] = TAP_TIMER;
    data_buffer[1] = FLICK_TIMER;
    data_buffer[2] = FLICK_THRESHOLD;
   IQS263_I2cStartCondition();
    CommsIQS_Write(IQS263_ADDR, GESTURE_TIMERS, &data_buffer[0], 3);
    IQS263_I2cStopCondition();

 
    data_buffer[0] = 0x10;
    IQS263_I2cStartCondition();
    CommsIQS_Write(IQS263_ADDR, PROX_SETTINGS, &data_buffer[0], 1);
    IQS263_I2cStopCondition();

   
    do
    {
        delay_ms(10);
       IQS263_I2cStartCondition();
        CommsIQS_Read(IQS263_ADDR, SYS_FLAGS, &data_buffer[0], 1);
        IQS263_I2cStopCondition();
    }while((data_buffer[0] & 0x04) == 0x04);
#if 1
   
    data_buffer[0] = PROXSETTINGS0_VAL;
    data_buffer[1] = (PROXSETTINGS1_VAL|0x0040);   //go to event
    data_buffer[2] = (PROXSETTINGS2_VAL|0x0002);
    data_buffer[3] = PROXSETTINGS3_VAL;
    data_buffer[4] = EVENT_MASK_VAL;
   IQS263_I2cStartCondition();
    CommsIQS_Write(IQS263_ADDR, PROX_SETTINGS, &data_buffer[0], 5);
    IQS263_I2cStopCondition();   
#endif
}

///////*****************************************************///////////////////
void handleEvents(void)
{
    IQS263_I2cStartCondition();                                                          
    CommsIQS_Read(IQS263_ADDR, SYS_FLAGS, &data_buffer[0], 2);                  
    CommsIQSxxx_repeat_start();                                                    
    CommsIQS_Read(IQS263_ADDR, TOUCH_BYTES, &data_buffer[2], 4);                
    CommsIQSxxx_repeat_start();                                                    
    CommsIQS_Read(IQS263_ADDR, COORDINATES, &data_buffer[3], 3);               
    IQS263_I2cStopCondition();                                                           

 
  touch0 = data_buffer[2];

	touch_val =data_buffer[3];
	
	
}
















