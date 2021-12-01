#include "keyframe.h"

glm::vec3 Keyframe::lerp(glm::vec3 p0, glm::vec3 p1, float t) {
    float x = (1 - t) * p0.x + t * p1.x;
    float y = (1 - t) * p0.y + t * p1.y;
    float z = (1 - t) * p0.z + t * p1.z;
    return {x, y, z};
}

glm::vec3 Keyframe::lerp(glm::quat p0, glm::quat p1, float t) {
    float x = (1 - t) * p0.x + t * p1.x;
    float y = (1 - t) * p0.y + t * p1.y;
    float z = (1 - t) * p0.z + t * p1.z;
    return {x, y, z};
}

glm::vec3 Keyframe::smoothstep(glm::vec3 p0, glm::vec3 p1, float t) {
    float x, y, z;
    x = glm::clamp((t - p0.x) / (p1.x - p0.x), 0.0f, 1.0f);
    y = glm::clamp((t - p0.y) / (p1.y - p0.y), 0.0f, 1.0f);
    z = glm::clamp((t - p0.z) / (p1.z - p0.z), 0.0f, 1.0f);
    return {x * x * (3 - 2 * x), y * y * (3 - 2 * y), z * z * (3 - 2 * z)};
}

glm::vec3 Keyframe::interpolatePosition() {
    return lerp(startPosition, endPosition, currTime / maxTime);
}
