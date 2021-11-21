#pragma once
#include <ppgso/ppgso.h>

#include "object.h"


class BezierObject final : public Object{
private:
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::TextureAlpha> texture;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;

    struct face {
        GLuint v0, v1, v2;
    };

    std::vector<face> mesh;
    GLuint vao, vbo, tbo, ibo;

    void bezierPatch(const glm::vec3 controlPoints[4][4]);

public:
    /*!
     * Create a new static object
     */
    BezierObject(const glm::vec3 controlPoints[4][4], const std::string &tex_file);

    ~BezierObject();

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

