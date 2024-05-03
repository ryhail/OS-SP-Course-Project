#ifndef COURSE_ENTITYTYPE_H
#define COURSE_ENTITYTYPE_H

namespace EntityType {
    enum Type {
        NONE            = 0,
        ACTIVE_PLAYER   = 1 << 1,
        INACTIVE_PLAYER = 1 << 2,
        PLAYER          = ACTIVE_PLAYER | INACTIVE_PLAYER,
        BOSS            = 1 << 3,
        BULLET          = 1 << 4,
        SCENE           = 1 << 5
    };
}

#endif //COURSE_ENTITYTYPE_H
