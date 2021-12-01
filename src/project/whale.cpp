#include "whale.h"
#include "scene.h"

#include <ppgso/image_png.h>
#include <shaders/light_vert_glsl.h>
#include <shaders/light_frag_glsl.h>

Whale::Whale() {
    // Initialize static resources if needed
    if (!texture) texture = std::make_unique<ppgso::TextureAlpha>(ppgso::image::loadPNG("animals/whale.png"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("animals/whale.obj");
    if (!shader) shader = std::make_unique<ppgso::Shader>(light_vert_glsl, light_frag_glsl);

    keyframes.push_back({{0, 20, 0}, {0,0,0}, {1,1,1}, 10});
    keyframes.push_back({{10, 20, 0}, {0,0,0}, {1,1,1}, 10});
    keyframes.push_back({{0, 20, 0}, {0,0,0}, {1,1,1}, 10});
}

bool Whale::update(Scene &scene, float dt) {
    if (keyframes.size() > 1) {
        float x = keyframes.at(0).currTime * keyframes.at(1).position.x / keyframes.at(0).maxTime;
        float y = keyframes.at(0).currTime * keyframes.at(1).position.y / keyframes.at(0).maxTime;
        float z = keyframes.at(0).currTime * keyframes.at(1).position.z / keyframes.at(0).maxTime;

        position.x = glm::smoothstep(keyframes.at(0).position.x, keyframes.at(1).position.x, x) * x;
        printf("%lf %lf %lf\n", position.x, position.y, position.z);

        keyframes.at(0).currTime += dt;
        if (keyframes.at(0).currTime > keyframes.at(0).maxTime)
            keyframes.pop_front();
    }

    generateModelMatrix();
    return true;
}

void Whale::render(Scene &scene) {
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

void Whale::addChild(Object *s) {
    s->parent = this;
    children.push_back(s);
}
