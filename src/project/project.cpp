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

#include "scene.h"
#include "camera.h"
#include "background.h"
#include "static_object.h"
#include "bezier_object.h"
#include "cube.h"
#include "whale.h"

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
        auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 200.0f);
        scene.camera = move(camera);

        auto skydome = std::make_unique<Background>("objects/skydome.obj", "objects/skydome.png");
        skydome->scale = {81, 81, 81};
        skydome->position = {0, 45, 0};
        scene.objects.push_back(move(skydome));

        auto background = std::make_unique<Background>("objects/bg.obj", "objects/sea1.png");
        background->scale = {20, 20, 20};
        background->position = {0, 20, 0};
        scene.objects.push_back(move(background));

        auto water = std::make_unique<StaticObject>("plane.obj", "water_seamless.bmp", TEXTURE_SHADER);
        water->scale = {80,80,80};
        water->position = {0, 55, 0};
        water->rotation = {ppgso::PI/2, 0, 0};
        scene.objects.push_back(move(water));

        auto seabed = std::make_unique<StaticObject>("objects/seabed.obj", "objects/sand.bmp", LIGHT_SHADER);
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
        cave->position = {-7.0f, 7.0f, 0.0f};
        cave->scale = {2, 2.5f, 2};

        auto coral = new StaticObject("corals/coral.obj", "corals/coral_green2.bmp", LIGHT_SHADER);
        coral->scale = {1.0f, 2.0f, 1.0f};
        coral->position = {0.0f, -2.7f, -7.8f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral.obj", "corals/coral_green2.bmp", LIGHT_SHADER);
        coral->scale = {2.0f, 2.0f, 2.0f};
        coral->position = {5.0f, -2.8f, -3.6f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral.obj", "corals/coral_green.bmp", LIGHT_SHADER);
        coral->position = {-4.0f, -2.8f, -3.8f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral1.obj", "corals/coral_red.bmp", LIGHT_SHADER);
        coral->rotation = {ppgso::PI, 0.0f, 0.0f};
        coral->position = {0.57f, 1.8f, -7.0f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral1.obj", "corals/coral_yellow.bmp", LIGHT_SHADER);
        coral->scale = {1.5f, 1.5f, 1.5f};
        coral->rotation = {0.0f, 0.0f, ppgso::PI/2};
        coral->position = {2.0f, -2.8f, -4.5f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral1.obj", "corals/coral_pink.bmp", LIGHT_SHADER);
        coral->position = {-1.0f, -2.8f, -7.0f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral2.obj", "corals/coral_blue.bmp", LIGHT_SHADER);
        coral->scale = {2.5f, 2.5f, 2.5f};
        coral->position = {2.0f, -2.8f, -7.5f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral2.obj", "corals/coral_pink.bmp", LIGHT_SHADER);
        coral->rotation = {-ppgso::PI/2 + 0.4f, 0.0f, 0.0f};
        coral->position = {0.0f, -0.5f, -3.8f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral2.obj", "corals/coral_blue.bmp", LIGHT_SHADER);
        coral->scale = {1.2f, 1.2f, 1.2f};
        coral->rotation = {ppgso::PI + 0.2f, 0.0f, 0.0f};
        coral->position = {0.0f, 2.3f, -5.5f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral3.obj", "corals/coral_purple.bmp", LIGHT_SHADER);
        coral->scale = {1.2f, 1.2f, 1.2f};
        coral->position = {3.5f, -2.4f, -7.2f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral4.obj", "corals/coral_orange.bmp", LIGHT_SHADER);
        coral->scale = {1.2f, 1.2f, 1.2f};
        coral->rotation = {ppgso::PI + 0.1f, 0.0f, 0.0f};
        coral->position = {1.0f, 2.2f, -4.6f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral4.obj", "corals/coral_red.bmp", LIGHT_SHADER);
        coral->scale = {1.5f, 1.5f, 1.5f};
        coral->rotation = {0.0f, 0.0f, 3.0f};
        coral->position = {0.7f, -2.7f, -6.5f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral5.obj", "corals/coral_green2.bmp", LIGHT_SHADER);
        coral->position = {0.0f, -2.8f, -4.2f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral5.obj", "corals/coral_green.bmp", LIGHT_SHADER);
        coral->scale = {2.0f, 2.0f, 2.0f};
        coral->position = {-4.0f, -2.8f, -7.2f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral5.obj", "corals/coral_green.bmp", LIGHT_SHADER);
        coral->scale = {0.7f, 0.7f, 0.7f};
        coral->position = {2.5f, -2.7f, -6.7f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral6.obj", "corals/coral_blue.bmp", LIGHT_SHADER);
        coral->scale = {1.5f, 1.5f, 1.5f};
        coral->position = {-2.0f, -2.8f, -4.5f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral6.obj", "corals/coral_red.bmp", LIGHT_SHADER);
        coral->scale = {1.5f, 1.5f, 1.5f};
        coral->rotation = {0.0f, 0.0f, 1.0f};
        coral->position = {3.5f, -2.8f, -4.0f};
        cave->addChild(coral);

        coral = new StaticObject("corals/coral6.obj", "corals/coral_pink.bmp", LIGHT_SHADER);
        coral->rotation = {ppgso::PI, 0.4f, 0.0f};
        coral->position = {-2.0f, 2.0f, -5.6f};
        cave->addChild(coral);

        scene.objects.push_back(move(cave));

        auto whale = std::make_unique<Whale>();
        scene.objects.push_back(move(whale));

        /* Algae
        for (int i = 0; i < 100; i++) {
            auto algae = std::make_unique<BezierObject>("green_algae.png");
            algae->position = {glm::linearRand(0.0f, -12.0f), 4.5f, glm::linearRand(-8.0f, -4.0f)};
            algae->rotation = {0, 0, -ppgso::PI/2};
            algae->scale = {0.1f, 0.1f, 0.1f};
            scene.objects.push_back(move(algae));
        }
        */
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
		
        //disables cursor and binds mouse to window TODO: move this to ppgso::window (?)
	    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        initScene();
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

        if (key == GLFW_KEY_O && action == GLFW_PRESS) {
            auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 200.0f);
            camera->keyframes.push_back({{7.2,6.6,-6}, {7.2,20,-6}, {0, 0, -1}, {0, 0, -1}, 10});
            camera->keyframes.push_back({{7.2,20,-6}, {7.2,10,-6}, {0, 0, -1}, {-1, 0, 0}, 10});
            camera->keyframes.push_back({{7.2,10,-6}, {7.2,10,-6}, {-1, 0, 0}, {-1, 0, 0}, 10});
            camera->keyframes.push_back({{7.2,10,-6}, {7.2,10,-15}, {-1, 0, 0}, {0, 0, 1}, 10});
            scene.camera = move(camera);
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
