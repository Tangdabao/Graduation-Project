#include "LP55231.h" 
#include "delay.h"

unsigned char data_buffer_LP[10];


void LP55_I2c2Init()
{
    GPIO_InitTypeDef LP55231;

 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);


  
    LP55231.GPIO_Pin =  SCL2 | SDA2;
    LP55231.GPIO_Speed = GPIO_Speed_50MHz;
    LP55231.GPIO_Mode =GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &LP55231);

    SDA2High();
    SCL2High();

    LP55231.GPIO_Pin  = LPENPin;
    LP55231.GPIO_Mode = GPIO_Mode_Out_PP;
    LP55231.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &LP55231);

    GPIO_WriteBit(LPENPort,LPENPin,Bit_SET);
		
		
}

/******************************************************************************/
void SetSDA2Output(void)
{
    GPIO_InitTypeDef LP55231;
    LP55231.GPIO_Pin = SDA2;
    LP55231.GPIO_Mode = GPIO_Mode_Out_OD;
    LP55231.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &LP55231);
}

void SetSDA2Input(void)
{
    GPIO_InitTypeDef LP55231;
    LP55231.GPIO_Pin = SDA2;
    LP55231.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &LP55231);
	
}

void APP_I2c2Start()
{
    
	  SDA2High();
    delay_us(5);
    SCL2High();
    delay_us(5);
    SDA2Low();
    delay_us(5);
    SCL2Low();
    delay_us(5);
}
/******************************************************************************/


void APP_I2c2Stop()
{
    SDA2Low();
    delay_us(5);
    SCL2High();
    delay_us(5);
    SDA2High();
    delay_us(5);
    SCL2Low();
    delay_us(5);
}

/*******************************************************************************/
void APP_I2c2WriteByte(u8 u8Byte)
{
    u8  i;

    for(i=0;i<8;i++)
    {
        if((u8Byte<<i)&0x80)
        {   
            SDA2High();
        }
        else
        {
            SDA2Low();
        }

        delay_us(5);
        SCL2High();
        delay_us(5);
        SCL2Low();
        delay_us(5);
    }
}

/******************************************************************************/
u8 APP_I2c2ReadAck()
{
	  u8  u8Ret = 1;
    SetSDA2Input();
	  delay_us(3);
    SCL2High();
    delay_us(5);
	if(GetSDA2())
	{
	   u8Ret = 0;
	}
	
    SCL2Low();
    delay_us(1);
    SetSDA2Output();
    delay_us(4);
    return  u8Ret;
		
}
/******************************************************************************/


void I2C2_Ack(void)
{
	SCL2Low();
	SetSDA2Output();
	SDA2Low();
	delay_us(5);
	SCL2High();
	delay_us(5);
	SCL2Low();
}

/*******************************************************************************/	    
void I2C2_NAck(void)
{
	SCL2Low();
	SetSDA2Output();
	SDA2High();
	delay_us(5);
	SCL2High();
	delay_us(5);
	SCL2Low();
}	

/*******************************************************************************/
u8 I2C2_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
    SetSDA2Input();
    for(i=0;i<8;i++ )
	{
        SCL2Low();
        delay_us(4);
		    SCL2High();
        receive<<=1;
        if(GetSDA2())receive++;   
		    delay_us(1); 
    }					 
    if (!ack)
        I2C2_NAck();
    else
        I2C2_Ack(); 
    return receive;
}
/******************************************************************************/
void LP55231WriteSingle(u8 ChipAdd,u8 AddPtr,u8 I2C2Data)
{
	
	
    APP_I2c2Start();
    APP_I2c2WriteByte(ChipAdd);
    APP_I2c2ReadAck();
    APP_I2c2WriteByte(AddPtr);
    APP_I2c2ReadAck();
    APP_I2c2WriteByte(I2C2Data);
    APP_I2c2ReadAck();
    APP_I2c2Stop();	
	
}





void LP55231WriteBytes(u8 ChipAdd,u8 AddPtr,u8 Length,u8 *I2C2Data)
{
    u8  i;
    APP_I2c2Start();
    APP_I2c2WriteByte(ChipAdd);
    APP_I2c2ReadAck();
    APP_I2c2WriteByte(AddPtr);
    APP_I2c2ReadAck();
    
    for(i=0;i<Length;i++)
    {
        APP_I2c2WriteByte(I2C2Data[i]);
        APP_I2c2ReadAck();
    }
    APP_I2c2Stop();
}



/******************************************************************************/

void APP_LP55231ReadSingle(u8 ChipAdd,u8 AddPtr,u8 *data,u8 NoOfBytes)
{
	u8 i;
    APP_I2c2Start();
    APP_I2c2WriteByte(ChipAdd);
    APP_I2c2ReadAck();
    APP_I2c2WriteByte(AddPtr);
    APP_I2c2ReadAck();
	  APP_I2c2Start();
    APP_I2c2WriteByte(ChipAdd+0x01);
    APP_I2c2ReadAck();  
  if (NoOfBytes > 1)
    {
        for (i = 0; i < NoOfBytes - 1; i++)
			   data[i]=I2C2_Read_Byte(1);
    }
    data[NoOfBytes-1] = I2C2_Read_Byte(0);  
    APP_I2c2Stop();	
}


void LPP55231_CLOSE(void)
{
	  LP55231WriteSingle(ADD1,0x00,0x00);//CHIP NO EN
		delay_us(200);
}



void Breathing_LED_PWM_1(unsigned char val)
{
	  LP55231WriteSingle(ADD1,0x00,0x00);
		delay_us(200);
    LP55231WriteSingle(ADD1,0x3D,0xFF);
    delay_us(200);
    LP55231WriteSingle(0x64,0x00,0x40);
    delay_us(200);
    LP55231WriteSingle(0x64,0x36,0x4D);
    delay_us(200);
	
	if(val>15)
	{
	  LP55231WriteSingle(0x64,0x16,val);//D1
    LP55231WriteSingle(0x64,0x17,255-val);
    LP55231WriteSingle(0x64,0x18,val);
    LP55231WriteSingle(0x64,0x19,255-val);
    LP55231WriteSingle(0x64,0x1A,val);  
    LP55231WriteSingle(0x64,0x1B,255-val);
    LP55231WriteSingle(0x64,0x1C,(255+val)/2);
    LP55231WriteSingle(0x64,0x1D,(255+val)/2);  
    LP55231WriteSingle(0x64,0x1E,(255+val)/2);//D9
	}else 
	{
		LP55231WriteSingle(0x64,0x16,val);//D1
    LP55231WriteSingle(0x64,0x17,val);
    LP55231WriteSingle(0x64,0x18,val);
    LP55231WriteSingle(0x64,0x19,val);
    LP55231WriteSingle(0x64,0x1A,val);  
    LP55231WriteSingle(0x64,0x1B,val);
    LP55231WriteSingle(0x64,0x1C,val);
    LP55231WriteSingle(0x64,0x1D,val);  
    LP55231WriteSingle(0x64,0x1E,val);//D
	}
}
