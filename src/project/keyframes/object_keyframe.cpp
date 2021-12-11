#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/matrix_interpolation.hpp>

#include "object_keyframe.h"

ObjectKeyframe::ObjectKeyframe(glm::vec3 s_pos, glm::vec3 e_pos, glm::vec3 s_rot, glm::vec3 e_rot,
                   glm::vec3 s_scl, glm::vec3 e_scl, float dt) {
    maxTime = dt;
    startTranslationMatrix = glm::translate(glm::mat4(1.0f), s_pos);
    endTranslationMatrix = glm::translate(glm::mat4(1.0f), e_pos);
    startRotationMatrix = glm::orientate4(s_rot);
    endRotationMatrix = glm::orientate4(e_rot);
    startScaleMatrix = glm::scale(glm::mat4(1.0f), s_scl);
    endScaleMatrix = glm::scale(glm::mat4(1.0f), e_scl);

    startModelMatrix = startTranslationMatrix * startRotationMatrix * startScaleMatrix;
    endModelMatrix = endTranslationMatrix * endRotationMatrix * endScaleMatrix;
}

ObjectKeyframe::ObjectKeyframe(glm::vec3 s_pos, glm::vec3 e_pos, glm::vec3 s_rot, glm::vec3 e_rot, float dt) {
    maxTime = dt;
    startTranslationMatrix = glm::translate(glm::mat4(1.0f), s_pos);
    endTranslationMatrix = glm::translate(glm::mat4(1.0f), e_pos);
    startRotationMatrix = glm::orientate4(s_rot);
    endRotationMatrix = glm::orientate4(e_rot);
    startScaleMatrix = glm::scale(glm::mat4(1.0f), {1, 1, 1});
    endScaleMatrix = glm::scale(glm::mat4(1.0f), {1, 1, 1});

    startModelMatrix = startTranslationMatrix * startRotationMatrix * startScaleMatrix;
    endModelMatrix = endTranslationMatrix * endRotationMatrix * endScaleMatrix;
}

glm::vec3 ObjectKeyframe::interpolatePosition() {
    return lerp(startPosition, endPosition, currTime / maxTime);
}

glm::vec3 ObjectKeyframe::interpolatePositionSmoothstep() {
    return smoothstep(startPosition, endPosition, currTime);
}

glm::vec3 ObjectKeyframe::interpolateRotation() {
    return glm::eulerAngles(slerp(startRotation, endRotation, currTime / maxTime));
}

glm::vec3 ObjectKeyframe::interpolateScale() {
    return lerp(startScale, endScale, currTime / maxTime);
}

glm::mat4 ObjectKeyframe::interpolateModelMatrix() {
    return glm::interpolate(startModelMatrix, endModelMatrix, currTime / maxTime);
}
