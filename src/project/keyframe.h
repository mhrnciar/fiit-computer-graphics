#pragma once
#include <memory>
#include <list>
#include <map>

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
    Keyframe(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl, float dt);

    // Keyframe properties
    float currTime = 0;
    float maxTime;
    glm::vec3 position{0,0,0};
    glm::quat rotation;
    glm::vec3 scale{1,1,1};
};

