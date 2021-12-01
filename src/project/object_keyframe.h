#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "keyframe.h"

/*!
 *  Abstract scene object interface
 *  All objects in the scene should be able to update and render
 *  Generally we also want to keep position, rotation and scale for each object to generate a modelMatrix
 */
class ObjectKeyframe final : public Keyframe {
public:
    // Define default constructors as this is an abstract class
    ObjectKeyframe(glm::vec3 s_pos, glm::vec3 e_pos, glm::vec3 s_rot, glm::vec3 e_rot, glm::vec3 s_scl, glm::vec3 e_scl, float dt);

    // Keyframe properties
    glm::quat startRotation;
    glm::quat endRotation;
    glm::vec3 startScale{1,1,1};
    glm::vec3 endScale{1,1,1};

    glm::vec3 interpolatePosition() override;
    glm::vec3 interpolateRotation();
    glm::vec3 interpolateScale();
};

