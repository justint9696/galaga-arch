#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>

void Time_Init();
uint64_t Time_Ticks();
uint64_t Time_GetTick();
uint64_t Time_Passed(uint64_t tick);

#endif
