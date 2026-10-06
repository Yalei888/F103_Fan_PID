#ifndef __MENU_H
#define __MENU_H

extern volatile int Level;
extern volatile int isRound;

void First_Set(void);
int Level_Set(void);
int Round_Set(void);
int PID_Set(int SetFlag);

#endif
