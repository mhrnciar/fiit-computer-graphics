#include "static_object.h"
#include "scene.h"

#include <shaders/light_vert_glsl.h>
#include <shaders/light_frag_glsl.h>
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

StaticObject::StaticObject(const std::string &mesh_file, const std::string &tex_file, int shader_type) {
    // Initialize static resources if needed
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(tex_file));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>(mesh_file);

    if (!shader) {
        if (shader_type == 0)
            shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        else if (shader_type == 1) {
            shader = std::make_unique<ppgso::Shader>(light_vert_glsl, light_frag_glsl);
            lightPosition = {0.0f, 5.0f, 0.0f};
        }
        else if (shader_type == 2) {
            shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
        }
    }
}

bool StaticObject::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void StaticObject::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);
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

void StaticObject::addChild(Object *s) {
    s->parent = this;
    children.push_back(s);
}


