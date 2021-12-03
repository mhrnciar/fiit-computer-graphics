
#include "kelp.h"
#include "scene.h"

#include <ppgso/image_png.h>

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

Kelp::Kelp(const std::string &tex_file, int child_num) {
	// Initialize static resources if needed
	if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
	if (!texture) texture = std::make_unique<ppgso::TextureAlpha>(ppgso::image::loadPNG(tex_file));
	//if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(tex_file));
	
	vertices = {
			{-1.0f, -2.0f, 0.0f}, // x_plane bottom left
			{-1.0f, 2.0f,  0.0f},  // x_plane top left
			{1.0f,  2.0f,  0.0f},   // x_plane top right
			{1.0f,  -2.0f, 0.0f},  // x_plane bottom right
			{0.0f,  -2.0f, -1.0f}, // z_plane bottom left
			{0.0f,  2.0f,  -1.0f},  // z_plane top left
			{0.0f,  2.0f,  1.0f},   // z_plane top right
			{0.0f,  -2.0f, 1.0f},  // z_plane bottom right
	};
	
	
	texCoords = {
			
			{1, 0},
			{1, 1},
			{0, 1},
			{0, 0},
			{1, 0},
			{1, 1},
			{0, 1},
			{0, 0},
	};
	move_vec = {0.0f, 0.0f, 0.0f};
	
	generateIndices();
	updateBuffers();
	this->child_num = child_num;
	child_y_offset = 2;
	
	
}

void Kelp::create_children() {
	if (child_num > 0) {
		auto kelp = new Kelp("seaweed_tex.png", child_num - 1);
		kelp->position = {0, child_y_offset, 0};
		this->addChild(kelp);
		kelp->create_children();
	}
}



Kelp::~Kelp() {
	// Delete data from OpenGL
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &tbo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void Kelp::generateIndices() {
	
	mesh.clear();
	face triangle_x_1 = {0,1,2};
	face triangle_x_2 = {2,3,0};
	face triangle_x_3 = {2,1,0};
	face triangle_x_4 = {0,3,2};
	
	face triangle_z_1 = {4,5,6};
	face triangle_z_2 = {6,7,4};
	face triangle_z_3 = {6,5,4};
	face triangle_z_4 = {4,7,6};
	
	mesh = {triangle_x_1, triangle_x_2, triangle_x_3, triangle_x_4,
		 triangle_z_1, triangle_z_2, triangle_z_3, triangle_z_4};
	
}

void Kelp::updateBuffers() {
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
}


bool Kelp::update(Scene &scene, float dt) {
	auto time = (float) glfwGetTime();
	
	if (this->parent != NULL){
		glm::vec3 real_pos = this->getRealPosition();
		if (glm::distance(real_pos, scene.camera->cameraPosition) <= collisionDistance){
			glm::vec3 difference_vector = real_pos - scene.camera->cameraPosition;
			move_vec.x = difference_vector.x;
			move_vec.z = difference_vector.z;
			
			if (glm::distance(position + move_vec * dt, {0,child_y_offset,0}) <= 0.5f){
			
			}
			else {
				move_vec = {0.0f,0.0f,0.0f};
			}
		}
		else {
			glm::vec3 ideal_position = {0.0f, child_y_offset, 0.0f};
			move_vec.x = ideal_position.x - position.x * 0.5f;
			move_vec.z = ideal_position.z - position.z * 0.5f;
		}
		
		position += move_vec * dt;
		
	}
	
	
	for(auto & i : children) {
		i->update(scene, dt);
	}
	
	generateModelMatrix();
	return true;
}

void Kelp::render(Scene &scene) {
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

void Kelp::addChild(Object *s) {
	s->parent = this;
	children.push_back(s);
}


