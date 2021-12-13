#pragma once
#include <ppgso/ppgso.h>

#include "object.h"

class Kelp final : public Object{
private:
	std::unique_ptr<ppgso::Shader> shader;
	std::unique_ptr<ppgso::TextureAlpha> texture;
	//std::unique_ptr<ppgso::Texture> texture;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	
	struct face {
		GLuint v0, v1, v2;
	};
	std::vector<face> mesh;
	GLuint vao, vbo, tbo, ibo;
	
	glm::vec3 move_vec;
	
	int child_num;
	float collisionDistance = 3.0f;
	float child_y_offset;
	
	void generateIndices();
	void updateBuffers();
	

public:
	
	
	Kelp(const std::string &tex_file, int child_num, float child_offset);
	
	~Kelp();
	
	void create_children();
	
	bool update(Scene &scene, float dt) override;
	
	void render(Scene &scene) override;
	
	void addChild(Object *s) override;
};

