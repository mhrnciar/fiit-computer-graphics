#pragma once
#include <ppgso/ppgso.h>

#include "src/project/object.h"

class ChasedFish final : public Object{
private:
    // Static resources (Shared between instances)
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::TextureAlpha> texture;


public:
    /*!
     * Create a new chased fish object
     */
    ChasedFish();

    /*!
     * Update chased fish object
     * @param scene Scene to update
     * @param dt Time delta
     * @return true to delete the object
     */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render chased fish object
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;

    /*!
     * Add child object creating hierarchy
     * @param s - child object
     */
    void addChild(Object *s) override;
};

