#include "static_object.h"
#include "scene.h"

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <shaders/light_vert_glsl.h>
#include <shaders/light_frag_glsl.h>

StaticObject::StaticObject(const std::string &mesh_file, const std::string &tex_file, int shader_type) {
    // Initialize static resources if needed
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(tex_file));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>(mesh_file);

    if (!shader) {
        if (shader_type == COLOR_SHADER) {
            shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
        }
        else if (shader_type == TEXTURE_SHADER) {
            shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
        }
        else if (shader_type == DIFFUSE_SHADER) {
            shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
        }
        else if (shader_type == LIGHT_SHADER) {
            shader = std::make_unique<ppgso::Shader>(light_vert_glsl, light_frag_glsl);
            lights.push_back({{-4.78f, 6.0f, -7.5f}, {0.0f, 0.0f, 1.0f}, 0.7, 1.8});
            lights.push_back({{-7.87f, 5.2f, -7.0f}, {0.6f, 0.0f, 1.0f}, 0.7, 1.8});
            lights.push_back({{-6.06f, 8.5f, -4.85f}, {1.0f, 0.0f, 0.0f}, 0.7, 1.8});
            lights.push_back({{-6.35f, 8.15f, -7.26f}, {1.0f, 0.0f, 0.0f}, 0.7, 1.8});
            lights.push_back({{-4.85f, 5.2f, -4.47f}, {0.9f, 0.8f, 0.0f}, 0.7, 1.8});
            lights.push_back({{-7.09f, 5.0f, -4.1f}, {0.0f, 1.0f, 0.0f}, 0.7, 1.8});
            lights.push_back({{-1.9f, 5.2f, -3.62f}, {0.0f, 1.0f, 0.0f}, 0.7, 1.8});
            lights.push_back({{-6.88f, 8.86f, -5.66f}, {0.0f, 0.0f, 1.0f}, 0.7, 1.8});
            lights.push_back({{-3.5f, 5.2f, -7.14f}, {0.6f, 0.0f, 1.0f}, 0.7, 1.8});
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
    shader->setUniform("viewPos", scene.camera->cameraPosition);
    shader->setUniform("numLights", lights.size());

    shader->setUniform("dirLight.direction", scene.lightDirection);
    shader->setUniform("dirLight.ambient", scene.lightAmbient);
    shader->setUniform("dirLight.diffuse", scene.lightDiffuse);
    shader->setUniform("dirLight.specular", scene.lightSpecular);

    for (unsigned long i = 0; i < lights.size(); i++) {
        shader->setUniform(setLightProperty("position", i), lights[i].position);
        shader->setUniform(setLightProperty("color", i), lights[i].color);
        shader->setUniform(setLightProperty("constant", i), lights[i].constant);
        shader->setUniform(setLightProperty("linear", i), lights[i].linear);
        shader->setUniform(setLightProperty("quadratic", i), lights[i].quadratic);
        shader->setUniform(setLightProperty("ambient", i), lights[i].ambient);
        shader->setUniform(setLightProperty("diffuse", i), lights[i].diffuse);
        shader->setUniform(setLightProperty("specular", i), lights[i].specular);
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

void StaticObject::addChild(Object *s) {
    s->parent = this;
    children.push_back(s);
}
