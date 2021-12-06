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

void Camera::initCameraAnimation() {
    keyframes.push_back({{-74.0f, 112.0f, -42.0f}, {-74.0f, 112.0f, -42.0f}, {-0.5f, 0, -0.5f}, {-0.5f, 0, -0.5f}, 3});
    keyframes.push_back({{-74.0f, 112.0f, -42.0f}, {-74.0f, 100.0f, -42.0f}, {-0.5f, 0, -0.5f}, {-0.5f, 0.75f, -0.5f}, 5});
    keyframes.push_back({{-74.0f, 100.0f, -42.0f}, {-46.0f, 60.0f, -20.0f}, {-0.5f, 0.75f, -0.5f}, {-0.5f, 0.66f, -0.5f}, 10});
    keyframes.push_back({{-46.0f, 60.0f, -20.0f}, {-46.0f, 60.0f, -20.0f}, {-0.5f, 0.66f, -0.5f}, {-0.25f, 0.5f, -0.75f}, 4});
    keyframes.push_back({{-46.0f, 60.0f, -20.0f}, {-46.0f, 60.0f, -20.0f}, {-0.25f, 0.5f, -0.75f}, {-0.5f, 0.5f, -0.5f}, 3});
    keyframes.push_back({{-46.0f, 60.0f, -20.0f}, {-46.0f, 60.0f, -20.0f}, {-0.5f, 0.5f, -0.5f}, {-0.75f, 0.5f, -0.25f}, 3});
    keyframes.push_back({{-46.0f, 60.0f, -20.0f}, {-33.0f, 33.0f, -4.0f}, {-0.75f, 0.5f, -0.25f}, {-0.9f, 0.2f, -0.1f}, 6});
    keyframes.push_back({{-33.0f, 33.0f, -4.0f}, {-25.0f, 10.0f, -5.0f}, {-0.9f, 0.2f, -0.1f}, {-0.9f, 0, 0.1f}, 6});
    keyframes.push_back({{-25.0f, 10.0f, -5.0f}, {-16.0f, 7.0f, -6.0f}, {-0.9f, 0, 0.1f}, {-1.0f, 0.0f, 0.0f}, 6});
    keyframes.push_back({{-16.0f, 7.0f, -6.0f}, {-12.0f, 7.0f, -5.0f}, {-1.0f, 0.0f, 0.0f}, {-0.8f, 0.0f, 0.2f}, 3});
    keyframes.push_back({{-12.0f, 7.0f, -5.0f}, {-10.0f, 7.0f, -6.0f}, {-0.8f, 0.0f, 0.2f}, {-0.8f, 0.0f, -0.2f}, 3});
    keyframes.push_back({{-10.0f, 7.0f, -6.0f}, {-3.0f, 7.0f, -6.5f}, {-0.8f, 0.0f, -0.2f}, {-1.0f, 0.0f, 0.0f}, 3});
    keyframes.push_back({{-3.0f, 7.0f, -6.5f}, {-3.0f, 7.0f, -6.5f}, {-1.0f, 0.0f, 0.0f}, {-0.5f, 0.0f, -0.5f}, 3});
    keyframes.push_back({{-3.0f, 7.0f, -6.5f}, {-2.5f, 7.0f, -8.0f}, {-0.5f, 0.0f, -0.5f}, {-0.2f, 0.0f, -0.8f}, 3});
    keyframes.push_back({{-2.5f, 7.0f, -8.0f}, {1.5f, 7.5f, -10.0f}, {-0.2f, 0.0f, -0.8f}, {0.4f, 0.0f, -0.6f}, 3});
    keyframes.push_back({{1.5f, 7.5f, -10.0f}, {1.5f, 7.5f, -10.0f}, {0.4f, 0.0f, -0.6f}, {-0.3f, 0.0f, -0.7f}, 3});
    keyframes.push_back({{1.5f, 7.5f, -10.0f}, {1.5f, 7.5f, -10.0f}, {-0.3f, 0.0f, -0.7f}, {-0.5, 0.0f, -0.5f}, 3});
    keyframes.push_back({{1.5f, 7.5f, -10.0f}, {1.5f, 7.5f, -10.0f}, {-0.5, 0.0f, -0.5f}, {-0.8f, 0.0f, -0.2f}, 3});
    keyframes.push_back({{1.5f, 7.5f, -10.0f}, {45.0f, 6.0f, -8.0f}, {-0.8, 0.0f, -0.2f}, {-0.9f, 0.0f, -0.1f}, 15});
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
