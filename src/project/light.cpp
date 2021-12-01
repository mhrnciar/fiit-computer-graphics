#include <glm/glm.hpp>

#include "light.h"
#include "scene.h"

Light::Light(glm::vec3 pos, glm::vec3 col, float lin, float quad) {
    position = pos;
    color = col;
    linear = lin;
    quadratic = quad;
}

Light::Light(glm::vec3 pos, glm::vec3 col, glm::vec3 amb, glm::vec3 dif, glm::vec3 spec, float lin, float quad) {
    position = pos;
    color = col;
    ambient = amb;
    diffuse = dif;
    specular = spec;
    linear = lin;
    quadratic = quad;
}
