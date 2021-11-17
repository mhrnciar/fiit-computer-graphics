#include "cube.h"
#include "scene.h"

#include <shaders/light_vert_glsl.h>
#include <shaders/light_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Cube::mesh;
std::unique_ptr<ppgso::Texture> Cube::texture;
std::unique_ptr<ppgso::Shader> Cube::shader;

Cube::Cube() {
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(light_vert_glsl, light_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("water.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("cube.obj");
    lightPosition = {0.0f, -5.0f, 0.0f};
    lightEmit = {0.2, 0.2, 0.2};
}

bool Cube::update(Scene &scene, float dt) {
    // Keyboard controls
    if(scene.keyboard[GLFW_KEY_LEFT]) {
        position.x += 10 * dt;
    } else if(scene.keyboard[GLFW_KEY_RIGHT]) {
        position.x -= 10 * dt;
    } else if(scene.keyboard[GLFW_KEY_UP]) {
        position.y += 10 * dt;
    } else if(scene.keyboard[GLFW_KEY_DOWN]) {
        position.y -= 10 * dt;
    } else if(scene.keyboard[GLFW_KEY_LEFT_SHIFT]) {
        position.z += 10 * dt;
    } else if(scene.keyboard[GLFW_KEY_LEFT_CONTROL]) {
        position.z -= 10 * dt;
    } else {
        rotation.x += dt*ppgso::PI;
    }

    generateModelMatrix();
    return true;
}

void Cube::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightPosition", lightPosition);
    shader->setUniform("LightEmit", lightEmit);
    shader->setUniform("LightColor", lightColor);
    shader->setUniform("LightPower", lightPower);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);
    mesh->render();

    for(auto & i : children) {
        i->render(scene);
    }
}

void Cube::addChild(Object *s) {
    s->parent = this;
    children.push_back(s);
}


