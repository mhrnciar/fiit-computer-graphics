#include "boids.h"
#include "src/project/scene.h"

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
    static float time = 3;

    time += dt;

    if (distance(scene.camera->cameraPosition, position) < 10.0f && !separated) {
        separated = true;

        for (auto &c : container) {
            c->speed = glm::linearRand(0.1f, 0.2f);

            c->rotation.x = glm::radians(scene.camera->cameraPitch) + glm::linearRand(-ppgso::PI/4, ppgso::PI/4);
            c->rotation.z = glm::radians(scene.camera->cameraYaw) + glm::linearRand(-ppgso::PI/4, ppgso::PI/4);

            c->movement_vector.x = sin(c->rotation.z);
            c->movement_vector.y = -sin(c->rotation.x);
            c->movement_vector.z = cos(c->rotation.z);

            c->update(scene, dt);
        }
        time = 0;
    }
    else if (time > 1 && separated) {
        for (auto &c : container) {
            c->rotation.x += glm::linearRand(-ppgso::PI/4, ppgso::PI/4);
            c->rotation.z += glm::linearRand(-ppgso::PI/4, ppgso::PI/4);

            if (c->position.y < 10) {
                c->rotation.x = -ppgso::PI/4;
            }
            if (c->position.y > 80) {
                c->rotation.x = ppgso::PI/4;
            }

            if(c->position.x > -13 && c->position.x < 0 && c->position.z > -15 && c->position.z < 10) {
                c->rotation.z += ppgso::PI/2;
            }

            if (c->rotation.x > ppgso::PI/2) {
                c->rotation.x -= ppgso::PI/4;
            }
            else if (c->rotation.x < -ppgso::PI/2) {
                c->rotation.x += ppgso::PI/4;
            }

            for (auto &d : container) {
                if (c == d)
                    continue;
                if (glm::distance(c->position, d->position) < 2.0f) {
                    c->rotation.z += d->rotation.z;
                }
            }

            c->movement_vector.x = sin(c->rotation.z);
            c->movement_vector.y = -sin(c->rotation.x);
            c->movement_vector.z = cos(c->rotation.z);

            c->update(scene, dt);
        }
        time = 0;
    }
    else if (time > 3 && !separated) {
        rotation.x += glm::linearRand(-ppgso::PI/4, ppgso::PI/4);
        rotation.z += glm::linearRand(-ppgso::PI/4, ppgso::PI/4);

        if (position.y < 10) {
            rotation.x = -ppgso::PI/4;
        }
        if (position.y > 80) {
            rotation.x = ppgso::PI/4;
        }

        if (rotation.x > ppgso::PI/2) {
            rotation.x -= ppgso::PI/4;
        }
        else if (rotation.x < -ppgso::PI/2) {
            rotation.x += ppgso::PI/4;
        }

        vector.x = sin(rotation.z);
        vector.y = -sin(rotation.x);
        vector.z = cos(rotation.z);

        position += vector * speed;

        for (auto &c : container) {
            c->rotation = rotation;
            c->movement_vector = vector;
            c->update(scene, dt);
        }

        time = 0;
    }
    else {
        position += vector * speed;

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

void Boids::renderShadowmap(Scene &scene) {
}
