#ifndef __PID_H
#define __PID_H

#define MAX_SPEED		84
#define LEVEL1_SPEED	40
#define LEVEL2_SPEED	55
#define LEVEL3_SPEED	75
#define MAX_CHANGE_I	600

extern volatile int Kp,Ki,Kd;

void PID_Control(int tar_speed,int cur_speed);

#endif
