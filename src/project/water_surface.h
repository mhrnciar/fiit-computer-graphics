#pragma once
#include <ppgso/ppgso.h>
#include <random>

#include "object.h"

class WaterSurface final : public Object{
private:
	std::unique_ptr<ppgso::Shader> shader;
	std::unique_ptr<ppgso::Texture> texture;
	//std::unique_ptr<ppgso::TextureAlpha> texture;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	
	glm::vec3 controlPoints[4][4]{
			{ {-1.5,0,-1.5}, {-0.5,0,-1.5}, {0.5,0,-1.5}, {1.5,0,-1.5}, },
			{ {-1.5,0,-0.5}, {-0.5,0,-0.5}, {0.5,0,-0.5}, {1.5,0,-0.5}, },
			{ {-1.5,0, 0.5}, {-0.5,0, 0.5}, {0.5,0, 0.5}, {1.5,0, 0.5}, },
			{ {-1.5,0, 1.5}, {-0.5,0, 1.5}, {0.5,0, 1.5}, {1.5,0, 1.5}, },
	};
	
	struct waterPlane {
		glm::vec3 cPoints[4][4];
		int x;
		int z;
		float heights[4][4];
	};
	
	struct face {
		GLuint v0, v1, v2;
	};
	
	std::vector<waterPlane> water_planes;
	std::vector<face> mesh;
	GLuint vao, vbo, tbo, ibo;
	
	int len_x;
	int len_z;
	
	std::default_random_engine generator;
	std::normal_distribution<float> normal_dist;
	
	/*!
	 * Generate Bezier surface from control points along with uv coordinates and faces and pass the data to GPU
	 */
	void bezierPatches(waterPlane plane_in);
	void updateBuffers();
	
	glm::vec3 interpolate(const glm::vec3 &p0, const glm::vec3 &p1, const float t);
	glm::vec3 bezierPoint(const glm::vec3 controlCurvePoints[4], float t);

public:
	/*!
	 * Create a new WaterSurface object
	 */
	WaterSurface(const std::string &tex_file, int len_x_in, int len_z_in);
	
	~WaterSurface();
	
	/*!
	 * Update WaterSurface
	 * @param scene Scene to update
	 * @param dt Time delta
	 * @return true to keep the object
	 */
	bool update(Scene &scene, float dt) override;
	
	/*!
	 * Render WaterSurface
	 * @param scene Scene to render in
	 */
	void render(Scene &scene) override;

	/*!
	 * Add child object creating hierarchy
	 * @param s - child object
	 */
	void addChild(Object *s) override;
};

