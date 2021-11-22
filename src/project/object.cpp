#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <sstream>

#include "object.h"

void Object::generateModelMatrix() {
    glm::mat4 translationMatrix, rotationMatrix, scaleMatrix;

    if(parent) { //This node has a parent...
        translationMatrix = glm::translate(glm::mat4(1.0f), position);
        rotationMatrix = glm::orientate4(rotation);
        scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
        modelMatrix = translationMatrix * rotationMatrix * scaleMatrix * parent->modelMatrix;
    }

    else{ //Root node, world transform is local transform!
        translationMatrix = glm::translate(glm::mat4(1.0f), position);
        rotationMatrix = glm::orientate4(rotation);
        scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
        modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
    }

    for(auto & i : children) {
        i->generateModelMatrix();
    }
}

std::string Object::setLightUniform(const char* propertyName, size_t lightIndex) {
    std::ostringstream ss;
    ss << "pointLights[" << lightIndex << "]." << propertyName;
    return ss.str();
}
