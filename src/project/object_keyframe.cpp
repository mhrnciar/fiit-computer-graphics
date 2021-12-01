#include "object_keyframe.h"

ObjectKeyframe::ObjectKeyframe(glm::vec3 s_pos, glm::vec3 e_pos, glm::vec3 s_rot, glm::vec3 e_rot,
                   glm::vec3 s_scl, glm::vec3 e_scl, float dt) {
    startPosition = s_pos;
    endPosition = e_pos;
    startRotation = glm::quat(s_rot);
    endRotation = glm::quat(e_rot);
    startScale = s_scl;
    endScale = e_scl;
    maxTime = dt;
}

glm::vec3 ObjectKeyframe::interpolatePosition() {
    return lerp(startPosition, endPosition, currTime / maxTime);
}

glm::vec3 ObjectKeyframe::interpolateRotation() {
    return lerp(startRotation, endRotation, currTime / maxTime);
}

glm::vec3 ObjectKeyframe::interpolateScale() {
    return lerp(startScale, endScale, currTime / maxTime);
}
