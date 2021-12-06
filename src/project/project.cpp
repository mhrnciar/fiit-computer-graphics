// Example gl_scene
// - Introduces the concept of a dynamic scene of objects
// - Uses abstract object interface for Update and Render steps
// - Creates a simple game scene with Player, Asteroid and Space objects
// - Contains a generator object that does not render but adds Asteroids to the scene
// - Some objects use shared resources and all object deallocations are handled automatically
// - Controls: LEFT, RIGHT, "R" to reset, SPACE to fire

#include <iostream>
#include <map>
#include <list>

#include <ppgso/ppgso.h>
#include <random>

#include "scene.h"
#include "camera.h"
#include "background.h"
#include "static_object.h"
#include "cube.h"
#include "animated/whale/whale.h"
#include "animated/foliage.h"
#include "src/project/animated/shark.h"
#include "src/project/animated/fish_chased.h"
#include "src/project/animated/boids.h"
#include "water_surface.h"
#include "kelp.h"

const unsigned int SIZEW = 1280;
const unsigned int SIZEH = 720;

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

	    std::default_random_engine generator;
	    std::normal_distribution<float> normal_dist;

        bool kelp_forrest_enabled = true;
	    if (kelp_forrest_enabled) {
		    float kelp_x_offset, kelp_z_offset;
		    int rand_kelp_height;
		    float kelp_forrest_x = 35.0f;
		    float kelp_forrest_z = 35.0f;
		    float kelp_forrest_height = -1.65f;
		    for (int i = 0; i < 10; i++) {
			    for (int u = 0; u < 10; u++) {
				    kelp_x_offset = normal_dist(generator) * 0.3f;
				    kelp_z_offset = normal_dist(generator) * 0.3f;
				    rand_kelp_height = rand() % 4 + 3;
				    auto kelp = std::make_unique<Kelp>("seaweed_tex.png", rand_kelp_height);
				    kelp->position = {
						    kelp_forrest_x + (i * 1.2f) + kelp_x_offset,
						    kelp_forrest_height,
						    kelp_forrest_z + (u * 1.2f) + kelp_z_offset
				    };
				    kelp->scale = {0.5f, 0.5f, 0.5f};
				    kelp->create_children();
				    scene.objects.push_back(move(kelp));
			    }
		    }
	    }

	    glm::vec3 unified_volcano_position = {50.0f, -2.3f, -30.0f};
	    glm::vec3 unified_volcano_scale = {15, 15, 15};
	    glm::vec3 unified_volcano_rotation = {0.0f, 0.0f, ppgso::PI};


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


        auto water_surface = std::make_unique<WaterSurface>("water_seamless.bmp", 19, 19);
        water_surface->position = {-80, 53, -80};
        water_surface->scale = {3,2,3};
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

        auto axisX = std::make_unique<Cube>(glm::vec3{1, 0, 0});
        auto axisY = std::make_unique<Cube>(glm::vec3{0, 1, 0});
        auto axisZ = std::make_unique<Cube>(glm::vec3{0, 0, 1});

        const float scaleMin = 0.1f;
        const float scaleMax = 100.00f;

        axisX->scale = {scaleMax, scaleMin, scaleMin};
        axisY->scale = {scaleMin, scaleMax, scaleMin};
        axisZ->scale = {scaleMin, scaleMin, scaleMax};
        scene.objects.push_back(move(axisX));
        scene.objects.push_back(move(axisY));
        scene.objects.push_back(move(axisZ));

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

        scene.lights.push_back({{-4.78f, 6.0f, -7.5f}, {0.0f, 0.0f, 1.0f}, 0.7, 1.8});
        scene.lights.push_back({{-7.87f, 5.2f, -7.0f}, {0.6f, 0.0f, 1.0f}, 0.7, 1.8});
        scene.lights.push_back({{-6.06f, 8.5f, -4.85f}, {1.0f, 0.0f, 0.0f}, 0.7, 1.8});
        scene.lights.push_back({{-6.35f, 8.15f, -7.26f}, {1.0f, 0.0f, 0.0f}, 0.7, 1.8});
        scene.lights.push_back({{-4.85f, 5.2f, -4.47f}, {0.9f, 0.8f, 0.0f}, 0.7, 1.8});
        scene.lights.push_back({{-7.09f, 5.0f, -4.1f}, {0.0f, 1.0f, 0.0f}, 0.7, 1.8});
        scene.lights.push_back({{-1.9f, 5.2f, -3.62f}, {0.0f, 1.0f, 0.0f}, 0.7, 1.8});
        scene.lights.push_back({{-6.88f, 8.86f, -5.66f}, {0.0f, 0.0f, 1.0f}, 0.7, 1.8});
        scene.lights.push_back({{-3.5f, 5.2f, -7.14f}, {0.6f, 0.0f, 1.0f}, 0.7, 1.8});
        scene.lights.push_back({{0, 25, 0}, {1.0f, 1.0f, 1.0f}, 0.07, 0.017});

        auto whale = std::make_unique<Whale>();
        whale->position = {20, 20, -20};
        scene.objects.push_back(move(whale));

        auto shark = std::make_unique<Shark>();
        scene.objects.push_back(move(shark));

        auto chased_fish = std::make_unique<ChasedFish>();
        scene.objects.push_back(move(chased_fish));

        auto boids = std::make_unique<Boids>(glm::vec3{10,15,10}, glm::vec3{0,0,0});
        scene.objects.push_back(move(boids));

        auto foliage = std::make_unique<Foliage>(-25.0f, 20.0f, -25.0f, 20.0f);
        scene.objects.push_back(move(foliage));

        /* Long loading, so put in comments while modeling other things
        mesh = "objects/shipwreck.obj";
        tex = "objects/ship.png";
        auto ship = std::make_unique<Background>(mesh, tex);
        ship->position = {45.0f, -2.1f, 0.0f};
        scene.objects.push_back(move(ship));
         */
    }

public:
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

        initScene();
    }

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

        // Set gray background
        glClearColor(.5f, .5f, .5f, 0);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // Clear depth and color buffers
        glClear(GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, 1024, 1024);
/*

        scene.renderShadows();
*/
        glViewport(0, 0, SIZEW, SIZEH);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update and render all objects
        scene.update(dt);
        scene.render();
    }
};

int main() {
    // Initialize our window
    SceneWindow window;

    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}
