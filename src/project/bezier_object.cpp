#include "bezier_object.h"
#include "scene.h"
#include <ppgso/image_png.h>

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

BezierObject::BezierObject(const glm::vec3 controlPoints[4][4], const std::string &tex_file) {
    // Initialize static resources if needed
    if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
    bezierPatch(controlPoints);

    texture = std::make_unique<ppgso::TextureAlpha>(ppgso::image::loadPNG(tex_file));
}

BezierObject::~BezierObject() {
    // Delete data from OpenGL
    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &tbo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}

glm::vec3 interpolate(const glm::vec3 &p0, const glm::vec3 &p1, const float t){
    float x = (1-t) * p0.x + t * p1.x;
    float y = (1-t) * p0.y + t * p1.y;
    float z = (1-t) * p0.z + t * p1.z;
    return {x, y, z};
}

glm::vec3 bezierPoint(const glm::vec3 controlPoints[4], float t) {
    glm::vec3 linear_p1 = interpolate(controlPoints[0], controlPoints[1], t);
    glm::vec3 linear_p2 = interpolate(controlPoints[1], controlPoints[2], t);
    glm::vec3 linear_p3 = interpolate(controlPoints[2], controlPoints[3], t);

    glm::vec3 quadratic_p1 = interpolate(linear_p1, linear_p2, t);
    glm::vec3 quadratic_p2 = interpolate(linear_p2, linear_p3, t);

    glm::vec3 cubic_p1 = interpolate(quadratic_p1, quadratic_p2, t);
    return cubic_p1;
}

void BezierObject::bezierPatch(const glm::vec3 controlPoints[4][4]) {
    vertices.clear();
    texCoords.clear();
    
    unsigned int PATCH_SIZE = 10;
    for(unsigned int i = 0; i < PATCH_SIZE ; i++) {
        for (unsigned int j = 0; j < PATCH_SIZE; j++) {
            float u = (float) i / (float)(PATCH_SIZE - 1);
            float v = (float) j / (float)(PATCH_SIZE - 1);
            
            glm::vec3 points_u[] = {bezierPoint(controlPoints[0], u),
                                    bezierPoint(controlPoints[1], u),
                                    bezierPoint(controlPoints[2], u),
                                    bezierPoint(controlPoints[3], u)};

            glm::vec3 point_v = bezierPoint(points_u, v);

            vertices.push_back(point_v);
            texCoords.emplace_back(u, 1-v);
        }
    }
    
    // Generate indices
    for(unsigned int i = 1; i < PATCH_SIZE; i++) {
        for (unsigned int j = 1; j < PATCH_SIZE; j++) {
            face triangle1 = {(i - 1) * PATCH_SIZE + (j - 1),
                              i * PATCH_SIZE + (j - 1),
                              (i - 1) * PATCH_SIZE + j};
            mesh.push_back(triangle1);
            
            face triangle2 = {i * PATCH_SIZE + (j - 1),
                              i * PATCH_SIZE + j,
                              (i - 1) * PATCH_SIZE + j};
            mesh.push_back(triangle2);
        }
    }

    // Copy data to OpenGL
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Copy positions to gpu
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    // Set vertex program inputs
    auto position_attrib = shader->getAttribLocation("Position");
    glEnableVertexAttribArray(position_attrib);
    glVertexAttribPointer(position_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Copy texture positions to gpu
    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), texCoords.data(), GL_STATIC_DRAW);

    // Set vertex program inputs
    auto texCoord_attrib = shader->getAttribLocation("TexCoord");
    glEnableVertexAttribArray(texCoord_attrib);
    glVertexAttribPointer(texCoord_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Copy indices to gpu
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.size() * sizeof(face), mesh.data(), GL_STATIC_DRAW);
};

bool BezierObject::update(Scene &scene, float dt) {
    generateModelMatrix();
    return true;
}

void BezierObject::render(Scene &scene) {
    shader->use();

    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    shader->setUniform("ModelMatrix", modelMatrix);
    shader->setUniform("Texture", *texture);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
    glDrawElements(GL_TRIANGLES, mesh.size() * sizeof(face), GL_UNSIGNED_INT, nullptr);

    for(auto & i : children) {
        i->render(scene);
    }
}

void BezierObject::addChild(Object *s) {
    s->parent = this;
    children.push_back(s);
}


