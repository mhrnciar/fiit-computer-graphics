#pragma once
#include <ppgso/ppgso.h>

#include "src/project/object.h"
#include "algae.h"

class Foliage final : public Object{
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::TextureAlpha> texture;

public:
    /*!
     * Create a new static object
     */
    Foliage(float xMin, float xMax, float zMin, float zMax);

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

    void renderShadowmap(Scene &scene) override;

private:
    std::list< std::unique_ptr<Algae> > container;
    std::vector< std::string > textures = {"foliage/green_algae1.png", "foliage/green_algae2.png", "foliage/green_algae3.png",
                                           "foliage/green_algae4.png", "foliage/green_algae5.png", "foliage/green_algae6.png",
                                           "foliage/green_algae7.png", "foliage/green_algae8.png", "foliage/red_algae1.png",
                                           "foliage/red_algae2.png", "foliage/red_algae3.png", "foliage/red_algae4.png"};
};

