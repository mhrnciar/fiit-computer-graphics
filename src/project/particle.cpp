//
// Created by Leonard on 12/8/2021.
//
#include <map>
#include "particle.h"
#include "scene.h"
#include <ppgso/image_png.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

//#include <shaders/diffuse_vert_glsl.h>
//#include <shaders/diffuse_frag_glsl.h>

Particle::Particle(const std::string &tex_file, float time_to_live, float gravity_effectiveness, glm::vec3 velocity, float wce) {
	// Initialize static resources if needed
	if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
	//if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(tex_file));
	if (!texture) texture = std::make_unique<ppgso::TextureAlpha>(ppgso::image::loadPNG(tex_file));
	
	this->time_to_live = time_to_live;
	this->velocity = velocity;
	this->gravity_effectiveness = gravity_effectiveness;
	this->water_current_effectiveness = wce;
	
	
	vertices = {
			{-0.5f, -0.5f, 0.0f},
			{-0.5f, 0.5f, 0.0f},
			{0.5f, 0.5f, 0.0f},
			{0.5f, -0.5f, 0.0f},
	};
	
	face triangle1 = {0,1,2};
	face triangle2 = {0,2,3};
	face triangle1_inv = {2,1,0};
	face triangle2_inv = {3,2,0};
	mesh = {
		triangle1_inv,
		triangle2_inv,
	};
	texCoords = {
			{1, 0},
			{1, 1},
			{0, 1},
			{0, 0},
	};
	updateBuffers();
	
}

Particle::~Particle() {
	// Delete data from OpenGL
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &tbo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}


void Particle::updateBuffers() {
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


bool Particle::update(Scene &scene, float dt) {
	
	time_elapsed += dt;
	if (time_elapsed >= time_to_live){
		return false;
	}
	
	glm::vec3 vel_vec = velocity;
	vel_vec += (scene.water_current * water_current_effectiveness);
	
	position += vel_vec * dt;
	
	velocity.y -= 1 * gravity_effectiveness * dt;
	
	
	
	
	generateModelMatrixBilboard(scene);
	return true;
}

void Particle::render(Scene &scene) {
	shader->use();
	
	shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
	shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
	
	shader->setUniform("ModelMatrix", modelMatrix);
	shader->setUniform("Texture", *texture);
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
	glDrawElements(GL_TRIANGLES, mesh.size() * sizeof(face), GL_UNSIGNED_INT, nullptr);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	for(auto & i : children) {
		i->render(scene);
	}
}

void Particle::addChild(Object *s) {
	s->parent = this;
	children.push_back(s);
}

void Particle::generateModelMatrixBilboard(Scene &scene) {
	
	translationMatrix = glm::translate(glm::mat4(1.0f), position);
	translationMatrix[0][0] = scene.camera->viewMatrix[0][0];
	translationMatrix[0][1] = scene.camera->viewMatrix[1][0];
	translationMatrix[0][2] = scene.camera->viewMatrix[2][0];
	translationMatrix[1][0] = scene.camera->viewMatrix[0][1];
	translationMatrix[1][1] = scene.camera->viewMatrix[1][1];
	translationMatrix[1][2] = scene.camera->viewMatrix[2][1];
	translationMatrix[2][0] = scene.camera->viewMatrix[0][2];
	translationMatrix[2][1] = scene.camera->viewMatrix[1][2];
	translationMatrix[2][2] = scene.camera->viewMatrix[2][2];
	rotation.x = 0.0f;
	rotation.z = 0.0f;
	rotationMatrix = glm::orientate4(rotation);
	scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
	modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	
}


