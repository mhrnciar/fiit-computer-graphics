//
// Created by Leonard on 12/8/2021.
//
#include <ppgso/ppgso.h>
#include <random>

#include "object.h"

class ParticleEmitter final : public Object{
private:
	
	std::string tex_file;
	float prod_time_delay;
	int particles_to_produce;
	float time_elapsed = 0.0f;
	float time_to_live;
	float gravity_effectiveness;
	glm::vec3 velocity;
	glm::vec3 scale;

public:
	ParticleEmitter(glm::vec3 pos, const std::string &tex_file, float time_delay, int particle_num, glm::vec3 vel, glm::vec3 scale, float grav, float ttl);
	
	~ParticleEmitter();
	
	bool update(Scene &scene, float dt) override;
	
	void render(Scene &scene) override;
	
	void renderShadowmap(Scene &scene) override;
	
	void addChild(Object *s) override;
	
	std::list< std::unique_ptr<Object> > particles;

};

