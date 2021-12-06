#pragma once
#include <memory>

#include <glm/glm.hpp>
#include <ppgso/ppgso.h>
#include "keyframes/camera_keyframe.h"

/*!
 * Simple camera object that keeps track of viewMatrix and projectionMatrix
 * the projectionMatrix is by default constructed as perspective projection
 * the viewMatrix is generated from up, position and back vectors on update
 */
class Camera {
public:
    glm::vec3 cameraUp{0,1,0};
    glm::vec3 cameraPosition{7.2,6.6,-6};
    glm::vec3 cameraFront{0,0,-1};
    float cameraYaw = -90.0f;
    float cameraPitch  = 0.0f;

    bool firstCamera = true;

    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    std::vector<CameraKeyframe> keyframes;

    /*!
     * Create new Camera that will generate viewMatrix and projectionMatrix based on its position, up and back vectors
     * @param fow - Field of view in degrees
     * @param ratio - Viewport screen ratio (usually width/height of the render window)
     * @param near - Distance to the near frustum plane
     * @param far - Distance to the far frustum plane
     */
    Camera(float fov = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f);

    /*!
     * Update Camera viewMatrix based on up, position and back vectors
     */
    void update(class Scene &scene, float dt);

    /*!
     * Updates camera direction based on cursor position
     */
    void updateDir(class Scene &scene);

    void initCameraAnimation();

    /*!
     * Get direction vector in world coordinates through camera projection plane
     * @param u - camera projection plane horizontal coordinate [-1,1]
     * @param v - camera projection plane vertical coordinate [-1,1]
     * @return Normalized vector from camera position to position on the camera projection plane
     */
    glm::vec3 cast(double u, double v);
};

