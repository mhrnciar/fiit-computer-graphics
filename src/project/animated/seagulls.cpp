#include <ppgso/image_png.h>
#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

#include "seagulls.h"
#include "src/project/scene.h"

Seagulls::Seagulls() {
    // Initialize static resources if needed
    if (!texture) texture = std::make_unique<ppgso::TextureAlpha>(ppgso::image::loadPNG("animals/seagulls.png"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("animals/seagulls.obj");
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
}

bool Seagulls::update(Scene &scene, float dt) {
    if (scene.camera->cameraPosition.y > 87) {
        rotation.z += dt;
    }

    generateModelMatrix();
    return true;
}

void Seagulls::render(Scene &scene) {
    shader->use();

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

void Seagulls::addChild(Object *s) {
    s->parent = this;
    children.push_back(s);
}
