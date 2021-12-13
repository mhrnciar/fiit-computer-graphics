
#include "particle_emitter.h"
#include "particle.h"
#include "scene.h"
#include <shaders/texture_vert_glsl.h>


ParticleEmitter::ParticleEmitter(glm::vec3 pos, const std::string &tex_file, float time_delay, int particle_num, glm::vec3 vel,  glm::vec3 scale, float grav, float ttl, float wce) {
	
	this->position = pos;
	this->tex_file = tex_file;
	this->prod_time_delay = time_delay;
	this->particles_to_produce = particle_num;
	this->gravity_effectiveness = grav;
	this->time_to_live = ttl;
	this->velocity = vel;
	this->scale = scale;
	
	this->water_current_effectiveness = wce;
	
}

ParticleEmitter::~ParticleEmitter() {
	int i = 0;
}

bool ParticleEmitter::update(Scene &scene, float dt) {
	
	time_elapsed += dt;
	if (time_elapsed >= prod_time_delay){
		time_elapsed = 0.0f;
		
		for (int i = 0; i < particles_to_produce; i++){
			glm::vec3 p_vel = this->velocity;
			auto new_particle = std::make_unique<Particle>(tex_file, time_to_live, gravity_effectiveness, p_vel, water_current_effectiveness);
			new_particle->position = this->position;
			new_particle->scale = this->scale;
			particles.push_back(move(new_particle));
		}
	}
	
	auto i = std::begin(particles);
	while (i != std::end(particles)) {
		auto obj = i->get();
		if (!obj->update(scene, dt))
			i = particles.erase(i);
		else
			++i;
	}
	return true;
}

void ParticleEmitter::render(Scene &scene) {
	
	for(auto & i : particles) {
		i->render(scene);
	}
}

void ParticleEmitter::addChild(Object *s) {
	s->parent = this;
	children.push_back(s);
}
