#include "static_object.h"
#include "scene.h"

#include <string>

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>
#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>
#include <shaders/light_vert_glsl.h>
#include <shaders/light_frag_glsl.h>

#include <shaders/shadowmap_vert_glsl.h>
#include <shaders/shadowmap_frag_glsl.h>

StaticObject::StaticObject(const std::string &mesh_file, const std::string &tex_file, int shader_type) {
    // Initialize static resources if needed
    if (tex_file[tex_file.size() - 1] == 'p'){
	    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(tex_file));
	    tex_type = 0;
    }
    else {
	    if (!texture_alpha) texture_alpha = std::make_unique<ppgso::TextureAlpha>(ppgso::image::loadPNG(tex_file));
	    tex_type = 1;
    }
    
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
        }
    }
}

bool StaticObject::update(Scene &scene, float dt) {
    generateModelMatrix();

    // Update children
    for (auto c : children) {
        c->update(scene, dt);
    }
    return true;
}

void StaticObject::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("viewPos", scene.camera->cameraPosition);
    shader->setUniform("numLights", scene.lights.size());

    // Directional light
    shader->setUniform("dirLight.direction", scene.lightDirection);
    shader->setUniform("dirLight.ambient", scene.lightAmbient);
    shader->setUniform("dirLight.diffuse", scene.lightDiffuse);
    shader->setUniform("dirLight.specular", scene.lightSpecular);

    // Point lights
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
    
    if (tex_type == 0){
	    shader->setUniform("material.diffuse", *texture);
    }
    else {
	    shader->setUniform("material.diffuse", *texture_alpha);
    }
    
    shader->setUniform("material.shininess", shininess);
    mesh->render();

    // Render children
    for(auto & i : children) {
        i->render(scene);
    }
}

void StaticObject::addChild(Object *s) {
    s->parent = this;
    children.push_back(s);
}
