#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>

uint64_t Get_Tick();

void Time_Init();
uint64_t Time_Passed(uint64_t tick);
uint64_t Time_Ticks();

#endif
