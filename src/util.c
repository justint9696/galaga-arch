#include "inc/util.h"

float clamp(int min, float current, int max) {
    return (current < min ? min : current > max ? max : current);
}
