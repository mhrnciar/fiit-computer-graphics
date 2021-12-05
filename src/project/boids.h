#pragma once
#include <ppgso/ppgso.h>

#include "object.h"
#include "boids_fish.h"

class Boids final : public Object{
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::TextureAlpha> texture;


public:
    /*!
     * Create a new static object
     */
    Boids(glm::vec3 pos, glm::vec3 rot);

    /*!
     * Update static object
     * @param scene Scene to update
     * @param dt Time delta
     * @return true to delete the object
     */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render static object
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;

    glm::vec3 position{0, 0, 0};
    glm::vec3 rotation{0, 0, 0};
    float speed = 0.01f;
    bool separated = false;

private:
    std::list< std::unique_ptr<BoidsFish> > container;
};

