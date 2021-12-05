#include "boids.h"
#include "scene.h"

Boids::Boids(glm::vec3 pos, glm::vec3 rot) {
    position = pos;
    rotation = rot;

    auto fish = std::make_unique<BoidsFish>();
    fish->position = position;
    container.push_back(move(fish));

    fish = std::make_unique<BoidsFish>();
    fish->position = position + glm::vec3{0, 4, 0};
    container.push_back(move(fish));

    fish = std::make_unique<BoidsFish>();
    fish->position = position + glm::vec3{-1, -4, 0};
    container.push_back(move(fish));

    fish = std::make_unique<BoidsFish>();
    fish->position = position + glm::vec3{4, 0, 0};
    container.push_back(move(fish));

    fish = std::make_unique<BoidsFish>();
    fish->position = position + glm::vec3{-2, 0, 0};
    container.push_back(move(fish));

    fish = std::make_unique<BoidsFish>();
    fish->position = position + glm::vec3{2, 0, 4};
    container.push_back(move(fish));

    fish = std::make_unique<BoidsFish>();
    fish->position = position + glm::vec3{-3, 0, -4};
    container.push_back(move(fish));

    fish = std::make_unique<BoidsFish>();
    fish->position = position + glm::vec3{5, 0, -4};
    container.push_back(move(fish));

    fish = std::make_unique<BoidsFish>();
    fish->position = position + glm::vec3{0, 3, -3};
    container.push_back(move(fish));

    fish = std::make_unique<BoidsFish>();
    fish->position = position + glm::vec3{1, 3, 3};
    container.push_back(move(fish));

    fish = std::make_unique<BoidsFish>();
    fish->position = position + glm::vec3{2, -5, -4};
    container.push_back(move(fish));

    fish = std::make_unique<BoidsFish>();
    fish->position = position + glm::vec3{0, 5, -4};
    container.push_back(move(fish));

    fish = std::make_unique<BoidsFish>();
    fish->position = position + glm::vec3{4, 2, -4};
    container.push_back(move(fish));

    fish = std::make_unique<BoidsFish>();
    fish->position = position + glm::vec3{-4, 2, -4};
    container.push_back(move(fish));
}

bool Boids::update(Scene &scene, float dt) {
    static float time = 0;

    time += dt;

    if (distance(scene.camera->cameraPosition, position) < 4.0f && !separated) {
        separated = true;
        speed = 0.1f;

        for (auto &c : container) {
            c->speed = speed;

            c->rotation.x += glm::linearRand(-ppgso::PI/4, ppgso::PI/4);
            c->rotation.z += glm::linearRand(-ppgso::PI/4, ppgso::PI/4);

            c->movement_vector.x = sin(c->rotation.z);
            c->movement_vector.y = -sin(c->rotation.x);
            c->movement_vector.z = cos(c->rotation.z);

            c->update(scene, dt);
        }
    }
    else if (separated && time > 1) {
        for (auto &c : container) {
            c->rotation.x += glm::linearRand(-ppgso::PI/4, ppgso::PI/4);
            c->rotation.z += glm::linearRand(-ppgso::PI/4, ppgso::PI/4);

            c->movement_vector.x = sin(c->rotation.z);
            c->movement_vector.y = -sin(c->rotation.x);
            c->movement_vector.z = cos(c->rotation.z);

            c->update(scene, dt);
        }
        time = 0;
    }
    else if (time > 3) {
        rotation.x += glm::linearRand(-ppgso::PI/4, ppgso::PI/4);
        rotation.z += glm::linearRand(-ppgso::PI/4, ppgso::PI/4);

        glm::vec3 move;

        move.x = sin(rotation.z);
        move.y = -sin(rotation.x);
        move.z = cos(rotation.z);

        position += move * speed;

        for (auto &c : container) {
            c->rotation = rotation;
            c->movement_vector = move;
            c->update(scene, dt);
        }

        time = 0;
    }
    else {
        for (auto &c : container) {
            c->update(scene, dt);
        }
    }

    return true;
}

void Boids::render(Scene &scene) {
    for(auto &c : container) {
        c->render(scene);
    }
}
