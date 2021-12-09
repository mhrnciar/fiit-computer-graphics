#include "water_surface.h"
#include "scene.h"
#include <ppgso/image_png.h>

#include <shaders/texture_vert_glsl.h>
#include <shaders/texture_frag_glsl.h>

//#include <shaders/diffuse_vert_glsl.h>
//#include <shaders/diffuse_frag_glsl.h>

WaterSurface::WaterSurface(const std::string &tex_file, int len_x_in, int len_z_in) {
	// Initialize static resources if needed
	if (!shader) shader = std::make_unique<ppgso::Shader>(texture_vert_glsl, texture_frag_glsl);
	if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP(tex_file));
	
	len_x = len_x_in;
	len_z = len_z_in;
	
	generator.seed(time(NULL));

	//Create water_planes field based on len_x and len_z
	water_planes.clear();
	for(int i = 0; i < len_x; i++){
		for(int u = 0; u < len_z; u++){
			waterPlane new_plane;
			for (int z_coord = 0; z_coord < 4; z_coord++){
				for (int x_coord = 0; x_coord < 4; x_coord++){
					new_plane.cPoints[z_coord][x_coord] = controlPoints[z_coord][x_coord];
					new_plane.cPoints[z_coord][x_coord].x += 3 * i;
					new_plane.cPoints[z_coord][x_coord].z += 3 * u;
					new_plane.heights[z_coord][x_coord] = normal_dist(generator);
					
					if (z_coord == 0 && i > 0) {
						new_plane.heights[z_coord][x_coord] = water_planes[u + ((i - 1) * len_z)].heights[3][x_coord];
					}
					if (x_coord == 0 && u > 0) {
						new_plane.heights[z_coord][x_coord] = water_planes[(u- 1) + ((i) * len_z)].heights[z_coord][3];
					}
				}
			}
			new_plane.x = i;
			new_plane.z = u;
			water_planes.push_back(new_plane);
		}
	}
	
	vertices.clear();
	texCoords.clear();
	mesh.clear();
	
	
	// Generate Bezier suface
	for (int wp_idx = 0; wp_idx < water_planes.size(); wp_idx++){
		bezierPatches(water_planes[wp_idx]);
	}

	updateBuffers();
	
}

WaterSurface::~WaterSurface() {
	// Delete data from OpenGL
	glDeleteBuffers(1, &ibo);
	glDeleteBuffers(1, &tbo);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

glm::vec3 WaterSurface::interpolate(const glm::vec3 &p0, const glm::vec3 &p1, const float t){
	float x = (1-t) * p0.x + t * p1.x;
	float y = (1-t) * p0.y + t * p1.y;
	float z = (1-t) * p0.z + t * p1.z;
	return {x, y, z};
}

/*!
 * Generate Bezier point using de Casteljau algorithm
 * @param controlPoints - four control points defining Bezier curve
 * @param t - time delta
 * @return - point on Bezier curve
 */
glm::vec3 WaterSurface::bezierPoint(const glm::vec3 controlCurvePoints[4], float t) {
	glm::vec3 linear_p1 = interpolate(controlCurvePoints[0], controlCurvePoints[1], t);
	glm::vec3 linear_p2 = interpolate(controlCurvePoints[1], controlCurvePoints[2], t);
	glm::vec3 linear_p3 = interpolate(controlCurvePoints[2], controlCurvePoints[3], t);
	
	glm::vec3 quadratic_p1 = interpolate(linear_p1, linear_p2, t);
	glm::vec3 quadratic_p2 = interpolate(linear_p2, linear_p3, t);
	
	glm::vec3 cubic_p1 = interpolate(quadratic_p1, quadratic_p2, t);
	return cubic_p1;
}

void WaterSurface::bezierPatches(waterPlane plane_in) {
	// Generate Bezier patch points and incidences
	
	unsigned int PATCH_SIZE = 10;
	for(unsigned int i = 0; i < PATCH_SIZE ; i++) {
		for (unsigned int j = 0; j < PATCH_SIZE; j++) {
			float u = (float) i / (float)(PATCH_SIZE - 1);
			float v = (float) j / (float)(PATCH_SIZE - 1);
			
			glm::vec3 points_u[] = {bezierPoint(plane_in.cPoints[0], u),
			                        bezierPoint(plane_in.cPoints[1], u),
			                        bezierPoint(plane_in.cPoints[2], u),
			                        bezierPoint(plane_in.cPoints[3], u)};
			
			glm::vec3 point_v = bezierPoint(points_u, v);
			
			vertices.push_back(point_v);
			texCoords.emplace_back(u, 1-v);
		}
	}
	
	
	// Generate indices for upward facing triangles
	int offset = (plane_in.x *  len_z *  PATCH_SIZE  * PATCH_SIZE) + (plane_in.z * PATCH_SIZE * PATCH_SIZE);
	for(unsigned int i = 1; i < PATCH_SIZE; i++) {
		for (unsigned int j = 1; j < PATCH_SIZE; j++) {
			face triangle1 = {offset + ((i - 1) * PATCH_SIZE + (j - 1)),
			                  offset + (i * PATCH_SIZE + (j - 1)),
			                  offset + ((i - 1) * PATCH_SIZE + j)};
			mesh.push_back(triangle1);
			
			face triangle2 = {offset + (i * PATCH_SIZE + (j - 1)),
			                  offset + (i * PATCH_SIZE + j),
			                  offset + ((i - 1) * PATCH_SIZE + j)};
			mesh.push_back(triangle2);
		}
	}
	
	// Generate indices for downward facing triangles
	for(unsigned int i = 1; i < PATCH_SIZE; i++) {
		for (unsigned int j = 1; j < PATCH_SIZE; j++) {
			face triangle1 = {offset + ((i - 1) * PATCH_SIZE + j),
			                  offset + (i * PATCH_SIZE + (j - 1)),
			                  offset + ((i - 1) * PATCH_SIZE + (j - 1))};
			mesh.push_back(triangle1);
			
			face triangle2 = {offset + ((i - 1) * PATCH_SIZE + j),
			                  offset + (i * PATCH_SIZE + j),
			                  offset + (i * PATCH_SIZE + (j - 1))};
			mesh.push_back(triangle2);
		}
	}
}

void WaterSurface::updateBuffers() {
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


bool WaterSurface::update(Scene &scene, float dt) {
	auto time = (float) glfwGetTime();
	
	
	for (int i = 0; i < len_x; i ++){
		for (int u = 0; u < len_z; u++){
			
			for (int z_coord = 0; z_coord < 4; z_coord++){
				for (int x_coord = 0; x_coord < 4; x_coord++){
					if(u == 0 && z_coord == 0) {}
					else if (u == (len_z-1) && z_coord == 3){}
					else if (i == 0 && x_coord == 0){}
					else if (i == (len_x-1) && x_coord == 3){}
					else{
						
						if ((z_coord == 0 && u > 0) || (x_coord == 0 && i > 0)) {
							if (z_coord == 0 && u > 0) {
								water_planes[u + (i * len_z)].cPoints[z_coord][x_coord].y =
										water_planes[(u - 1) + (i * len_z)].cPoints[3][x_coord].y;
							}
							if (x_coord == 0 && i > 0) {
								water_planes[u + (i * len_z)].cPoints[z_coord][x_coord].y =
										water_planes[(u + ((i-1) * len_z))].cPoints[z_coord][3].y;
							}
						}
						else {
							water_planes[u + (i * len_z)].cPoints[z_coord][x_coord].y =
									sin(time) * water_planes[u + (i * len_z)].heights[z_coord][x_coord];
						}
					}
				}
			}
		}
	}

    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &tbo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
	
	vertices.clear();
	texCoords.clear();
	mesh.clear();
	
	// Generate Bezier suface
	for (int wp_idx = 0; wp_idx < water_planes.size(); wp_idx++){
		bezierPatches(water_planes[wp_idx]);
	}
	updateBuffers();
	
	generateModelMatrix();
	return true;
}

void WaterSurface::render(Scene &scene) {
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

void WaterSurface::addChild(Object *s) {
	s->parent = this;
	children.push_back(s);
}


