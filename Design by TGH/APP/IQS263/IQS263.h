#ifndef __IQS263_H
#define __IQS263_H
#include "myiic.h"   

#define IQS263_ADDR             0x44

/*********************** IQS263 REGISTERS *************************************/

#define DEVICE_INFO             0x00
#define SYS_FLAGS               0x01
#define COORDINATES             0x02
#define TOUCH_BYTES             0x03
#define COUNTS                  0x04
#define LTA                     0x05
#define DELTAS                  0x06
#define MULTIPLIERS             0x07
#define COMPENSATION            0x08
#define PROX_SETTINGS           0x09
#define THRESHOLDS              0x0A
#define TIMINGS_AND_TARGETS     0x0B
#define GESTURE_TIMERS          0x0C
#define ACTIVE_CHANNELS         0x0D


///****************************************************************//////


#define SYSTEM_FLAGS_VAL					0x00////

#define SYSTEM_EVENTS_VAL					0x00

#define MULTIPLIERS_CH0						0x08
#define MULTIPLIERS_CH1						0x08
#define MULTIPLIERS_CH2						0x08
#define MULTIPLIERS_CH3						0x08
#define BASE_VAL						      0x08
#define COMPENSATION_CH0					0x51
#define COMPENSATION_CH1					0x49
#define COMPENSATION_CH2					0x4A
#define COMPENSATION_CH3					0x49
#define PROXSETTINGS0_VAL					0x00
#define PROXSETTINGS1_VAL					0x1D//0x19
#define PROXSETTINGS2_VAL					0x40
#define PROXSETTINGS3_VAL					0x00
#define EVENT_MASK_VAL						0x03
#define PROX_THRESHOLD						  0x08
#define TOUCH_THRESHOLD_CH1					0x20
#define TOUCH_THRESHOLD_CH2					0x20
#define TOUCH_THRESHOLD_CH3					0x20
#define MOVEMENT_THRESHOLD					0x08
#define RESEED_BLOCK						    0x00
#define HALT_TIME						        0x14
#define I2C_TIMEOUT						      0x04
#define LOW_POWER						       0
#define ATI_TARGET_TOUCH					0x30
#define ATI_TARGET_PROX						0x40
#define TAP_TIMER						      0x05
#define FLICK_TIMER						    0x51
#define FLICK_THRESHOLD						0x33
#define ACTIVE_CHS						0x0F


void IQS263_Init(void); 
void i2c_event_mode_handshake(void);
void IQS263DeviceSettings(void);
uint8_t IQS263_Test(void); 
void CommsIQS_Read(unsigned char i2c_addr, unsigned char read_addr, unsigned char *data, unsigned char NoOfBytes);
void CommsIQS_Write(unsigned char i2c_addr, unsigned char write_addr, unsigned char *data, unsigned char NoOfBytes);
void handleEvents(void);


#endif















