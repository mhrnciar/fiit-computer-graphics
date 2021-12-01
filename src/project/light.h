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

    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;

    glm::vec3 ambient{0.05f, 0.05f, 0.05f};
    glm::vec3 diffuse{0.8f, 0.8f, 0.8f};
    glm::vec3 specular{1.0f, 1.0f, 1.0f};

    /*!
     * Create new light with variable position and color, attenuation is set to distance 50
     * @param pos   position of the light
     * @param col   color in RGB format
     */
    Light(glm::vec3 pos, glm::vec3 col) : position(pos), color(col) {}

    /*!
     * Create new light with static ambient, diffuse and specular parts and variable position, color, linear
     * and quadratic power of the light. Constant part is always = 1.0f, linear and quadratic can be fine-tuned,
     * or can be taken from table in: https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
     * @param pos   position of the light
     * @param col   color in RGB format
     * @param lin   linear attenuation
     * @param quad  quadratic attenuation
     */
    Light(glm::vec3 pos, glm::vec3 col, float lin, float quad);

    /*!
     * Create new light with variable position, color, ambient, diffuse and specular values, linear and quadratic
     * attenuation. Constant attenuation is always = 1.0f, linear and quadratic can be fine-tuned, or can be taken
     * from table in: https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
     * @param pos   position of the light
     * @param col   color in RGB format
     * @param amb   ambient lighting
     * @param dif   diffuse lighting
     * @param spec  specular lighting
     * @param lin   linear attenuation
     * @param quad  quadratic attenuation
     */
    Light(glm::vec3 pos, glm::vec3 col, glm::vec3 amb, glm::vec3 dif, glm::vec3 spec, float lin, float quad);
};

