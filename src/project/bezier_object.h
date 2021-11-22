#pragma once
#include <ppgso/ppgso.h>

#include "object.h"

class BezierObject final : public Object{
private:
    std::unique_ptr<ppgso::Shader> shader;
    std::unique_ptr<ppgso::TextureAlpha> texture;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;

    glm::vec3 controlPoints[4][4]{
            { {-1,4,0}, {-0.5f,4,0}, {0.5f,4,0}, {1,4,0}, },
            { {-1,1.5f,0}, {-0.5f,1.5f,0}, {0.5f,1.5f,0}, {1,1.5f,0}, },
            { {-1,-1.5f,0}, {-0.5f,-1.5f,0}, {0.5f,-1.5f,0}, {1,-1.5f,0}, },
            { {-1,-4,0}, {-0.5f,-4,0}, {0.5f,-4,0}, {1,-4,0} }
    };

    struct face {
        GLuint v0, v1, v2;
    };

    std::vector<face> mesh;
    GLuint vao, vbo, tbo, ibo;

    /*!
     * Generate Bezier surface from control points along with uv coordinates and faces and pass the data to GPU
     */
    void bezierPatch();

public:
    /*!
     * Create a new Bezier object
     */
    BezierObject(const std::string &tex_file);

    ~BezierObject();

    /*!
     * Update Bezier surface
     * @param scene Scene to update
     * @param dt Time delta
     * @return true to keep the object
     */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render Bezier surface
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;

    /*!
     * Add child object creating hierarchy
     * @param s - child object
     */
    void addChild(Object *s) override;
};

