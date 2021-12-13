#include <ppgso/image_png.h>
#include <shaders/light_vert_glsl.h>
#include <shaders/light_frag_glsl.h>

#include "fish_chased.h"
#include "src/project/scene.h"

ChasedFish::ChasedFish() {
    // Initialize static resources if needed
    if (!texture) texture = std::make_unique<ppgso::TextureAlpha>(ppgso::image::loadPNG("animals/crucian_carp.png"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("animals/crucian_carp.obj");
    if (!shader) shader = std::make_unique<ppgso::Shader>(light_vert_glsl, light_frag_glsl);

    keyframes.push_back({{0, 0, 0}, {0, 0, 0}, {0, 0, ppgso::PI/2}, {0, 0, ppgso::PI/2}, 62});
    keyframes.push_back({{0, 0, 0}, {-9.0f, 7.0f, -6.0f}, {0, 0, ppgso::PI/2}, {0, 0, ppgso::PI/2}, 0.001f});
    keyframes.push_back({{-9.0f, 7.0f, -6.0f}, {7.0f, 7.0f, -6.0f}, {0, 0, ppgso::PI/2}, {0, 0, ppgso::PI/2}, 4});
    keyframes.push_back({{7.0f, 7.0f, -6.0f}, {20.0f, 15.0f, -6.0f}, {0, 0, ppgso::PI/2}, {-ppgso::PI/4, 0, ppgso::PI/2}, 3});
    keyframes.push_back({{20.0f, 15.0f, -6.0f}, {20.0f, 18.0f, 0.0f}, {-ppgso::PI/4, 0, ppgso::PI/2}, {0, 0, 0}, 2});
    keyframes.push_back({{20.0f, 18.0f, 0.0f}, {32.0f, 25.0f, 6.0f}, {0, 0, 0}, {-ppgso::PI/4, 0, ppgso::PI/2}, 2});
    keyframes.push_back({{32.0f, 25.0f, 6.0f}, {38.0f, 28.0f, 0.0f}, {-ppgso::PI/4, 0, ppgso::PI/2}, {0, 0, ppgso::PI}, 2});
    keyframes.push_back({{38.0f, 28.0f, 0.0f}, {38.0f, 28.0f, -12.0f}, {0, 0, ppgso::PI}, {0, 0, ppgso::PI}, 2});
    keyframes.push_back({{38.0f, 28.0f, -12.0f}, {28.0f, 28.0f, -16.0f}, {0, 0, ppgso::PI}, {0, 0, -ppgso::PI/2}, 1});
    keyframes.push_back({{28.0f, 28.0f, -16.0f}, {-20.0f, 28.0f, -16.0f}, {0, 0, -ppgso::PI/2}, {0, 0, -ppgso::PI/2}, 4.8});
    keyframes.push_back({{-20.0f, 28.0f, -16.0f}, {-24.0f, 28.0f, -12.0f}, {0, 0, -ppgso::PI/2}, {0, 0, 0}, 0.7});
    keyframes.push_back({{-24.0f, 28.0f, -12.0f}, {-24.0f, 28.0f, -5.0f}, {0, 0, 0}, {0, 0, 0}, 3});
}

bool ChasedFish::update(Scene &scene, float dt) {
    static int count = 0;
    if (!keyframes.empty()) {
        modelMatrix = keyframes[count].interpolateModelMatrix();

        keyframes[count].currTime += dt;
        if (keyframes[count].currTime > keyframes[count].maxTime) {
            keyframes[count].currTime = 0;
            count++;
            if (count >= keyframes.size())
                return false;
        }
    }

    return true;
}

void ChasedFish::render(Scene &scene) {
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

void ChasedFish::addChild(Object *s) {
    s->parent = this;
    children.push_back(s);
}
