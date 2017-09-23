#include "myiic.h"
#include "delay.h"

void IQS263_IO_Init(void)
{
    GPIO_InitTypeDef IQS263;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
    IQS263.GPIO_Pin =  IQS263_SCL_PIN | IQS263_SDA_PIN;
    IQS263.GPIO_Mode = GPIO_Mode_Out_PP;
    IQS263.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(IQS263_I2C_PORT, &IQS263);

	  IQS263.GPIO_Pin =  GPIO_Pin_2;
    IQS263.GPIO_Mode = GPIO_Mode_IPD;
    IQS263.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &IQS263);

	
    IQS263_SCL_HIGH();
    IQS263_SDA_HIGH();

     delay_ms(4);

 
}

void IQS263_SCL_OUTPUT(void)
{
    GPIO_InitTypeDef IQS263;


    IQS263.GPIO_Pin =  IQS263_SCL_PIN;
    IQS263.GPIO_Mode = GPIO_Mode_Out_OD;
    IQS263.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(IQS263_I2C_PORT, &IQS263);
}

void IQS263_SCL_INPUT(void)
{
    GPIO_InitTypeDef IQS263;


    IQS263.GPIO_Pin =  IQS263_SCL_PIN;
    IQS263.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(IQS263_I2C_PORT, &IQS263);
}



void IQS263_SDA_OUTPUT(void)
{
    GPIO_InitTypeDef IQS263;


    IQS263.GPIO_Pin =  IQS263_SDA_PIN;
    IQS263.GPIO_Mode = GPIO_Mode_Out_OD;
    IQS263.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(IQS263_I2C_PORT, &IQS263);
}

void IQS263_SDA_INPUT(void)
{
    GPIO_InitTypeDef IQS263;


    IQS263.GPIO_Pin =  IQS263_SDA_PIN;
    IQS263.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(IQS263_I2C_PORT, &IQS263);
}


void IQS263_RDY_INPUT(void)
{
    GPIO_InitTypeDef IQS263;
    IQS263.GPIO_Pin =  GPIO_Pin_2;
    IQS263.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &IQS263);
	
}



void IQS263_I2cStartCondition(void)
{
		IQS263_SCL_OUTPUT();
		IQS263_SDA_OUTPUT();
		IQS263_RDY_INPUT();
	  while(!(I2C_RDY_READ()==0)); 
		IQS263_SCL_INPUT();
	  while(!IQS263_SCL_STATE());
	
		IQS263_SDA_HIGH();
	  while(!IQS263_SDA_STATE());
		delay_us(4);;
		IQS263_SDA_LOW();
	  IQS263_SDA_OUTPUT();
		delay_us(4);
	  IQS263_SCL_OUTPUT();
		IQS263_SCL_LOW();
		
	
}



	 void CommsIQSxxx_repeat_start(){
		 while(!(I2C_RDY_READ()==0)); 
		 IQS263_SCL_INPUT();
		 while(!IQS263_SCL_STATE());
		 IQS263_SDA_HIGH();
		 while(!IQS263_SDA_STATE());
		 delay_us(4);;
		 IQS263_SDA_LOW();
		 IQS263_SDA_OUTPUT();
		 delay_us(4);
		 IQS263_SCL_OUTPUT();
		 IQS263_SCL_LOW();
	 }

	 
	 
void IQS263_I2cStopCondition(void)
{
  
	  IQS263_SDA_LOW();
    IQS263_SDA_OUTPUT();
	  delay_us(1);
	  IQS263_SCL_INPUT();
   	while(!IQS263_SCL_STATE());
    delay_us(1);
    IQS263_SDA_HIGH();
	  while(!IQS263_SDA_STATE());
    delay_us(1);
}




uint8_t CommsIQSxxx_read_ack(void)
{
  uint8_t BitCnt,databuf = 0;     //位
  
  IQS263_SDA_INPUT();  
  
  for(BitCnt=0;BitCnt<8;BitCnt++)
  {   
    delay_us(1);  
		IQS263_SCL_INPUT(); 
    delay_us(1);  
    while (!IQS263_SCL_STATE());
    databuf=databuf<<1;
    delay_us(1);//1us
    if(IQS263_SDA_STATE())
    {
      databuf=databuf+1;     //读数据位，接收的数据放入cct中
    }
    delay_us(1);//1us
    IQS263_SCL_OUTPUT();
    IQS263_SCL_LOW();//SCL = 0;       
    delay_us(1);//1us
  }             
  
     IQS263_SDA_LOW();
	   IQS263_SDA_OUTPUT();
   

     while(IQS263_SDA_STATE());  
     delay_us(2);//1us
	   IQS263_SCL_INPUT();
     while (!IQS263_SCL_STATE());        //ACK
     delay_us(2);//2us
	   IQS263_SCL_OUTPUT();
     IQS263_SCL_LOW();
     delay_us(3);//1us
    return databuf;
}




	uint8_t CommsIQSxxx_read_nack(void)
	{
		uint8_t BitCnt,databuf = 0;     //位
    IQS263_SDA_INPUT();  
  for(BitCnt=0;BitCnt<8;BitCnt++)
		{   
			delay_us(1);  
			IQS263_SCL_INPUT(); 
			delay_us(1);  
			while (!IQS263_SCL_STATE());
			databuf=databuf<<1;
			delay_us(1);//1us
			if(IQS263_SDA_STATE())
			{
				databuf=databuf+1;     
			}
			delay_us(1);//1us
			IQS263_SCL_OUTPUT();
			IQS263_SCL_LOW();//SCL = 0;       
			delay_us(1);//1us
		}             
		
		  IQS263_SDA_HIGH();
	    while (!IQS263_SDA_STATE());  
		  delay_us(2);//1us
		  IQS263_SCL_INPUT();
		  while (!IQS263_SCL_STATE());       
		  delay_us(2);//2us
		  IQS263_SCL_OUTPUT();
		  IQS263_SCL_LOW();
		  delay_us(3);//1us
		 return databuf;
	}


uint8_t CommsIQSxxx_send(uint8_t send_data)
	{
		uint8_t  BitCnt;
		uint8_t time1 = 10;
		uint8_t ack;
		for (BitCnt=0; BitCnt<8; BitCnt++)  //要传送的数据长度为8位
		{
			if (send_data&0x80)    //判断发送位(从高位起发送)
			{
				IQS263_SDA_HIGH();
				while (!IQS263_SDA_STATE());   
			}
			else
			{				
				IQS263_SDA_LOW();//SDA = 0;	
				IQS263_SDA_OUTPUT();
			}
			send_data = send_data<<1;
			delay_us(1);
			IQS263_SCL_INPUT();
			while (!IQS263_SCL_STATE());
			delay_us(4);
			IQS263_SCL_OUTPUT();
			IQS263_SCL_LOW();
			delay_us(2);//2 us
		}
		 delay_us(2);
		 IQS263_SDA_INPUT();	
		 IQS263_SCL_INPUT();
		while (!IQS263_SCL_STATE());
		delay_us(1);
		while (IQS263_SDA_STATE())
		{
			time1--;
			delay_us(10); 
			if (time1==0)
			{
				 delay_us(1);
				 IQS263_SCL_OUTPUT();
				 IQS263_SCL_LOW();
			   delay_us(1);
				 ack = 0;
				 IQS263_SDA_HIGH();
				 return ack;
			}
		}
		delay_us(1);
    IQS263_SCL_OUTPUT();
		IQS263_SCL_LOW();
		delay_us(1);
		ack=1;
		IQS263_SDA_HIGH();
		return ack;
		
	}

	








