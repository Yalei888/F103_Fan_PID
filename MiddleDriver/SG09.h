#ifndef __SG09_H
#define __SG09_H

extern volatile uint8_t KeyState;
extern volatile uint8_t OLEDState;
extern volatile int16_t act_speed,cur_speed;


void SG09_Init(void);
void SG09_Start(void);
void SG09_Stop(void);

#endif
