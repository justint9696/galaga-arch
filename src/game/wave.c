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

ewave_t Wave_Update(uint64_t tick, eformation_t formation) {
    if (!_Wave_Complete())
        return _wave.current;

    switch (_wave.current) {
        case WAVE_COMPLETE:
            break;
        default:
            _wave.tail = Enemy_SpawnWave(tick, 1, _wave.current, formation);
            break;
    }
    
    return ++_wave.current;
}
