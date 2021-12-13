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

    ObjectKeyframe(glm::vec3 s_pos, glm::vec3 e_pos, glm::vec3 s_rot, glm::vec3 e_rot, float dt);

    // Keyframe properties
    glm::quat startRotation;
    glm::quat endRotation;
    glm::vec3 startScale;
    glm::vec3 endScale;

    glm::mat4 startModelMatrix{1.0f};
    glm::mat4 endModelMatrix{1.0f};
    glm::mat4 startTranslationMatrix{1.0f};
    glm::mat4 endTranslationMatrix{1.0f};
    glm::mat4 startRotationMatrix{1.0f};
    glm::mat4 endRotationMatrix{1.0f};
    glm::mat4 startScaleMatrix{1.0f};
    glm::mat4 endScaleMatrix{1.0f};

    /*!
     * Interpolate position using lerp
     * @return  interpolated position
     */
    glm::vec3 interpolatePosition() override;

    /*!
     * Interpolate position using smoothstep
     * @return  interpolated position
     */
    glm::vec3 interpolatePositionSmoothstep() override;

    /*!
     * Interpolate rotation using slerp
     * @return  interpolated rotation
     */
    glm::vec3 interpolateRotation();

    /*!
     * Interpolate scale using lerp
     * @return  interpolated scale
     */
    glm::vec3 interpolateScale();

    /*!
     * Interpolate model matrix using glm::interpolate
     * @return  interpolated model matrix
     */
    glm::mat4 interpolateModelMatrix();
};

