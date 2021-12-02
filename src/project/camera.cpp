#include <glm/glm.hpp>
#include <math.h>

#include "camera.h"
#include "scene.h"



Camera::Camera(float fov, float ratio, float near, float far) {
  float fovInRad = (ppgso::PI/180.0f) * fov;

  projectionMatrix = glm::perspective(fovInRad, ratio, near, far);
}

void Camera::update(Scene &scene, float dt) {
    //printf("%lf %lf %lf\n", cameraPosition.x, cameraPosition.y, cameraPosition.z);
    if (keyframes.empty()) {
        float cameraSpeed = 5 * dt;
        if(scene.keyboard[GLFW_KEY_W]) {
            cameraPosition -= cameraSpeed * cameraFront;
        } else if(scene.keyboard[GLFW_KEY_S]) {
            cameraPosition += cameraSpeed * cameraFront;
        } else if(scene.keyboard[GLFW_KEY_A]) {
            cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        } else if(scene.keyboard[GLFW_KEY_D]) {
            cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
        viewMatrix = lookAt(cameraPosition, cameraPosition-cameraFront, cameraUp);
    }
    else {
        static int count = 0;
        cameraPosition = keyframes[count].interpolatePosition();
        cameraFront = keyframes[count].interpolateLookAt();
        viewMatrix = lookAt(cameraPosition, cameraPosition-cameraFront, cameraUp);

        keyframes[count].currTime += dt;
        if (keyframes[count].currTime > keyframes[count].maxTime) {
            keyframes[count].currTime = 0;
            count++;
            if (count >= keyframes.size())
                count = 0;
        }
    }
}

void Camera::updateDir(Scene &scene){
	float xoffset, yoffset;
	if (firstCamera) {
		xoffset = 0;
		yoffset = 0;
		scene.cursor.last_x = scene.cursor.x;
		scene.cursor.last_y = scene.cursor.y;
		firstCamera = false;
	}
	else{
		xoffset = scene.cursor.x - scene.cursor.last_x;
		yoffset = scene.cursor.y - scene.cursor.last_y;
	}
	
	scene.cursor.last_x = scene.cursor.x;
	scene.cursor.last_y = scene.cursor.y;
	
	float cameraSensitivity = 0.1f;
	xoffset *= cameraSensitivity;
	yoffset *= cameraSensitivity;
	
	cameraYaw += xoffset;
	cameraPitch += yoffset;
	if (cameraPitch >89.0f){
		cameraPitch = 89.0f;
	}
	if (cameraPitch < -89.0f){
		cameraPitch = -89.0f;
	}
	
	glm::vec3 front;
	front.x = cos(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
	front.y = sin(glm::radians(cameraPitch));
	front.z = sin(glm::radians(cameraYaw)) * cos(glm::radians(cameraPitch));
	cameraFront = glm::normalize(front);
}

glm::vec3 Camera::cast(double u, double v) {
  // Create point in Screen coordinates
  glm::vec4 screenPosition{u,v,0.0f,1.0f};

  // Use inverse matrices to get the point in world coordinates
  auto invProjection = glm::inverse(projectionMatrix);
  auto invView = glm::inverse(viewMatrix);

  // Compute position on the camera plane
  auto planePosition = invView * invProjection * screenPosition;
  planePosition /= planePosition.w;

  // Create direction vector
  auto direction = glm::normalize(planePosition - glm::vec4{cameraPosition,1.0f});
  return glm::vec3{direction};
}
