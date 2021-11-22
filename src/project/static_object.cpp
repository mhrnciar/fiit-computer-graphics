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
        if (shader_type == DIFFUSE_SHADER) {
            shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        }
        else if (shader_type == LIGHT_SHADER) {
            shader = std::make_unique<ppgso::Shader>(light_vert_glsl, light_frag_glsl);
            lights.push_back({{0, 5, 0}, {1, 1, 1}, 10});
            lights.push_back({{1, 6, 2}, {1, 0, 0}, 20});
        }
        else if (shader_type == COLOR_SHADER) {
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
