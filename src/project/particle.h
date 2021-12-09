#pragma once
#include <ppgso/ppgso.h>
#include <random>

#include "object.h"

class Particle final : public Object{
private:
	std::unique_ptr<ppgso::Shader> shader;
	//std::unique_ptr<ppgso::Texture> texture;
	std::unique_ptr<ppgso::TextureAlpha> texture;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	
	struct face {
		GLuint v0, v1, v2;
	};

	std::vector<face> mesh;
	GLuint vao, vbo, tbo, ibo;
	
	float gravity_effectiveness;
	float time_to_live;
	float time_elapsed = 0.0f;
	
	glm::vec3 velocity;
	
	void updateBuffers();
	
public:

	Particle(const std::string &tex_file, float time_to_live, float gravity_effectiveness, glm::vec3 velocity);
	
	~Particle();
	
	bool update(Scene &scene, float dt) override;
	
	void render(Scene &scene) override;
	
	void renderShadowmap(Scene &scene) override;

	void addChild(Object *s) override;
	
	void generateModelMatrixBilboard(Scene &scene);
};

