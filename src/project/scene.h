#ifndef _PPGSO_SCENE_H
#define _PPGSO_SCENE_H

#include <memory>
#include <map>
#include <list>

#include "object.h"
#include "camera.h"

/*
 * Scene is an object that will aggregate all scene related data
 * Objects are stored in a list of objects
 * Keyboard and Mouse states are stored in a map and struct
 */
class Scene {
  public:
    /*!
     * Update all objects in the scene
     * @param time
     */
    void update(float dt);

    /*!
     * Render all objects in the scene
     */
    void render();

    /*!
     * Pick objects using a ray
     * @param position - Position in the scene to pick object from
     * @param direction - Direction to pick objects from
     * @return Objects - Vector of pointers to intersected objects
     */
    std::vector<Object*> intersect(const glm::vec3 &position, const glm::vec3 &direction);

    // Camera object
    // TODO: Add utilities to verify movement and rotation of camera - so it's easier to work with
    std::unique_ptr<Camera> camera;

    // All objects to be rendered in scene
    // TODO: Create list of objects for every group (coral reef, boids, algae forest, ship, ...)
    std::list< std::unique_ptr<Object> > objects;

    // Keyboard state
    std::map< int, int > keyboard;

    // Lights, in this case using only simple directional diffuse lighting
    glm::vec3 lightDirection{-1.0f, -1.0f, -1.0f};
    glm::vec3 lightAmbient{0.05f, 0.05f, 0.05f};
    glm::vec3 lightDiffuse{0.2f, 0.2f, 0.2f};
    glm::vec3 lightSpecular{0.3f, 0.3f, 0.3f};


    // Store cursor state
    struct {
      double x, y;
      double last_x, last_y; //records positions of x and y from the last call
      bool left, right;
    } cursor;
};

#endif // _PPGSO_SCENE_H
