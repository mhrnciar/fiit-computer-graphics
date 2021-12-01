#include "camera_keyframe.h"

CameraKeyframe::CameraKeyframe(glm::vec3 s_pos, glm::vec3 e_pos, glm::vec3 s_look, glm::vec3 e_look, float dt) {
    startPosition = s_pos;
    endPosition = e_pos;
    startLookAt = s_look;
    endLookAt = e_look;
    maxTime = dt;
}

glm::vec3 CameraKeyframe::interpolatePosition() {
    return lerp(startPosition, endPosition, currTime / maxTime);
}

glm::vec3 CameraKeyframe::interpolateLookAt() {
    return lerp(startLookAt, endLookAt, currTime / maxTime);
}
