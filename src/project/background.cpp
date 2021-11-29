#include "background.h"
#include "scene.h"

#include <ppgso/image_png.h>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

Background::Background(const std::string &mesh_file, const std::string &tex_file) {
    // Initialize static resources if needed
    if (!texture) texture = std::make_unique<ppgso::TextureAlpha>(ppgso::image::loadPNG(tex_file));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>(mesh_file);
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
}

bool Background::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void Background::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("numLights", lights.size());

    for (unsigned long i = 0; i < lights.size(); i++) {
        shader->setUniform(setLightUniform("position", i), lights[i].position);
        shader->setUniform(setLightUniform("color", i), lights[i].color);
        shader->setUniform(setLightUniform("power", i), lights[i].power);
        shader->setUniform(setLightUniform("ambient", i), lights[i].ambient);
        shader->setUniform(setLightUniform("specular", i), lights[i].specular);
    }

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("material.diffuse", *texture);
    mesh->render();

    for(auto & i : children) {
        i->render(scene);
    }
}

void Background::addChild(Object *s) {
    s->parent = this;
    children.push_back(s);
}