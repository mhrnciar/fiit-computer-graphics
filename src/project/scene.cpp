#include "scene.h"
#include "static_object.h"


float randfrom(float min, float max)
{
	float range = (max - min);
	float div = RAND_MAX / range;
	return min + (rand() / div);
}


void Scene::update(float dt) {
    camera->update(*this, dt);
    
    
    water_current_time_elapsed += dt;
    if(water_current_time_elapsed >= 5.0f){
    	water_current_changes++;
    	if (water_current_changes % 2 == 0){
		    water_current.x = 0.0f;
		    water_current.z = 0.0f;
    	}
    	else {
		    water_current.x = randfrom(-1.0f, 1.0f);
		    water_current.z = randfrom(-1.0f, 1.0f);
    	}
    	water_current_time_elapsed = 0.0f;
    }

    // Use iterator to update all objects so we can remove while iterating
    auto i = std::begin(objects);

    while (i != std::end(objects)) {
        // Update and remove from list if needed
        auto obj = i->get();
        if (!obj->update(*this, dt))
            i = objects.erase(i); // NOTE: no need to call destructors as we store shared pointers in the scene
        else
            ++i;
    }
}

void Scene::render() {
    // Simply render all objects
    for ( auto& obj : objects )
        obj->render(*this);
}

std::vector<Object*> Scene::intersect(const glm::vec3 &position, const glm::vec3 &direction) {
    std::vector<Object*> intersected = {};
    for(auto& object : objects) {
        // Collision with sphere of size object->scale.x
        auto oc = position - object->position;
        auto radius = object->scale.x;
        auto a = glm::dot(direction, direction);
        auto b = glm::dot(oc, direction);
        auto c = glm::dot(oc, oc) - radius * radius;
        auto dis = b * b - a * c;

        if (dis > 0) {
            auto e = sqrt(dis);
            auto t = (-b - e) / a;

            if ( t > 0 ) {
                intersected.push_back(object.get());
                continue;
            }

            t = (-b + e) / a;

            if ( t > 0 ) {
                intersected.push_back(object.get());
                continue;
            }
        }
    }

    return intersected;
}
