#include "game/wave.h"

static wave_s _wave;

static inline bool _Wave_Complete() {
    if (!_wave.tail || !_wave.tail->path.size)
        return true;

    path_s *path = (path_s *)queue_rear(&_wave.tail->path);
    return _wave.current && _wave.tail->path.size == 1 && path->complete;
}

inline void Wave_Init() {
    _wave.tail = NULL;
    _wave.current = WAVE_ONE;
}

void Wave_Update(uint64_t tick, eformation_t formation) {
    if (!_Wave_Complete())
        return;

    switch (_wave.current) {
        case WAVE_ONE:
            _wave.tail = Enemy_SpawnWave(tick, 1, _wave.current++, formation);
            break;
        case WAVE_TWO:
            _wave.tail = Enemy_SpawnWave(tick, 1, _wave.current++, formation);
            break;
        case WAVE_THREE:
            break;
        case WAVE_FOUR:
            break;
        default:
            break;
    }
}
