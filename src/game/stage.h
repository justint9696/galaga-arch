#ifndef _STAGE_H_
#define _STAGE_H_

#include <stdbool.h>
#include <stdint.h>

#define MAX_ENEMY       1

typedef enum {
    FORMATION_SINGLE,
    FORMATION_DOUBLE,
} formation_t;

typedef struct {
    uint32_t id;
    float scalar;
    formation_t formation;
    struct {
        uint32_t total, remaining;
    } enemies;
} stage_s;

void Stage_Init();
void Stage_Set();
void Stage_Clear();
uint32_t Stage_Next();
bool Stage_Complete();

uint32_t Stage_Current();
uint32_t Stage_Remaining();

float Stage_Scalar();
formation_t Stage_Formation();

#endif
