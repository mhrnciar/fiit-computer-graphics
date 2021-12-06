#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <sstream>

#include "object.h"

void Object::generateModelMatrix() {
    if(parent) { //This node has a parent...
        translationMatrix = parent->translationMatrix * glm::translate(glm::mat4(1.0f), position);
        rotationMatrix = parent->rotationMatrix * glm::orientate4(rotation);
        scaleMatrix = parent->scaleMatrix * glm::scale(glm::mat4(1.0f), scale);
        modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
    }
    else{ //Root node, world transform is local transform!
        translationMatrix = glm::translate(glm::mat4(1.0f), position);
        rotationMatrix = glm::orientate4(rotation);
        scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
        modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
    }
}

std::string Object::setLightProperty(const char* propertyName, size_t lightIndex) {
    std::ostringstream ss;
    ss << "pointLights[" << lightIndex << "]." << propertyName;
    return ss.str();
}

glm::vec3 Object::getRealPosition() {
	if (this->parent != NULL){
		return (this->parent->getRealPosition() + this->position);
	}
	else {
		return this->position;
	}
}

Object* Object::getRootParent() {
	if (this->parent != NULL){
		return (this->parent->getRootParent());
	}
	else{
		return this;
	}
}
