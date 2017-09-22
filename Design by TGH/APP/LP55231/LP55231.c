#include "LP55231.h"
#include "delay.h"

unsigned char data_buffer_LP[10];


/*******************************************************************************
Function:I2C2 Initial and Back Light Control
Input:
Output:
Note:
BY:TGH
*******************************************************************************/
void LP55_I2c2Init()
{
    GPIO_InitTypeDef LP55231;

    /* Enable GPIOA clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);


    /* Configure I2C2 pins: SCL2 and SDA2 */
    LP55231.GPIO_Pin =  SCL2 | SDA2;
    LP55231.GPIO_Speed = GPIO_Speed_50MHz;
    LP55231.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &LP55231);

    SDA2High();
    SCL2High();

    //LP55231EN
    LP55231.GPIO_Pin  = LPENPin;
    LP55231.GPIO_Mode = GPIO_Mode_Out_PP;
    LP55231.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &LP55231);

    GPIO_WriteBit(LPENPort, LPENPin, Bit_SET);


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


/*******************************************************************************
Function:I2C2 Start
Input:
Output:
Note:
*******************************************************************************/
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


/*******************************************************************************
Function:I2C2 Stop
Input:
Output:
Note:
*******************************************************************************/
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

/*******************************************************************************
Function:Write the Byte Data
Input:the date of the sending
Output:
Note:MSB first.
*******************************************************************************/
void APP_I2c2WriteByte(u8 u8Byte)
{
    u8  i;

    for(i = 0; i < 8; i++)
    {
        if((u8Byte << i) & 0x80)
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
/*******************************************************************************
Function:I2C2 Read Ack
Input:
Output:
Note:
BY:TGH
*******************************************************************************/
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

/*******************************************************************************
Function:I2C2_Ack
Input:
Output:
Note:
BY:TGH
*******************************************************************************/

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

/*******************************************************************************
Function:I2C2_NAck
Input:
Output:
Note:
BY:TGH
*******************************************************************************/
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

/*******************************************************************************
Function:I2C2_Read_Byte
Input:
Output:
Note:
BY:TGH
*******************************************************************************/
u8 I2C2_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    SetSDA2Input();
    for(i = 0; i < 8; i++ )
    {
        SCL2Low();
        delay_us(4);
        SCL2High();
        receive <<= 1;
        if(GetSDA2())receive++;
        delay_us(1);
    }
    if (!ack)
        I2C2_NAck();//发送nACK
    else
        I2C2_Ack(); //发送ACK
    return receive;
}
/******************************************************************************/

/*=============================================================================*
 * Function Name : LP55231WriteSingle
 * Description   :
 * Argument      :
 * Return        : none
 * BY            : TGH
 *=============================================================================*/
void LP55231WriteSingle(u8 ChipAdd, u8 AddPtr, u8 I2C2Data)
{


    APP_I2c2Start();
    APP_I2c2WriteByte(ChipAdd);//address
    APP_I2c2ReadAck();
    APP_I2c2WriteByte(AddPtr);//register addr
    APP_I2c2ReadAck();
    APP_I2c2WriteByte(I2C2Data);//data
    APP_I2c2ReadAck();
    APP_I2c2Stop();

}


/*=============================================================================*
 * Function Name : LP55231WriteSingle
 * Description   :
 * Argument      :
 * Return        : none
 * BY            : TGH
 *=============================================================================*/


void LP55231WriteBytes(u8 ChipAdd, u8 AddPtr, u8 Length, u8 *I2C2Data)
{
    u8  i;
    APP_I2c2Start();
    APP_I2c2WriteByte(ChipAdd);//address
    APP_I2c2ReadAck();
    APP_I2c2WriteByte(AddPtr);//register addr
    APP_I2c2ReadAck();

    for(i = 0; i < Length; i++)
    {
        APP_I2c2WriteByte(I2C2Data[i]);//data
        APP_I2c2ReadAck();
    }
    APP_I2c2Stop();
}



/******************************************************************************/

/*=============================================================================*
 * Function Name : APP_LP55231WriteSingle
 * Description   :
 * Return        : none
 * BY            : TGH
 *=============================================================================*/
void APP_LP55231ReadSingle(u8 ChipAdd, u8 AddPtr, u8 *data, u8 NoOfBytes)
{
    u8 i;
    APP_I2c2Start();
    APP_I2c2WriteByte(ChipAdd);//xie address
    APP_I2c2ReadAck();
    APP_I2c2WriteByte(AddPtr);//register addr
    APP_I2c2ReadAck();
    APP_I2c2Start();
    APP_I2c2WriteByte(ChipAdd + 0x01); //du address
    APP_I2c2ReadAck();
    if (NoOfBytes > 1)
    {
        for (i = 0; i < NoOfBytes - 1; i++)
            data[i] = I2C2_Read_Byte(1); // all bytes except las must be followed by ACK
    }
    data[NoOfBytes - 1] = I2C2_Read_Byte(0); // last byte read must be followed by a NACK
    APP_I2c2Stop();
}



/*=============================================================================*
 * Function Name : Read_temperature
 * Description   :
 * Argument      :
 * Return        : none
 * BY            : TGH
 *=============================================================================*/
void Read_temperature(void)
{

    LP55231WriteSingle(ADD1, 0x3D, 0xFF);
    delay_us(2000);
    LP55231WriteSingle(ADD1, 0x00, 0x40); //CHIP EN
    delay_us(2000);
    LP55231WriteSingle(ADD1, 0x3E, 0x04); //TEMP EN
    delay_ms(25);
    APP_LP55231ReadSingle(ADD1, 0x3f, &data_buffer_LP[0], 1);

}


/*=============================================================================*
 * Function Name : Read_temperature
 * Description   :
 * Argument      :
 * Return        : none
 * BY            : TGH
 *=============================================================================*/
uint8_t LP55231_Test(void)
{
    data_buffer_LP[0] = 0;
    LP55231WriteSingle(ADD1, 0x40, 0xFF);
    delay_us(2000);
    APP_LP55231ReadSingle(ADD1, 0x40, &data_buffer_LP[0], 1);
    if(data_buffer_LP[0] == 0xff)
        return 0;
    else
        return 1;
}

/*=============================================================================*
 * Function Name : LPP55231_CLOSE
 * Description   :
 * Argument      :
 * Return        : none
 * BY            : TGH
 *=============================================================================*/
void LPP55231_CLOSE(void)
{
    LP55231WriteSingle(ADD1, 0x00, 0x00); //CHIP NO EN
    delay_us(200);
}


/*=============================================================================*
 * Function Name : Breathing_LED_PWM_1
 * Description   :
 * Argument      :
 * Return        : none
 * BY            : TGH
 *=============================================================================*/
void Breathing_LED_PWM_1(unsigned char val)
{
    LP55231WriteSingle(ADD1, 0x00, 0x00); //关闭
    delay_us(200);
    LP55231WriteSingle(ADD1, 0x3D, 0xFF); ///复位
    delay_us(200);
    LP55231WriteSingle(0x64, 0x00, 0x40); //使能
    delay_us(200);
    LP55231WriteSingle(0x64, 0x36, 0x4D); //初始化
    delay_us(200);


    if(val > 15)
    {
        LP55231WriteSingle(0x64, 0x16, val); //D1
        LP55231WriteSingle(0x64, 0x17, 255 - val);
        LP55231WriteSingle(0x64, 0x18, val);
        LP55231WriteSingle(0x64, 0x19, 255 - val);
        LP55231WriteSingle(0x64, 0x1A, val);
        LP55231WriteSingle(0x64, 0x1B, 255 - val);
        LP55231WriteSingle(0x64, 0x1C, (255 + val) / 2);
        LP55231WriteSingle(0x64, 0x1D, (255 + val) / 2);
        LP55231WriteSingle(0x64, 0x1E, (255 + val) / 2); //D9
    }
    else
    {
        LP55231WriteSingle(0x64, 0x16, val); //D1
        LP55231WriteSingle(0x64, 0x17, val);
        LP55231WriteSingle(0x64, 0x18, val);
        LP55231WriteSingle(0x64, 0x19, val);
        LP55231WriteSingle(0x64, 0x1A, val);
        LP55231WriteSingle(0x64, 0x1B, val);
        LP55231WriteSingle(0x64, 0x1C, val);
        LP55231WriteSingle(0x64, 0x1D, val);
        LP55231WriteSingle(0x64, 0x1E, val); //D
    }

}


/*=============================================================================*
 * Function Name : Breathing_LED_PWM
 * Description   :
 * Argument      :
 * Return        : none
 * BY            : TGH
 *=============================================================================*/
void  Breathing_LED_PWM(void)
{
    LP55231WriteSingle(ADD1, 0x00, 0x00); //CHIP NO EN
    delay_us(200);
    LP55231WriteSingle(ADD1, 0x3D, 0xFF); ///RESTE
    delay_us(200);
    LP55231WriteSingle(0x64, 0x00, 0x40); //CHIP EN
    delay_us(200);
    LP55231WriteSingle(0x64, 0x36, 0x4D); //init
    delay_us(200);
    LP55231WriteSingle(0x64, 0x16, 0x02); //D1
    LP55231WriteSingle(0x64, 0x17, 0x02);
    LP55231WriteSingle(0x64, 0x18, 0x02);
    LP55231WriteSingle(0x64, 0x19, 0x02);
    LP55231WriteSingle(0x64, 0x1A, 0x02);
    LP55231WriteSingle(0x64, 0x1B, 0x02);
    LP55231WriteSingle(0x64, 0x1C, 0x02);
    LP55231WriteSingle(0x64, 0x1D, 0x02);
    LP55231WriteSingle(0x64, 0x1E, 0x02); //D9

}
/*=============================================================================*
 * Function Name : Breathing_LED_Blue
 * Description   :
 * Argument      :
 * Return        : none
 * BY            : TGH
 *=============================================================================*/
void  Breathing_LED_Blue(void)
{

    LP55231WriteSingle(0x64, 0x00, 0x00); //CHIP EN
    delay_us(2000);
    LP55231WriteSingle(0x64, 0x3D, 0xFF);
    delay_us(2000);

    LP55231WriteSingle(0x64, 0x00, 0x40); //CHIP EN
    delay_us(2000);
    LP55231WriteSingle(0x64, 0x36, 0x4D); //内部时钟1倍
    LP55231WriteSingle(0x64, 0x4C, 0x00); //ENGINE 1 Program start address
    LP55231WriteSingle(0x64, 0x01, LP5523_CMD_LOAD); //ENGINE 1 load program to SRAM, reset engine 1 PC
    delay_us(2000);
    LP55231WriteSingle(0x64, LP5523_REG_PROG_PAGE_SEL, 0x00); //ENGINE 1 Program memory page 0
    LP55231WriteSingle(0x64, 0x50, 0x9d);
    LP55231WriteSingle(0x64, 0x51, 0x06);
    LP55231WriteSingle(0x64, 0x52, RAMP_INC); //增
    LP55231WriteSingle(0x64, 0x53, 0xff);
    LP55231WriteSingle(0x64, 0x54, RAMP_DEC); //减
    LP55231WriteSingle(0x64, 0x55, 0xff);

    LP55231WriteSingle(0x64, 0x56, 0x9d);
    LP55231WriteSingle(0x64, 0x57, 0x02);
    LP55231WriteSingle(0x64, 0x58, RAMP_INC); //增
    LP55231WriteSingle(0x64, 0x59, 0xff);
    LP55231WriteSingle(0x64, 0x5a, RAMP_DEC); //减
    LP55231WriteSingle(0x64, 0x5b, 0xff);

    LP55231WriteSingle(0x64, 0x5c, 0x9d);
    LP55231WriteSingle(0x64, 0x5d, 0x04);
    LP55231WriteSingle(0x64, 0x5e, RAMP_INC); //增
    LP55231WriteSingle(0x64, 0x5f, 0xff);
    LP55231WriteSingle(0x64, 0x60, RAMP_DEC); //减
    LP55231WriteSingle(0x64, 0x61, 0xff);

    LP55231WriteSingle(0x64, 0x62, 0x9d);
    LP55231WriteSingle(0x64, 0x63, 0x00);
    LP55231WriteSingle(0x64, 0x64, RAMP_INC); //增
    LP55231WriteSingle(0x64, 0x65, 0xff);
    LP55231WriteSingle(0x64, 0x66, RAMP_DEC); //减
    LP55231WriteSingle(0x64, 0x67, 0xff);

    LP55231WriteSingle(0x64, 0x68, 0x9d);
    LP55231WriteSingle(0x64, 0x69, 0x00);
    LP55231WriteSingle(0x64, 0x6a, RAMP_INC); //增
    LP55231WriteSingle(0x64, 0x6b, 0xff);
    LP55231WriteSingle(0x64, 0x6c, RAMP_DEC); //减
    LP55231WriteSingle(0x64, 0x6d, 0xff);


    LP55231WriteSingle(0x64, 0x37, 0x00); //ENGINE 1 Program counter starting value
    LP55231WriteSingle(0x64, 0x00, 0x6A);
    LP55231WriteSingle(0x64, 0x01, 0x2A); //ENGINE 1 run program as defined by ENGINE1_EXEC bits.

}

/*=============================================================================*
 * Function Name : Breathing_LED_Red
 * Description   :
 * Argument      :
 * Return        : none
 * BY            : TGH
 *=============================================================================*/
void  Breathing_LED_Red(void)
{
    LP55231WriteSingle(0x64, 0x00, 0x00); //CHIP EN
    delay_us(2000);
    LP55231WriteSingle(0x64, 0x3D, 0xFF);
    delay_us(2000);

    LP55231WriteSingle(0x64, 0x00, 0x40); //CHIP EN
    delay_us(2000);
    LP55231WriteSingle(0x64, 0x36, 0x4D); //内部时钟1倍
    LP55231WriteSingle(0x64, 0x4C, 0x00); //ENGINE 1 Program start address
    LP55231WriteSingle(0x64, 0x01, LP5523_CMD_LOAD); //ENGINE 1 load program to SRAM, reset engine 1 PC
    delay_us(2000);
    LP55231WriteSingle(0x64, LP5523_REG_PROG_PAGE_SEL, 0x00); //ENGINE 1 Program memory page 0
    LP55231WriteSingle(0x64, 0x50, 0x9d);
    LP55231WriteSingle(0x64, 0x51, 0x09);
    LP55231WriteSingle(0x64, 0x52, RAMP_INC); //增
    LP55231WriteSingle(0x64, 0x53, 0xff);
    LP55231WriteSingle(0x64, 0x54, RAMP_DEC); //减
    LP55231WriteSingle(0x64, 0x55, 0xff);

    LP55231WriteSingle(0x64, 0x56, 0x9d);
    LP55231WriteSingle(0x64, 0x57, 0x07);
    LP55231WriteSingle(0x64, 0x58, RAMP_INC); //增
    LP55231WriteSingle(0x64, 0x59, 0xff);
    LP55231WriteSingle(0x64, 0x5a, RAMP_DEC); //减
    LP55231WriteSingle(0x64, 0x5b, 0xff);

    LP55231WriteSingle(0x64, 0x5c, 0x9d);
    LP55231WriteSingle(0x64, 0x5d, 0x08);
    LP55231WriteSingle(0x64, 0x5e, RAMP_INC); //增
    LP55231WriteSingle(0x64, 0x5f, 0xff);
    LP55231WriteSingle(0x64, 0x60, RAMP_DEC); //减
    LP55231WriteSingle(0x64, 0x61, 0xff);

    LP55231WriteSingle(0x64, 0x62, 0x9d);
    LP55231WriteSingle(0x64, 0x63, 0x00);
    LP55231WriteSingle(0x64, 0x64, RAMP_INC); //增
    LP55231WriteSingle(0x64, 0x65, 0xff);
    LP55231WriteSingle(0x64, 0x66, RAMP_DEC); //减
    LP55231WriteSingle(0x64, 0x67, 0xff);

    LP55231WriteSingle(0x64, 0x68, 0x9d);
    LP55231WriteSingle(0x64, 0x69, 0x00);
    LP55231WriteSingle(0x64, 0x6a, RAMP_INC); //增
    LP55231WriteSingle(0x64, 0x6b, 0xff);
    LP55231WriteSingle(0x64, 0x6c, RAMP_DEC); //减
    LP55231WriteSingle(0x64, 0x6d, 0xff);



    LP55231WriteSingle(0x64, 0x37, 0x00); //ENGINE 1 Program counter starting value
    LP55231WriteSingle(0x64, 0x00, 0x6A);
    LP55231WriteSingle(0x64, 0x01, 0x2A); //ENGINE 1 run program as defined by ENGINE1_EXEC bits.

}
/*=============================================================================*
 * Function Name : Breathing_LED_Green_1
 * Description   :
 * Argument      :
 * Return        : none
 * BY            : TGH
 *=============================================================================*/
void  Breathing_LED_Green_1(void)
{
    LP55231WriteSingle(ADD1, 0x00, 0x00); //CHIP EN

    delay_us(2000);
    LP55231WriteSingle(ADD1, 0x3D, 0xFF);

    delay_us(2000);

    LP55231WriteSingle(ADD1, 0x00, 0x40); //CHIP EN
    delay_us(2000);
    LP55231WriteSingle(ADD1, 0x36, 0x4D); //内部时钟1.5倍
    LP55231WriteSingle(ADD1, 0x4C, 0x00); //ENGINE 1 Program start address
    LP55231WriteSingle(ADD1, 0x01, LP5523_CMD_LOAD); //ENGINE 1 load program to SRAM, reset engine 1 PC
    delay_us(2000);
    LP55231WriteSingle(ADD1, LP5523_REG_PROG_PAGE_SEL, 0x00); //ENGINE 1 Program memory page 0
    LP55231WriteSingle(ADD1, 0x50, 0x9d);
    LP55231WriteSingle(ADD1, 0x51, 0x05);
    LP55231WriteSingle(ADD1, 0x52, RAMP_INC); //增
    LP55231WriteSingle(ADD1, 0x53, LUX1);
    LP55231WriteSingle(ADD1, 0x54, 0x9d);
    LP55231WriteSingle(ADD1, 0x55, 0x01);
    LP55231WriteSingle(ADD1, 0x56, RAMP_INC); //增
    LP55231WriteSingle(ADD1, 0x57, LUX1);
    LP55231WriteSingle(ADD1, 0x58, 0x9d);
    LP55231WriteSingle(ADD1, 0x59, 0x03);
    LP55231WriteSingle(ADD1, 0x5A, RAMP_INC); //增
    LP55231WriteSingle(ADD1, 0x5B, LUX1);
    LP55231WriteSingle(ADD1, 0x5C, 0x9d);
    LP55231WriteSingle(ADD1, 0x5D, 0x03);
    LP55231WriteSingle(ADD1, 0x5E, 0x7A);
    LP55231WriteSingle(ADD1, 0x5F, 0x00);
    LP55231WriteSingle(ADD1, 0x60, 0x9d);
    LP55231WriteSingle(ADD1, 0x61, 0x03);
    LP55231WriteSingle(ADD1, 0x62, 0x7A);
    LP55231WriteSingle(ADD1, 0x63, 0x00);
    LP55231WriteSingle(ADD1, 0x64, 0XC0); //END
    LP55231WriteSingle(ADD1, 0x65, 0X00);


    LP55231WriteSingle(ADD1, 0x37, 0x00); //ENGINE 1 Program counter starting value
    LP55231WriteSingle(ADD1, 0x00, 0x6A);
    LP55231WriteSingle(ADD1, 0x01, 0x2A); //ENGINE 1 run program as defined by ENGINE1_EXEC bits.


}
/*=============================================================================*
 * Function Name : Breathing_LED_Green_2
 * Description   :
 * Argument      :
 * Return        : none
 * BY            : TGH
 *=============================================================================*/
void  Breathing_LED_Green_2(void)
{
    LP55231WriteSingle(ADD1, 0x00, 0x00); //CHIP EN

    delay_us(2000);
    LP55231WriteSingle(ADD1, 0x3D, 0xFF);

    delay_us(2000);

    LP55231WriteSingle(ADD1, 0x01, 0x00); //ENGINE 1 run program as defined by ENGINE1_EXEC bits.		LP55231WriteSingle(ADD2,0x01,0x00);//ENGINE 1 run program as defined by ENGINE1_EXEC bits.

    //Logarithmic dimming control for Dn output
    //对数调光模式，如果不需要此效果，可以删除
    LP55231WriteSingle(ADD1, 0x06, 0x20); //D1
    LP55231WriteSingle(ADD1, 0x07, 0x20); //D2
    LP55231WriteSingle(ADD1, 0x08, 0x20); //D3
    LP55231WriteSingle(ADD1, 0x09, 0x20); //D4
    LP55231WriteSingle(ADD1, 0x0A, 0x20); //D5
    LP55231WriteSingle(ADD1, 0x0B, 0x20); //D6
    LP55231WriteSingle(ADD1, 0x0C, 0x20); //D7
    LP55231WriteSingle(ADD1, 0x0D, 0x20); //D8
    LP55231WriteSingle(ADD1, 0x0E, 0x20); //D9

    LP55231WriteSingle(ADD1, 0x00, 0x40); //CHIP EN
    delay_us(2000);
    LP55231WriteSingle(ADD1, 0x36, 0x4D); //内部时钟1.5倍
    LP55231WriteSingle(ADD1, 0x4C, 0x00); //ENGINE 1 Program start address
    LP55231WriteSingle(ADD1, 0x4D, 0x20); //ENGINE 1 Program start address
    LP55231WriteSingle(ADD1, 0x4E, 0x40); //ENGINE 1 Program start address

    LP55231WriteSingle(ADD1, 0x01, LP5523_CMD_LOAD); //ENGINE 1 load program to SRAM, reset engine 1 PC
    delay_us(2000);
    LP55231WriteSingle(ADD1, LP5523_REG_PROG_PAGE_SEL, 0x00); //ENGINE 1 Program memory page 0
    LP55231WriteSingle(ADD1, 0x50, 0x9d);
    LP55231WriteSingle(ADD1, 0x51, 0x05);
    LP55231WriteSingle(ADD1, 0x52, 0x40);
    LP55231WriteSingle(ADD1, 0x53, 0XFF);
    LP55231WriteSingle(ADD1, 0x54, RAMP_DEC); //减
    LP55231WriteSingle(ADD1, 0x55, LUX2);
    LP55231WriteSingle(ADD1, 0x56, RAMP_DEC); //减
    LP55231WriteSingle(ADD1, 0x57, LUX2);
    LP55231WriteSingle(ADD1, 0x58, RAMP_DEC); //减
    LP55231WriteSingle(ADD1, 0x59, LUX2);
    LP55231WriteSingle(ADD1, 0x5A, RAMP_INC); //减
    LP55231WriteSingle(ADD1, 0x5B, LUX2);
    LP55231WriteSingle(ADD1, 0x5C, RAMP_INC); //减
    LP55231WriteSingle(ADD1, 0x5D, LUX2);

    LP55231WriteSingle(ADD1, 0x01, LP5523_CMD_LOAD); //ENGINE 1 load program to SRAM, reset engine 1 PC
    delay_us(2000);
    LP55231WriteSingle(ADD1, LP5523_REG_PROG_PAGE_SEL, 0x02); //ENGINE 1 Program memory page 0
    LP55231WriteSingle(ADD1, 0x50, 0x9d);
    LP55231WriteSingle(ADD1, 0x51, 0x01);
    LP55231WriteSingle(ADD1, 0x52, 0x40); //减
    LP55231WriteSingle(ADD1, 0x53, LUX2 - 1);
    LP55231WriteSingle(ADD1, 0x54, RAMP_INC); //增
    LP55231WriteSingle(ADD1, 0x55, LUX2);
    LP55231WriteSingle(ADD1, 0x56, RAMP_DEC); //减
    LP55231WriteSingle(ADD1, 0x57, LUX2);
    LP55231WriteSingle(ADD1, 0x58, RAMP_DEC); //减
    LP55231WriteSingle(ADD1, 0x59, LUX2);
    LP55231WriteSingle(ADD1, 0x5A, RAMP_DEC); //减
    LP55231WriteSingle(ADD1, 0x5B, LUX2);
    LP55231WriteSingle(ADD1, 0x5C, RAMP_INC); //减
    LP55231WriteSingle(ADD1, 0x5D, LUX2);

    LP55231WriteSingle(ADD1, 0x01, LP5523_CMD_LOAD); //ENGINE 1 load program to SRAM, reset engine 1 PC
    delay_us(2000);
    LP55231WriteSingle(ADD1, LP5523_REG_PROG_PAGE_SEL, 0x04); //ENGINE 3 Program memory page 0
    LP55231WriteSingle(ADD1, 0x50, 0x9d);
    LP55231WriteSingle(ADD1, 0x51, 0x03);
    LP55231WriteSingle(ADD1, 0x52, 0X40); //减
    LP55231WriteSingle(ADD1, 0x53, 0);
    LP55231WriteSingle(ADD1, 0x54, RAMP_INC); //减
    LP55231WriteSingle(ADD1, 0x55, LUX2);
    LP55231WriteSingle(ADD1, 0x56, RAMP_INC); //增
    LP55231WriteSingle(ADD1, 0x57, LUX2);
    LP55231WriteSingle(ADD1, 0x58, RAMP_DEC); //减
    LP55231WriteSingle(ADD1, 0x59, LUX2);
    LP55231WriteSingle(ADD1, 0x5A, RAMP_DEC); //减
    LP55231WriteSingle(ADD1, 0x5B, LUX2);
    LP55231WriteSingle(ADD1, 0x5C, RAMP_DEC); //减
    LP55231WriteSingle(ADD1, 0x5D, LUX2);


    LP55231WriteSingle(ADD1, 0x37, 0x00); //ENGINE 1 Program counter starting value
    LP55231WriteSingle(ADD1, 0x38, 0x00); //ENGINE 1 Program counter starting value
    LP55231WriteSingle(ADD1, 0x39, 0x00); //ENGINE 1 Program counter starting value
    LP55231WriteSingle(ADD1, 0x00, 0x6A);
    LP55231WriteSingle(ADD1, 0x01, 0x2A); //ENGINE 1 run program as defined by ENGINE1_EXEC bits.
}
/*=============================================================================*
 * Function Name : Breathing_LED_Green_3
 * Description   :
 * Argument      :
 * Return        : none
 * BY            : TGH
 *=============================================================================*/
void  Breathing_LED_Green_3(void)
{
    LP55231WriteSingle(ADD1, 0x00, 0x00); //CHIP EN

    delay_us(2000);
    LP55231WriteSingle(ADD1, 0x3D, 0xFF);

    delay_us(2000);

    LP55231WriteSingle(ADD1, 0x01, 0x00); //ENGINE 1 run program as defined by ENGINE1_EXEC bits.


    //Logarithmic dimming control for Dn output
    //对数调光模式，如果不需要此效果，可以删除
    LP55231WriteSingle(ADD1, 0x06, 0x20); //D1
    LP55231WriteSingle(ADD1, 0x07, 0x20); //D2
    LP55231WriteSingle(ADD1, 0x08, 0x20); //D3
    LP55231WriteSingle(ADD1, 0x09, 0x20); //D4
    LP55231WriteSingle(ADD1, 0x0A, 0x20); //D5
    LP55231WriteSingle(ADD1, 0x0B, 0x20); //D6
    LP55231WriteSingle(ADD1, 0x0C, 0x20); //D7
    LP55231WriteSingle(ADD1, 0x0D, 0x20); //D8
    LP55231WriteSingle(ADD1, 0x0E, 0x20); //D9

    LP55231WriteSingle(ADD1, 0x00, 0x40); //CHIP EN
    delay_us(2000);
    LP55231WriteSingle(ADD1, 0x36, 0x4D); //内部时钟1.5倍
    LP55231WriteSingle(ADD1, 0x4C, 0x00); //ENGINE 1 Program start address
    LP55231WriteSingle(ADD1, 0x4D, 0x20); //ENGINE 1 Program start address
    LP55231WriteSingle(ADD1, 0x4E, 0x40); //ENGINE 1 Program start address

    LP55231WriteSingle(ADD1, 0x01, LP5523_CMD_LOAD); //ENGINE 1 load program to SRAM, reset engine 1 PC
    delay_us(2000);
    LP55231WriteSingle(ADD1, LP5523_REG_PROG_PAGE_SEL, 0x00); //ENGINE 1 Program memory page 0
    LP55231WriteSingle(ADD1, 0x50, 0x9d);
    LP55231WriteSingle(ADD1, 0x51, 0x05);
    LP55231WriteSingle(ADD1, 0x52, RAMP_INC);
    LP55231WriteSingle(ADD1, 0x53, LUX3);
    LP55231WriteSingle(ADD1, 0x54, RAMP_INC); //减
    LP55231WriteSingle(ADD1, 0x55, LUX3);
    LP55231WriteSingle(ADD1, 0x56, RAMP_INC); //减
    LP55231WriteSingle(ADD1, 0x57, LUX3);
    LP55231WriteSingle(ADD1, 0x58, 0X40); //减
    LP55231WriteSingle(ADD1, 0x59, 0X00);
    LP55231WriteSingle(ADD1, 0x5A, RAMP_DEC); //减
    LP55231WriteSingle(ADD1, 0x5B, LUX3);
    LP55231WriteSingle(ADD1, 0x5C, RAMP_DEC); //减
    LP55231WriteSingle(ADD1, 0x5D, LUX3);

    LP55231WriteSingle(ADD1, 0x01, LP5523_CMD_LOAD); //ENGINE 1 load program to SRAM, reset engine 1 PC
    delay_us(2000);
    LP55231WriteSingle(ADD1, LP5523_REG_PROG_PAGE_SEL, 0x02); //ENGINE 1 Program memory page 0
    LP55231WriteSingle(ADD1, 0x50, 0x9d);
    LP55231WriteSingle(ADD1, 0x51, 0x01);
    LP55231WriteSingle(ADD1, 0x52, 0x40); //减
    LP55231WriteSingle(ADD1, 0x53, LUX3);
    LP55231WriteSingle(ADD1, 0x54, RAMP_INC); //增
    LP55231WriteSingle(ADD1, 0x55, LUX3);
    LP55231WriteSingle(ADD1, 0x56, RAMP_INC); //减
    LP55231WriteSingle(ADD1, 0x57, LUX3);
    LP55231WriteSingle(ADD1, 0x58, 0x40); //减
    LP55231WriteSingle(ADD1, 0x59, 0);
    LP55231WriteSingle(ADD1, 0x5A, RAMP_DEC); //减
    LP55231WriteSingle(ADD1, 0x5B, LUX3);
    LP55231WriteSingle(ADD1, 0x5C, RAMP_DEC); //减
    LP55231WriteSingle(ADD1, 0x5D, LUX3);
    LP55231WriteSingle(ADD1, 0x5E, RAMP_INC); //减
    LP55231WriteSingle(ADD1, 0x5F, LUX3);

    LP55231WriteSingle(ADD1, 0x01, LP5523_CMD_LOAD); //ENGINE 1 load program to SRAM, reset engine 1 PC
    delay_us(2000);
    LP55231WriteSingle(ADD1, LP5523_REG_PROG_PAGE_SEL, 0x04); //ENGINE 3 Program memory page 0
    LP55231WriteSingle(ADD1, 0x50, 0x9d);
    LP55231WriteSingle(ADD1, 0x51, 0x03);
    LP55231WriteSingle(ADD1, 0x52, 0X40); //减
    LP55231WriteSingle(ADD1, 0x53, LUX3 * 2);
    LP55231WriteSingle(ADD1, 0x54, RAMP_INC); //减
    LP55231WriteSingle(ADD1, 0x55, LUX3);
    LP55231WriteSingle(ADD1, 0x56, 0X40); //增
    LP55231WriteSingle(ADD1, 0x57, 0);
    LP55231WriteSingle(ADD1, 0x58, RAMP_DEC); //减
    LP55231WriteSingle(ADD1, 0x59, LUX3);
    LP55231WriteSingle(ADD1, 0x5A, RAMP_DEC); //减
    LP55231WriteSingle(ADD1, 0x5B, LUX3);
    LP55231WriteSingle(ADD1, 0x5C, RAMP_INC); //减
    LP55231WriteSingle(ADD1, 0x5D, LUX3);
    LP55231WriteSingle(ADD1, 0x5E, RAMP_INC); //减
    LP55231WriteSingle(ADD1, 0x5F, LUX3);

    LP55231WriteSingle(ADD1, 0x37, 0x00); //ENGINE 1 Program counter starting value
    LP55231WriteSingle(ADD1, 0x38, 0x00); //ENGINE 1 Program counter starting value
    LP55231WriteSingle(ADD1, 0x39, 0x00); //ENGINE 1 Program counter starting value
    LP55231WriteSingle(ADD1, 0x00, 0x6A);
    LP55231WriteSingle(ADD1, 0x01, 0x2A); //ENGINE 1 run program as defined by ENGINE1_EXEC bits.

}
/*=============================================================================*
 * Function Name : Breathing_LED_Green_4
 * Description   :
 * Argument      :
 * Return        : none
 * BY            : TGH
 *=============================================================================*/
void  Breathing_LED_Green_4(void)
{
    LP55231WriteSingle(ADD1, 0x00, 0x00); //CHIP EN

    delay_us(2000);
    LP55231WriteSingle(ADD1, 0x3D, 0xFF);
    delay_us(2000);

    LP55231WriteSingle(ADD1, 0x01, 0x00); //ENGINE 1 run program as defined by ENGINE1_EXEC bits.

    //Logarithmic dimming control for Dn output
    //对数调光模式，如果不需要此效果，可以删除
    LP55231WriteSingle(ADD1, 0x06, 0x20); //D1
    LP55231WriteSingle(ADD1, 0x07, 0x20); //D2
    LP55231WriteSingle(ADD1, 0x08, 0x20); //D3
    LP55231WriteSingle(ADD1, 0x09, 0x20); //D4
    LP55231WriteSingle(ADD1, 0x0A, 0x20); //D5
    LP55231WriteSingle(ADD1, 0x0B, 0x20); //D6
    LP55231WriteSingle(ADD1, 0x0C, 0x20); //D7
    LP55231WriteSingle(ADD1, 0x0D, 0x20); //D8
    LP55231WriteSingle(ADD1, 0x0E, 0x20); //D9

    LP55231WriteSingle(ADD1, 0x00, 0x40); //CHIP EN
    delay_us(2000);
    LP55231WriteSingle(ADD1, 0x36, 0x4D); //内部时钟1.5倍
    LP55231WriteSingle(ADD1, 0x4C, 0x00); //ENGINE 1 Program start address
    LP55231WriteSingle(ADD1, 0x4D, 0x20); //ENGINE 1 Program start address
    LP55231WriteSingle(ADD1, 0x4E, 0x40); //ENGINE 1 Program start address

    LP55231WriteSingle(ADD1, 0x01, LP5523_CMD_LOAD); //ENGINE 1 load program to SRAM, reset engine 1 PC
    delay_us(2000);
    LP55231WriteSingle(ADD1, LP5523_REG_PROG_PAGE_SEL, 0x00); //ENGINE 1 Program memory page 0
    LP55231WriteSingle(ADD1, 0x50, 0x9d);
    LP55231WriteSingle(ADD1, 0x51, 0x05);
    LP55231WriteSingle(ADD1, 0x52, 0x40);
    LP55231WriteSingle(ADD1, 0x53, LUX3);
    LP55231WriteSingle(ADD1, 0x54, WAIT); //减
    LP55231WriteSingle(ADD1, 0x55, 0X00);
    LP55231WriteSingle(ADD1, 0x56, RAMP_INC); //减
    LP55231WriteSingle(ADD1, 0x57, LUX3);
    LP55231WriteSingle(ADD1, 0x58, RAMP_INC); //减
    LP55231WriteSingle(ADD1, 0x59, LUX3);
    LP55231WriteSingle(ADD1, 0x5A, RAMP_DEC); //减
    LP55231WriteSingle(ADD1, 0x5B, LUX3);
    LP55231WriteSingle(ADD1, 0x5C, RAMP_DEC); //减
    LP55231WriteSingle(ADD1, 0x5D, LUX3);

    LP55231WriteSingle(ADD1, 0x01, LP5523_CMD_LOAD); //ENGINE 1 load program to SRAM, reset engine 1 PC
    delay_us(2000);
    LP55231WriteSingle(ADD1, LP5523_REG_PROG_PAGE_SEL, 0x02); //ENGINE 1 Program memory page 0
    LP55231WriteSingle(ADD1, 0x50, 0x9d);
    LP55231WriteSingle(ADD1, 0x51, 0x01);
    LP55231WriteSingle(ADD1, 0x52, 0x40); //减
    LP55231WriteSingle(ADD1, 0x53, LUX3 * 2);
    LP55231WriteSingle(ADD1, 0x54, RAMP_DEC); //增
    LP55231WriteSingle(ADD1, 0x55, LUX3);
    LP55231WriteSingle(ADD1, 0x56, WAIT); //减
    LP55231WriteSingle(ADD1, 0x57, 0X00);
    LP55231WriteSingle(ADD1, 0x58, RAMP_INC); //减
    LP55231WriteSingle(ADD1, 0x59, LUX3);
    LP55231WriteSingle(ADD1, 0x5A, RAMP_INC); //减
    LP55231WriteSingle(ADD1, 0x5B, LUX3);
    LP55231WriteSingle(ADD1, 0x5C, RAMP_DEC); //减
    LP55231WriteSingle(ADD1, 0x5D, LUX3);
    //		LP55231WriteSingle(ADD1,0x5E,RAMP_INC);//减
    //    LP55231WriteSingle(ADD1,0x5F,LUX3);

    LP55231WriteSingle(ADD1, 0x01, LP5523_CMD_LOAD); //ENGINE 1 load program to SRAM, reset engine 1 PC
    delay_us(2000);
    LP55231WriteSingle(ADD1, LP5523_REG_PROG_PAGE_SEL, 0x04); //ENGINE 3 Program memory page 0
    LP55231WriteSingle(ADD1, 0x50, 0x9d);
    LP55231WriteSingle(ADD1, 0x51, 0x03);
    LP55231WriteSingle(ADD1, 0x52, 0X40); //减
    LP55231WriteSingle(ADD1, 0x53, LUX3 * 3);
    LP55231WriteSingle(ADD1, 0x54, RAMP_DEC); //减
    LP55231WriteSingle(ADD1, 0x55, LUX3);
    LP55231WriteSingle(ADD1, 0x56, RAMP_DEC); //增
    LP55231WriteSingle(ADD1, 0x57, LUX3);
    LP55231WriteSingle(ADD1, 0x58, WAIT); //减
    LP55231WriteSingle(ADD1, 0x59, 0X00);
    LP55231WriteSingle(ADD1, 0x5A, RAMP_INC); //减
    LP55231WriteSingle(ADD1, 0x5B, LUX3);
    LP55231WriteSingle(ADD1, 0x5C, RAMP_INC); //减
    LP55231WriteSingle(ADD1, 0x5D, LUX3);
    //		LP55231WriteSingle(ADD1,0x5E,RAMP_INC);//减
    //    LP55231WriteSingle(ADD1,0x5F,LUX3);


    LP55231WriteSingle(ADD1, 0x37, 0x00); //ENGINE 1 Program counter starting value
    LP55231WriteSingle(ADD1, 0x38, 0x00); //ENGINE 1 Program counter starting value
    LP55231WriteSingle(ADD1, 0x39, 0x00); //ENGINE 1 Program counter starting value
    LP55231WriteSingle(ADD1, 0x00, 0x6A);
    LP55231WriteSingle(ADD1, 0x01, 0x2A); //ENGINE 1 run program as defined by ENGINE1_EXEC bits.

}
