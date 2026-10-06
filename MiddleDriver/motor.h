#ifndef __MOTOR_H
#define __MOTOR_H

void Motor_Init(void);
void Motor_Load(int PWM);
int16_t Motor_GetSpeed(void);

#endif
