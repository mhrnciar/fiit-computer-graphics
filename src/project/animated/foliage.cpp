#include "foliage.h"
#include "algae.h"
#include "src/project/scene.h"

Foliage::Foliage() {
    float posx, posz;
    int size = textures.size() - 1;

    for (int i = 0; i < 200; i++) {
        do {
            posx = glm::linearRand(-30.0f, 25.0f);
            posz = glm::linearRand(-30.0f, 25.0f);
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

void Foliage::renderShadowmap(Scene &scene) {
}
