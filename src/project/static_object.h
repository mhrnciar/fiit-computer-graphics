#pragma once
#include <ppgso/ppgso.h>

#include "object.h"


class StaticObject final : public Object{
private:
    // Static resources (Shared between instances)
    std::unique_ptr<ppgso::Mesh> mesh;
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::Texture> texture;


public:
    #define DIFFUSE_SHADER 0
    #define LIGHT_SHADER 1

    /*!
     * Create a new static object
     */
    StaticObject(const std::string &mesh_file,
                 const std::string &tex_file,
                 int shader_type);

    /*!
     * Update player position considering keyboard inputs
     * @param scene Scene to update
     * @param dt Time delta
     * @return true to delete the object
     */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render player
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;

    void addChild(Object *s) override;
};

