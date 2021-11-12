#include "rectangle.h"
#include "scene.h"

#include <shaders/light_vert_glsl.h>
#include <shaders/light_frag_glsl.h>

// shared resources
std::unique_ptr<ppgso::Mesh> Rectangle::mesh;
std::unique_ptr<ppgso::Texture> Rectangle::texture;
std::unique_ptr<ppgso::Shader> Rectangle::shader;

Rectangle::Rectangle() {
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(light_vert_glsl, light_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("sand.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("plane.obj");
}

bool Rectangle::update(Scene &scene, float dt) {
    // Keyboard controls
    if(scene.keyboard[GLFW_KEY_LEFT]) {
        position.x += 10 * dt;
    } else if(scene.keyboard[GLFW_KEY_RIGHT]) {
        position.x -= 10 * dt;
    } else if(scene.keyboard[GLFW_KEY_UP]) {
        position.y += 10 * dt;
    } else if(scene.keyboard[GLFW_KEY_DOWN]) {
        position.y -= 10 * dt;
    } else {
        rotation.z = ppgso::PI;
        rotation.x = ppgso::PI/4;
        rotation.y += 2 * dt;
    }

    generateModelMatrix();
    return true;
}

void Rectangle::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightPosition", scene.lightPosition);

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

void Rectangle::addChild(Object *s) {
    s->parent = this;
    children.push_back(s);
}

