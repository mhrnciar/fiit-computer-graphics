#include "cube.h"
#include "scene.h"

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Cube::mesh;
std::unique_ptr<ppgso::Texture> Cube::texture;
std::unique_ptr<ppgso::Shader> Cube::shader;

Cube::Cube(const glm::vec3 &color) {
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("cube.obj");
    position = {0, 0, 0};
    this->color = color;
}

bool Cube::update(Scene &scene, float dt) {
    // Keyboard controls
    generateModelMatrix();
    return true;
}

void Cube::render(Scene &scene) {
    shader->use();

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("OverallColor", color);
    mesh->render();

    for(auto & i : children) {
        i->render(scene);
    }
}

void Cube::renderShadowmap(Scene &scene) {
}

void Cube::addChild(Object *s) {
    s->parent = this;
    children.push_back(s);
}


