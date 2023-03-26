#include "inc/util.h"

int clamp(int min, int current, int max) {
    return (current < min ? min : current > max ? max : current);
}
