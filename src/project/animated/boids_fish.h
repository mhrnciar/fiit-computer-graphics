#pragma once
#include <ppgso/ppgso.h>

#include "src/project/object.h"

class BoidsFish final : public Object{
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::TextureAlpha> texture;


public:
    /*!
     * Create a new boids fish
     */
    BoidsFish();

    /*!
     * Update boids fish
     * @param scene Scene to update
     * @param dt Time delta
     * @return true to delete the object
     */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render boids fish
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;

    glm::vec3 movement_vector{0, 0, 0};
    float speed = 0.01f;
};

