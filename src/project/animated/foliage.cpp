#include "foliage.h"
#include "algae.h"
#include "src/project/scene.h"

Foliage::Foliage(float xMin, float xMax, float zMin, float zMax) {
    float posx, posz;
    int size = textures.size() - 1;

    // Generate 400 bezier surfaces representing algae and pass random texture from list to them
    for (int i = 0; i < 400; i++) {
        do {
            posx = glm::linearRand(xMin, xMax);
            posz = glm::linearRand(zMin, zMax);
        } while(posx > -13 && posx < 0 && posz > -15 && posz < 10);

        int choice = glm::linearRand(0, size);

        auto algae = std::make_unique<Algae>(textures[choice]);
        algae->position = {posx, 5.0f, posz};
        algae->rotation = {0, 0, glm::linearRand(-ppgso::PI/4, ppgso::PI/4)};
        algae->scale = {0.2f, 0.2f, 0.2f};

        container.push_back(move(algae));
    }
}

bool Foliage::update(Scene &scene, float dt) {
    for(auto &c : container) {
        c->update(scene, dt);
    }

    return true;
}

void Foliage::render(Scene &scene) {
    for(auto &c : container) {
        c->render(scene);
    }
}
