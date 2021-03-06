#pragma once
#include <ppgso/ppgso.h>

#include "src/project/object.h"

class WhaleFin final : public Object{
private:
    // Static resources (Shared between instances)
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::TextureAlpha> texture;
    bool right;


public:
    /*!
     * Create a new whale fin
     */
    WhaleFin(bool right);

    /*!
     * Update whale fin
     * @param scene Scene to update
     * @param dt Time delta
     * @return true to delete the object
     */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render whale fin
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;

    /*!
     * Add child object creating hierarchy
     * @param s - child object
     */
    void addChild(Object *s) override;
};

