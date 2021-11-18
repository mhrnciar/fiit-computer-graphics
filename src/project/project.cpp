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

#include "camera.h"
#include "scene.h"
#include "static_object.h"
#include "rectangle.h"
#include "cube.h"

const unsigned int SIZE = 512;

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
        auto camera = std::make_unique<Camera>(60.0f, 1.0f, 0.1f, 100.0f);
        //camera->cameraPosition.z = -15.0f;
        scene.camera = move(camera);

        std::string mesh = "seabed.obj";
        std::string tex = "sand.bmp";

        auto seabed = std::make_unique<StaticObject>(mesh, tex, LIGHT_SHADER);
        scene.objects.push_back(move(seabed));

        auto axisX = std::make_unique<Cube>(glm::vec3{1, 0, 0});
        auto axisY = std::make_unique<Cube>(glm::vec3{0, 1, 0});
        auto axisZ = std::make_unique<Cube>(glm::vec3{0, 0, 1});

        const float scaleMin = 0.1f;
        const float scaleMax = 60.00f;

        axisX->scale = {scaleMax, scaleMin, scaleMin};
        axisY->scale = {scaleMin, scaleMax, scaleMin};
        axisZ->scale = {scaleMin, scaleMin, scaleMax};
        scene.objects.push_back(move(axisX));
        scene.objects.push_back(move(axisY));
        scene.objects.push_back(move(axisZ));

        mesh = "cave.obj";
        tex = "rock_bg.bmp";
        auto rock = std::make_unique<StaticObject>(mesh, tex, LIGHT_SHADER);
        rock->position = {2.2f, 3.2f, 2.0f};
        //rock->scale = {0.1f, 0.1f, 0.1f};
        scene.objects.push_back(move(rock));
    }

public:
    /*!
     * Construct custom game window
     */
    SceneWindow() : Window{"project", SIZE, SIZE} {
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
        
    }

    /*!
     * Handle cursor position changes
     * @param cursorX Mouse horizontal position in window coordinates
     * @param cursorY Mouse vertical position in window coordinates
     */
    void onCursorPos(double cursorX, double cursorY) override {
    	scene.cursor.x = cursorX;
    	scene.cursor.y = cursorY;
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
