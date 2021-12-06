#pragma once
#include <memory>
#include <list>
#include <map>

#include <glm/glm.hpp>
#include "light.h"
#include "keyframes/object_keyframe.h"

// Forward declare a scene
class Scene;

/*!
 *  Abstract scene object interface
 *  All objects in the scene should be able to update and render
 *  Generally we also want to keep position, rotation and scale for each object to generate a modelMatrix
 */
class Object {
public:
    // Define default constructors as this is an abstract class
    Object() = default;
    Object(const Object&) = default;
    Object(Object&&) = default;

    virtual ~Object() {
        for(auto &it : children)
            delete it;
        children.clear();
    }

    /*!
     * Update Object parameters, usually used to update the modelMatrix based on position, scale and rotation
     *
     * @param scene - Reference to the Scene the object is rendered in
     * @param dt - Time delta for animation purposes
     * @return true to delete the object
     */
    virtual bool update(Scene &scene, float dt) = 0;

    /*!
     * Render the object in the scene
     * @param scene
     */
    virtual void render(Scene &scene) = 0;

    virtual void renderShadowmap(Scene &scene) = 0;


    /*!
     * Event to be called when the object is clicked
     * @param scene
     */
    virtual void onClick(Scene &scene) {};

    /*!
     * Add child object creating hierarchy
     * @param s - child object
     */
    virtual void addChild(Object *s) {};

    std::string setLightProperty(const char* propertyName, size_t lightIndex);

    // Object properties
    glm::vec3 position{0,0,0};
    glm::vec3 rotation{0,0,0};
    glm::vec3 scale{1,1,1};

    glm::mat4 modelMatrix{1.0f};
    glm::mat4 translationMatrix{1.0f};
    glm::mat4 rotationMatrix{1.0f};
    glm::mat4 scaleMatrix{1.0f};

    float shininess = 32.0f;
    Object *parent = nullptr;
    std::vector<ObjectKeyframe> keyframes;

protected:
    /*!
     * Generate modelMatrix from position, rotation and scale
     */
    void generateModelMatrix();

    std::list<Object *> children;
};

