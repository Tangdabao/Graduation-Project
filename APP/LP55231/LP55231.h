#ifndef __LP55231_H
#define __LP55231_H
#include "sys.h"

#define LP5523_REG_ENABLE               0x00
#define LP5523_REG_OP_MODE              0x01
#define LP5523_REG_RATIOMETRIC_MSB      0x02
#define LP5523_REG_RATIOMETRIC_LSB      0x03
#define LP5523_REG_ENABLE_LEDS_MSB      0x04
#define LP5523_REG_ENABLE_LEDS_LSB      0x05
#define LP5523_REG_LED_CNTRL_BASE       0x06
#define LP5523_REG_LED_PWM_BASE         0x16
#define LP5523_REG_LED_CURRENT_BASE     0x26
#define LP5523_REG_CONFIG               0x36
#define LP5523_REG_CHANNEL1_PC          0x37
#define LP5523_REG_CHANNEL2_PC          0x38
#define LP5523_REG_CHANNEL3_PC          0x39
#define LP5523_REG_STATUS               0x3a
#define LP5523_REG_GPO                  0x3b
#define LP5523_REG_VARIABLE             0x3c
#define LP5523_REG_RESET                0x3d
#define LP5523_REG_TEMP_CTRL            0x3e
#define LP5523_REG_TEMP_READ            0x3f
#define LP5523_REG_TEMP_WRITE           0x40
#define LP5523_REG_LED_TEST_CTRL        0x41
#define LP5523_REG_LED_TEST_ADC         0x42
#define LP5523_REG_ENG1_VARIABLE        0x45
#define LP5523_REG_ENG2_VARIABLE        0x46
#define LP5523_REG_ENG3_VARIABLE        0x47
#define LP5523_REG_MASTER_FADER1        0x48
#define LP5523_REG_MASTER_FADER2        0x49
#define LP5523_REG_MASTER_FADER3        0x4a
#define LP5523_REG_CH1_PROG_START       0x4c
#define LP5523_REG_CH2_PROG_START       0x4d
#define LP5523_REG_CH3_PROG_START       0x4e
#define LP5523_REG_PROG_PAGE_SEL        0x4f
#define LP5523_REG_PROG_MEM             0x50
#define LP5523_CMD_LOAD                 0x15 /* 00010101 */
#define LP5523_CMD_RUN                  0x2a /* 00101010 */
#define LP5523_CMD_DISABLED             0x00 /* 00000000 */
#define LP5523_ENABLE                   0x40
#define LP5523_AUTO_INC                 0x40
#define LP5523_PWR_SAVE                 0x20
#define LP5523_PWM_PWR_SAVE             0x04
#define LP5523_CP_1                     0x08
#define LP5523_CP_1_5                   0x10
#define LP5523_CP_AUTO                  0x18
#define LP5523_INT_CLK                  0x01
#define LP5523_AUTO_CLK                 0x02
#define LP5523_EN_LEDTEST               0x80
#define LP5523_LEDTEST_DONE             0x80

#define  RamLen           0x18 
#define  LPENPin          GPIO_Pin_5////EN >PA5
#define  LPENPort         GPIOA
#define  SCL2             GPIO_Pin_3////SCL>>>PA3
#define  SDA2             GPIO_Pin_4////SDA>>>PA4
#define  I2c2Port    	    GPIOA

#define  SDA2High()  	    GPIO_SetBits(I2c2Port,SDA2)
#define  SDA2Low()   	    GPIO_ResetBits(I2c2Port,SDA2)

#define  SCL2High()  	    GPIO_SetBits(I2c2Port,SCL2)
#define  SCL2Low()   	    GPIO_ResetBits(I2c2Port,SCL2)
                          
#define  GetSDA2()        GPIO_ReadInputDataBit(I2c2Port,SDA2)

#define  STEP					    0X01	//亮的单步时间	 
#define  RAMP_INC					STEP*2
#define  RAMP_DEC					STEP*2+1
#define	 ADD1							0x64	//LP地址1

#define  LUX1							0x03	//过程1的亮度
#define  LUX2							128	//过程2的亮度
#define  LUX3							76	//过程2的亮度
#define  WAIT							STEP*2*LUX3/32+64


void LPP55231_CLOSE(void);
void LP55_I2c2Init(void);
void Read_temperature(void);
uint8_t LP55231_Test(void);
void Breathing_LED_Green_1(void);
void Breathing_LED_PWM(void);
void Breathing_LED_PWM_1(unsigned char val);
void  Breathing_LED_Blue(void);
void  Breathing_LED_Red(void);
void  Breathing_LED_Green_1(void);
void  Breathing_LED_Green_2(void);
void  Breathing_LED_Green_3(void);
void  Breathing_LED_Green_4(void);

#endif

