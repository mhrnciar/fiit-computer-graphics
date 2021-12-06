#include <ppgso/image_png.h>
#include <shaders/light_vert_glsl.h>
#include <shaders/light_frag_glsl.h>

#include "whale.h"
#include "whale_back.h"
#include "whale_fin.h"
#include "src/project/scene.h"

Whale::Whale() {
    // Initialize static resources if needed
    if (!texture) texture = std::make_unique<ppgso::TextureAlpha>(ppgso::image::loadPNG("animals/whale/whale.png"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("animals/whale/head.obj");
    if (!shader) shader = std::make_unique<ppgso::Shader>(light_vert_glsl, light_frag_glsl);

    auto back = new WhaleBack();
    back->position = {0.1f, -0.2f, -4.6f};
    auto left_fin = new WhaleFin(false);
    left_fin->position = {4.5f, -2.2f, -1.5f};
    left_fin->scale = {1.1f, 1.1f, 1.1f};
    auto right_fin = new WhaleFin(true);
    right_fin->position = {-4.5f, -2.2f, -1.5f};
    right_fin->scale = {1.1f, 1.1f, 1.1f};

    addChild(back);
    addChild(left_fin);
    addChild(right_fin);
}

Whale::Whale(const std::string &mesh_file, const std::string &tex_file) {
    // Initialize static resources if needed
    if (!texture) texture = std::make_unique<ppgso::TextureAlpha>(ppgso::image::loadPNG(tex_file));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>(mesh_file);
    if (!shader) shader = std::make_unique<ppgso::Shader>(light_vert_glsl, light_frag_glsl);
}

bool Whale::update(Scene &scene, float dt) {
    auto time = (float) glfwGetTime();

    position.z += 0.01f;
    rotation.x = 0.1 * sin(time);

    generateModelMatrix();

    for (auto c : children) {
        c->update(scene, dt);
    }
    return true;
}

void Whale::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("viewPos", scene.camera->cameraPosition);
    shader->setUniform("numLights", scene.lights.size());

    shader->setUniform("dirLight.direction", scene.lightDirection);
    shader->setUniform("dirLight.ambient", scene.lightAmbient);
    shader->setUniform("dirLight.diffuse", scene.lightDiffuse);
    shader->setUniform("dirLight.specular", scene.lightSpecular);

    for (unsigned long i = 0; i < scene.lights.size(); i++) {
        shader->setUniform(setLightProperty("position", i), scene.lights[i].position);
        shader->setUniform(setLightProperty("color", i), scene.lights[i].color);
        shader->setUniform(setLightProperty("constant", i), scene.lights[i].constant);
        shader->setUniform(setLightProperty("linear", i), scene.lights[i].linear);
        shader->setUniform(setLightProperty("quadratic", i), scene.lights[i].quadratic);
        shader->setUniform(setLightProperty("ambient", i), scene.lights[i].ambient);
        shader->setUniform(setLightProperty("diffuse", i), scene.lights[i].diffuse);
        shader->setUniform(setLightProperty("specular", i), scene.lights[i].specular);
    }

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("material.diffuse", *texture);
    shader->setUniform("material.shininess", shininess);
    mesh->render();

    for(auto & i : children) {
        i->render(scene);
    }
}

void Whale::renderShadowmap(Scene &scene) {
}

void Whale::addChild(Object *s) {
    s->parent = this;
    children.push_back(s);
}
