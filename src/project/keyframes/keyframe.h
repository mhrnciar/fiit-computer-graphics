#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

/*!
 *  Abstract scene object interface
 *  All objects in the scene should be able to update and render
 *  Generally we also want to keep position, rotation and scale for each object to generate a modelMatrix
 */
class Keyframe {
public:
    // Define default constructors as this is an abstract class
    Keyframe() = default;

    // Keyframe properties
    float currTime = 0;
    float maxTime;
    glm::vec3 startPosition{0,0,0};
    glm::vec3 endPosition{0,0,0};

    glm::vec3 lerp(const glm::vec3 &p0, const glm::vec3 &p1, float t);
    glm::quat slerp(const glm::quat &p0, const glm::quat &p1, float t);
    glm::vec3 smoothstep(glm::vec3 p0, glm::vec3 p1, float t);

    virtual glm::vec3 interpolatePosition();
    virtual glm::vec3 interpolatePositionSmoothstep();
};

