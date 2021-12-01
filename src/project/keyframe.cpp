//
// Created by Matejo on 1. 12. 2021.
//

#include "keyframe.h"

Keyframe::Keyframe(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl, float dt) {
    position = pos;
    rotation = glm::quat(rot);
    scale = scl;
    maxTime = dt;
}