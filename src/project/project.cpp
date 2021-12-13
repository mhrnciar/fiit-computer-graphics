/*
 * Project - Underwater World
 * Matej Hrnciar, Leonard Puskac
 *
 * After start of program, camera is set to allow free movement. After pressing C, animation starts during which
 * keyboard and mouse input is ignored. Animation can be interrupted by pressing C again or paused by pressing P.
 * Scene cen be reset by pressing R
 */

#include <iostream>
#include <map>
#include <list>

#include <ppgso/ppgso.h>
#include <shaders/convolution_vert_glsl.h>
#include <shaders/convolution_frag_glsl.h>
#include <shaders/grayscale_vert_glsl.h>
#include <shaders/grayscale_frag_glsl.h>
#include <random>

#include "scene.h"
#include "camera.h"
#include "background.h"
#include "static_object.h"
#include "animated/whale/whale.h"
#include "animated/foliage.h"
#include "animated/shark.h"
#include "animated/fish_chased.h"
#include "animated/boids.h"
#include "animated/seagulls.h"
#include "water_surface.h"
#include "kelp.h"
#include "particle.h"
#include "particle_emitter.h"

#define FILTER true

const unsigned int SIZEW = 1280;
const unsigned int SIZEH = 720;

float randfloat(float min, float max)
{
	float range = (max - min);
	float div = RAND_MAX / range;
	return min + (rand() / div);
}

/*!
 * Custom windows for our simple game
 */
class SceneWindow : public ppgso::Window {
private:
    Scene scene;
    bool animate = true;

    /*!
     * Reset and initialize the game scene
     * Creating unique smart pointers to objects that are stored in the scene object list
     */
    void initScene() {
        scene.objects.clear();

        // Create a camera
        auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 400.0f);
        scene.camera = move(camera);

        printf("\nGenerating kelp forest...\n");
	    std::default_random_engine generator;
	    std::normal_distribution<float> normal_dist;

        bool kelp_forest_enabled = true;
	    if (kelp_forest_enabled) {
		    float kelp_x_offset, kelp_z_offset;
		    int rand_kelp_height;

		    // Staring location of kelp forest
		    float kelp_forrest_x = 35.0f;
		    float kelp_forrest_z = 35.0f;
		    float kelp_forrest_height = -1.65f;

		    // width
		    for (int i = 0; i < 15; i++) {
		        // length
			    for (int u = 0; u < 7; u++) {
				    kelp_x_offset = normal_dist(generator) * 0.3f;
				    kelp_z_offset = normal_dist(generator) * 0.3f;
				    rand_kelp_height = rand() % 4 + 3;

				    float sf = randfloat(0.25, 0.75f);

				    auto kelp = std::make_unique<Kelp>("seaweed_tex.png", rand_kelp_height, 4 * sf);
				    kelp->position = {
						    kelp_forrest_x + (i * 1.5f) + kelp_x_offset,
						    kelp_forrest_height,
						    kelp_forrest_z + (u * 1.5f) + kelp_z_offset
				    };

				    kelp->scale = {sf, sf, sf};
				    kelp->create_children();
				    scene.objects.push_back(move(kelp));
			    }
		    }
	    }
	    else {
	    	printf("Kelp forest disabled!\n");
	    }

	    glm::vec3 unified_volcano_position = {50.0f, -2.3f, -30.0f};
	    glm::vec3 unified_volcano_scale = {15, 15, 15};
	    glm::vec3 unified_volcano_rotation = {0.0f, 0.0f, ppgso::PI};

        printf("Generating volcano...\n");
	    auto volcano_rock = std::make_unique<StaticObject>("objects/volcano_rock_only.obj", "objects/sand.bmp", LIGHT_SHADER);
	    volcano_rock->scale = unified_volcano_scale;
	    volcano_rock->position = unified_volcano_position;
	    volcano_rock->rotation = unified_volcano_rotation;
	    scene.objects.push_back(move(volcano_rock));

	    auto volcano_lava = std::make_unique<StaticObject>("objects/volcano_lava_only.obj", "objects/lava_tex.bmp", LIGHT_SHADER);
	    volcano_lava->scale = unified_volcano_scale;
	    volcano_lava->position = unified_volcano_position;
	    volcano_lava->rotation = unified_volcano_rotation;
	    scene.objects.push_back(move(volcano_lava));

	    glm::vec3 p_vel = {0.5f,5.5f,-0.5f};
	    glm::vec3 p_scale = {7.0f,5.0f ,7.0f};
	    auto p_emitter = std::make_unique<ParticleEmitter>(unified_volcano_position,
														"smoke_tex.png",
														3.0f,
														1,
														p_vel,
														p_scale,
														0.4f,
														10.0f,
														1.0f);
	    scene.objects.push_back(move(p_emitter));

	    // Volcano lights
        scene.lights.push_back({{50.0f, 8.0f, -30.0f}, {1.0f, 0.0f, 0.0f}, 0.045, 0.0075});
        scene.lights.push_back({{45.0f, 0.7f, -20.5f}, {1.0f, 0.0f, 0.0f}, 0.22, 0.20});
        scene.lights.push_back({{48.0f, 2.2f, -22.5f}, {1.0f, 0.0f, 0.0f}, 0.22, 0.20});
        scene.lights.push_back({{49.0f, 5.0f, -24.0f}, {1.0f, 0.0f, 0.0f}, 0.22, 0.20});
        scene.lights.push_back({{53.5f, -1.0f, -17.0f}, {1.0f, 0.0f, 0.0f}, 0.22, 0.20});
        scene.lights.push_back({{56.0f, 0.5f, -21.5f}, {1.0f, 0.0f, 0.0f}, 0.22, 0.20});
        scene.lights.push_back({{56.0f, 2.5f, -24.0f}, {1.0f, 0.0f, 0.0f}, 0.22, 0.20});
        scene.lights.push_back({{54.0f, 4.5f, -26.0f}, {1.0f, 0.0f, 0.0f}, 0.22, 0.20});

        printf("Generating environment...\n");
        auto water_surface = std::make_unique<WaterSurface>("water_seamless.bmp", 21, 21);
        water_surface->position = {-125, 88, -125};
        water_surface->scale = {4,3,4};
	    scene.objects.push_back(move(water_surface));

        auto skydome = std::make_unique<Background>("objects/skydome.obj", "objects/skydome.png");
        skydome->scale = {125.0f, 125.0f, 125.0f};
        skydome->position = {0, 60, 0};
        scene.objects.push_back(move(skydome));

        auto background = std::make_unique<Background>("objects/bg.obj", "objects/sea1.png");
        background->scale = {30.0f, 30.0f, 30.0f};
        background->position = {0, 30, 0};
        background->rotation = {0, 0, ppgso::PI/4};
        scene.objects.push_back(move(background));

        auto seabed = std::make_unique<StaticObject>("objects/seabed.obj", "objects/sand.bmp", LIGHT_SHADER);
        seabed->scale = {1.5f, 1.0f, 1.5f};
        scene.objects.push_back(move(seabed));

        auto seagulls = std::make_unique<Seagulls>();
        seagulls->position = {0, 100, 0};
        scene.objects.push_back(move(seagulls));

        printf("Generating coral cave...\n");
        auto cave = std::make_unique<StaticObject>("objects/cave.obj", "objects/rock_bg.bmp", LIGHT_SHADER);
        cave->position = {-7.0f, 7.7f, 0.0f};
        cave->scale = {2.0f, 3.0f, 2.0f};

        auto coral = new StaticObject("corals/coral.obj", "corals/coral_green2.bmp", LIGHT_SHADER);
        coral->scale = {1.0f, 2.0f, 1.0f};
        coral->position = {0.0f, -3.2f, -7.8f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral.obj", "corals/coral_green2.bmp", LIGHT_SHADER);
        coral->scale = {2.0f, 2.0f, 2.0f};
        coral->position = {5.0f, -3.3f, -3.6f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral.obj", "corals/coral_green.bmp", LIGHT_SHADER);
        coral->position = {-4.0f, -3.3f, -3.8f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral1.obj", "corals/coral_red.bmp", LIGHT_SHADER);
        coral->rotation = {ppgso::PI, 0.0f, 0.0f};
        coral->position = {0.57f, 2.2f, -7.0f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral1.obj", "corals/coral_yellow.bmp", LIGHT_SHADER);
        coral->scale = {1.5f, 1.5f, 1.5f};
        coral->rotation = {0.0f, 0.0f, ppgso::PI/2};
        coral->position = {2.0f, -3.5f, -4.5f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral1.obj", "corals/coral_pink.bmp", LIGHT_SHADER);
        coral->position = {-1.0f, -3.4f, -7.0f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral2.obj", "corals/coral_blue.bmp", LIGHT_SHADER);
        coral->scale = {2.5f, 2.5f, 2.5f};
        coral->position = {2.0f, -3.3f, -7.5f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral2.obj", "corals/coral_pink.bmp", LIGHT_SHADER);
        coral->rotation = {-ppgso::PI/2 + 0.4f, 0.0f, 0.0f};
        coral->position = {0.0f, -0.5f, -3.8f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral2.obj", "corals/coral_blue.bmp", LIGHT_SHADER);
        coral->scale = {1.2f, 1.2f, 1.2f};
        coral->rotation = {ppgso::PI + 0.2f, 0.0f, 0.0f};
        coral->position = {0.0f, 2.5f, -5.5f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral3.obj", "corals/coral_purple.bmp", LIGHT_SHADER);
        coral->scale = {1.2f, 1.2f, 1.2f};
        coral->position = {3.5f, -3.2f, -7.2f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral4.obj", "corals/coral_orange.bmp", LIGHT_SHADER);
        coral->scale = {1.2f, 1.2f, 1.2f};
        coral->rotation = {ppgso::PI + 0.1f, 0.0f, 0.0f};
        coral->position = {1.0f, 2.5f, -4.6f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral4.obj", "corals/coral_red.bmp", LIGHT_SHADER);
        coral->scale = {1.5f, 1.5f, 1.5f};
        coral->rotation = {0.0f, 0.0f, 3.0f};
        coral->position = {0.7f, -3.3f, -6.5f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral5.obj", "corals/coral_green2.bmp", LIGHT_SHADER);
        coral->position = {0.0f, -3.3f, -4.2f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral5.obj", "corals/coral_green.bmp", LIGHT_SHADER);
        coral->scale = {2.0f, 2.0f, 2.0f};
        coral->position = {-4.0f, -3.3f, -7.2f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral5.obj", "corals/coral_green.bmp", LIGHT_SHADER);
        coral->scale = {0.7f, 0.7f, 0.7f};
        coral->position = {2.5f, -3.2f, -6.7f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral6.obj", "corals/coral_blue.bmp", LIGHT_SHADER);
        coral->scale = {1.5f, 1.5f, 1.5f};
        coral->position = {-2.0f, -3.3f, -4.5f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral6.obj", "corals/coral_red.bmp", LIGHT_SHADER);
        coral->scale = {1.5f, 1.5f, 1.5f};
        coral->rotation = {0.0f, 0.0f, 1.0f};
        coral->position = {3.5f, -3.3f, -4.0f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral6.obj", "corals/coral_pink.bmp", LIGHT_SHADER);
        coral->rotation = {ppgso::PI, 0.4f, 0.0f};
        coral->position = {-2.0f, 2.4f, -5.6f};
        cave->addChild(coral);

        scene.objects.push_back(move(cave));

        // Coral lights
        scene.lights.push_back({{-4.78f, 6.0f, -7.5f}, {0.0f, 0.0f, 1.0f}, 0.7, 1.8});
        scene.lights.push_back({{-7.87f, 5.2f, -7.0f}, {0.6f, 0.0f, 1.0f}, 0.7, 1.8});
        scene.lights.push_back({{-6.06f, 8.5f, -4.85f}, {1.0f, 0.0f, 0.0f}, 0.7, 1.8});
        scene.lights.push_back({{-6.35f, 8.15f, -7.26f}, {1.0f, 0.0f, 0.0f}, 0.7, 1.8});
        scene.lights.push_back({{-4.85f, 5.2f, -4.47f}, {0.9f, 0.8f, 0.0f}, 0.7, 1.8});
        scene.lights.push_back({{-7.09f, 5.0f, -4.1f}, {0.0f, 1.0f, 0.0f}, 0.7, 1.8});
        scene.lights.push_back({{-1.9f, 5.2f, -3.62f}, {0.0f, 1.0f, 0.0f}, 0.7, 1.8});
        scene.lights.push_back({{-6.88f, 8.86f, -5.66f}, {0.0f, 0.0f, 1.0f}, 0.7, 1.8});
        scene.lights.push_back({{-3.5f, 5.2f, -7.14f}, {0.6f, 0.0f, 1.0f}, 0.7, 1.8});

        printf("Generating whale, boids and foliage...\n");
        auto whale = std::make_unique<Whale>();
        whale->position = {20, 35, -20};
        scene.objects.push_back(move(whale));

        auto boids = std::make_unique<Boids>(glm::vec3{20,25,20}, glm::vec3{0,0,0});
        scene.objects.push_back(move(boids));

        auto upper_boids = std::make_unique<Boids>(glm::vec3{-53,70,-25}, glm::vec3{0,0,ppgso::PI/2});
        scene.objects.push_back(move(upper_boids));

        auto foliage = std::make_unique<Foliage>(-25.0f, 20.0f, -25.0f, 20.0f);
        scene.objects.push_back(move(foliage));

        printf("Generating shipwreck...\n");
        auto ship = std::make_unique<StaticObject>("objects/shipwreck.obj", "objects/ship.png", LIGHT_SHADER);
        ship->position = {58.0f, -1.5f, 63.0f};
        ship->rotation = {0, 0, -ppgso::PI/4};
        ship->scale = {3.0f, 3.0f, 3.0f};
        scene.objects.push_back(move(ship));
    }

public:
    // Postprocessing filters
    ppgso::Shader convQuadShader = {convolution_vert_glsl, convolution_frag_glsl};
    ppgso::Shader grayQuadShader = {grayscale_vert_glsl, grayscale_frag_glsl};
    ppgso::Mesh quadMesh = {"quad.obj"};
    ppgso::Texture quadTexture = {SIZEW, SIZEH};

    // OpenGL object ids for framebuffer and render buffer
    GLuint fbo = 0;
    GLuint rbo = 0;
    /*!
     * Construct custom game window
     */
    SceneWindow() : Window{"project", SIZEW, SIZEH} {
        //hideCursor();
        glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

        // Initialize OpenGL state
        // Enable Z-buffer
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        // Enable polygon culling
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
        //disables cursor and binds mouse to window
	    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	    if (FILTER) {
            // Disable mipmapping on the quadTexture
            quadTexture.bind();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            // Initialize framebuffer, its color texture (the sphere will be rendered to it) and its render buffer for depth info storage
            glGenFramebuffers(1, &fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);

            // Set up render buffer that has a depth buffer and stencil buffer
            glGenRenderbuffers(1, &rbo);
            glBindRenderbuffer(GL_RENDERBUFFER, rbo);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

            // Associate the quadTexture with it
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, quadTexture.image.width, quadTexture.image.height);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, quadTexture.getTexture(), 0);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                throw std::runtime_error("Cannot create framebuffer!");
            }
	    }

        initScene();
    }

    /*!
     * Generate objects that are animated using keyframes so the timing with camera animation is right
     */
    void initAnimation() {
        auto shark = std::make_unique<Shark>();
        scene.objects.push_back(move(shark));

        auto chased_fish = std::make_unique<ChasedFish>();
        scene.objects.push_back(move(chased_fish));
    }

    /*!
     * Handles pressed key when the window is focused
     * @param key Key code of the key being pressed/released
     * @param scanCode Scan code of the key being pressed/released
     * @param action Action indicating the key state change
     * @param mods Additional modifiers to consider
     */
    void onKey(int key, int scanCode, int action, int mods) override {
        scene.keyboard[key] = action;

        // Reset
        if (key == GLFW_KEY_R && action == GLFW_PRESS) {
            initScene();
        }

        // Pause
        if (key == GLFW_KEY_P && action == GLFW_PRESS) {
            animate = !animate;
        }

        if (key == GLFW_KEY_C && action == GLFW_PRESS) {
            if (scene.camera->keyframes.empty()) {
                printf("Starting animation...\n");
                initAnimation();
                scene.camera->initCameraAnimation();
            }
            else {
                scene.camera->keyframes.clear();
            }
        }
        
    }

    /*!
     * Handle cursor position changes
     * @param cursorX Mouse horizontal position in window coordinates
     * @param cursorY Mouse vertical position in window coordinates
     */
    void onCursorPos(double cursorX, double cursorY) override {
    	scene.cursor.x = cursorX;
    	scene.cursor.y = cursorY;

    	if (scene.camera->keyframes.empty())
    	    scene.camera->updateDir(scene);
    }

    /*!
     * Handle cursor buttons
     * @param button Mouse button being manipulated
     * @param action Mouse bu
     * @param mods
     */
    void onMouseButton(int button, int action, int mods) override {
        if(button == GLFW_MOUSE_BUTTON_LEFT) {
            scene.cursor.left = action == GLFW_PRESS;

            if (scene.cursor.left) {
                // Convert pixel coordinates to Screen coordinates
                double u = (scene.cursor.x / width - 0.5f) * 2.0f;
                double v = - (scene.cursor.y / height - 0.5f) * 2.0f;

                // Get mouse pick vector in world coordinates
                auto direction = scene.camera->cast(u, v);
                auto position = scene.camera->cameraPosition;

                // Get all objects in scene intersected by ray
                auto picked = scene.intersect(position, direction);

                // Go through all objects that have been picked
                for (auto &obj: picked) {
                    // Pass on the click event
                    obj->onClick(scene);
                }
            }
        }
        if(button == GLFW_MOUSE_BUTTON_RIGHT) {
            scene.cursor.right = action == GLFW_PRESS;
        }
    }

    /*!
     * Window update implementation that will be called automatically from pollEvents
     */
    void onIdle() override {
        // Track time
        static auto time = (float) glfwGetTime();

        // Compute time delta
        float dt = animate ? (float) glfwGetTime() - time : 0;

        time = (float) glfwGetTime();

        if (FILTER) {
            glViewport(0, 0, SIZEW, SIZEH);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        }

        // Clear the framebuffer
        glClearColor(.5f, .5f, .5f, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update and render all objects
        scene.update(dt);
        scene.render();

        if (FILTER) {
            resetViewport();
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // Clear the framebuffer
            glClearColor(.5f, .5f, .5f, 0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Create projection matrix (field of view (radians), aspect ratio, near plane distance, far plane distance)
            auto quadProjectionMatrix = glm::perspective((ppgso::PI / 180.f) * 60.0f, 1.0f, 0.1f, 10.0f);

            // Create view matrix (translate camera backwards a bit, so we can see the geometry)
            auto quadViewMatrix = glm::translate(glm::mat4{1.0f}, {0.0f, 0.0f, -1.7f});

            // Animate rotation of the quad
            auto quadModelMatrix = glm::mat4{1.0f};

            // If camera is above water, use grayscale filter
            if (scene.camera->cameraPosition.y > 88) {
                // Set shader inputs
                grayQuadShader.use();
                grayQuadShader.setUniform("ProjectionMatrix", quadProjectionMatrix);
                grayQuadShader.setUniform("ViewMatrix", quadViewMatrix);
                grayQuadShader.setUniform("ModelMatrix", quadModelMatrix);
                grayQuadShader.setUniform("Texture", quadTexture);
            }
            // Otherwise use convolution gaussian blur filter
            else {
                // Set shader inputs
                convQuadShader.use();
                convQuadShader.setUniform("ProjectionMatrix", quadProjectionMatrix);
                convQuadShader.setUniform("ViewMatrix", quadViewMatrix);
                convQuadShader.setUniform("ModelMatrix", quadModelMatrix);
                convQuadShader.setUniform("Texture", quadTexture);
            }

            quadMesh.render();
        }
    }
};

int main() {
    // Initialize our window
    SceneWindow window;

    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}
