#pragma once
#include <memory>

#include <glm/glm.hpp>
#include <ppgso/ppgso.h>

/*!
 * Simple light object that keeps track of light sources along with their positions, colors and
 * individual power (luminosity)
 */
class Light {
public:
    glm::vec3 position{0, 0, 0};
    glm::vec3 color{1, 1, 1};
    float power = 10;

    glm::vec3 ambient{0.1f, 0.1f, 0.1f};
    glm::vec3 specular{0.3f, 0.3f, 0.3f};


    /*!
     * Create new light source with defined position, power and color
     * @param pos - Position of the light source
     * @param color - RGB color of the light
     * @param power - Power of the light
     */
    Light(glm::vec3 pos, glm::vec3 col, float pow) : position(pos), color(col), power(pow) {}

    /*!
     * Update light
     */
    void update(class Scene &scene, float dt);
};

