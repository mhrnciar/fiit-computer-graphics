#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "keyframe.h"

/*!
 *  Abstract scene object interface
 *  All objects in the scene should be able to update and render
 *  Generally we also want to keep position, rotation and scale for each object to generate a modelMatrix
 */
class CameraKeyframe final : public Keyframe{
public:
    // Define default constructors as this is an abstract class
    CameraKeyframe(glm::vec3 s_pos, glm::vec3 e_pos, glm::vec3 s_look, glm::vec3 e_look, float dt);

    // Keyframe properties
    float currTime = 0;
    float maxTime;
    glm::vec3 startPosition{0,0,0};
    glm::vec3 endPosition{0,0,0};
    glm::vec3 startLookAt{0, 0, 0};
    glm::vec3 endLookAt{0, 0, 0};

    glm::vec3 interpolatePosition() override;
    glm::vec3 interpolateLookAt();
};

