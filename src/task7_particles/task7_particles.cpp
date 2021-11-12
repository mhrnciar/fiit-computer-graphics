// Task 7 - Dynamically generate objects in a 3D scene
//        - Implement a particle system where particles have position and speed
//        - Any object can be a generator and can add objects to the scene
//        - Create dynamic effect such as fireworks, rain etc.
//        - Encapsulate camera in a class

#include <map>
#include <list>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <ppgso/ppgso.h>

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

const unsigned int SIZE = 512;

class Camera {
public:
    // TODO: Add parameters
    glm::vec3 up{0,1,0};
    glm::vec3 position{0,5,-5};
    glm::vec3 back{0,0,-1};

    glm::mat4 viewMatrix{1.0f};
    glm::mat4 projectionMatrix{1.0f};

    /// Representaiton of
    /// \param fov - Field of view (in degrees)
    /// \param ratio - Viewport ratio (width/height)
    /// \param near - Distance of the near clipping plane
    /// \param far - Distance of the far clipping plane
    Camera(float fov = 45.0f, float ratio = 1.0f, float near = 0.1f, float far = 10.0f) {
        // TODO: Initialize perspective projection (hint: glm::perspective)
        float fowInRad = (ppgso::PI/180.0f) * fov;
        projectionMatrix = glm::perspective(fowInRad, ratio, near, far);

        update();
    }

    /// Recalculate viewMatrix from position, rotation and scale
    void update() {
        // TODO: Update viewMatrix (hint: glm::lookAt)
        viewMatrix = lookAt(position, position-back, up);
    }
};

/// Abstract renderable object interface
class Renderable; // Forward declaration for Scene
using Scene = std::list<std::unique_ptr<Renderable>>; // Type alias

class Renderable {
public:
    glm::vec3 position{0, 0, 0};
    glm::vec3 scale{1, 1, 1};
    glm::vec3 speed{0, 0, 0};
    glm::vec3 color{1, 1, 1};

    // Virtual destructor is needed for abstract interfaces
    virtual ~Renderable() = default;

    /// Render the object
    /// \param camera - Camera to use for rendering
    virtual void render(const Camera& camera) = 0;

    /// Update the object. Useful for specifing animation and behaviour.
    /// \param dTime - Time delta
    /// \param scene - Scene reference
    /// \return - Return true to keep object in scene
    virtual bool update(float dTime, Scene &scene) = 0;
};

/// Basic particle that will render a sphere
/// TODO: Implement Renderable particle
class SnowParticle final : public Renderable {
    // Static resources shared between all particles
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;

    glm::mat4 modelMatrix{1.0f};
    float age{0.0f};

    // TODO: add more parameters as needed
public:
    /// Construct a new Particle
    /// \param p - Initial position
    /// \param s - Initial speed
    /// \param c - Color of particle
    SnowParticle() {
        // First particle will initialize resources
        if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");

        scale *= glm::linearRand(0.05f, 0.2f);
        position = {glm::linearRand(-20.0f, 20.0f), 20.0f, 0.0f};
        speed = {glm::linearRand(-2.0f, 2.0f), glm::linearRand(-5.0f, -10.0f), 0.0f};
        color = {1.0f, 1.0f, 1.0f};
    }

    bool update(float dTime, Scene &scene) override {
        // TODO: Animate position using speed and dTime.
        // - Return true to keep the object alive
        // - Returning false removes the object from the scene
        // - hint: you can add more particles to the scene here also
        age += dTime;

        // Animate position according to time
        position += speed * dTime;

        // Delete when alive longer than 10s or out of visibility
        if (age > 15.0f) return false;

        if (position.y < -1.0f)
            speed = glm::vec3{0.0f};

        modelMatrix = glm::translate(glm::mat4(1.0f), position)
                * glm::orientate4(glm::vec3{0.0f})
                * glm::scale(glm::mat4(1.0f), scale);

        return true;
    }

    void render(const Camera& camera) override {
        // TODO: Render the object
        // - Use the shader
        // - Setup all needed shader inputs
        // - hint: use OverallColor in the color_vert_glsl shader for color
        // - Render the mesh
        shader->use();

        // use camera
        shader->setUniform("ProjectionMatrix", camera.projectionMatrix);
        shader->setUniform("ViewMatrix", camera.viewMatrix);

        // render mesh
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("OverallColor", color);
        mesh->render();
    }
};

/// Basic particle that will render a sphere
/// TODO: Implement Renderable particle
class IceParticle final : public Renderable {
    // Static resources shared between all particles
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;

    glm::mat4 modelMatrix{1.0f};
    float age{0.0f};
    float bounces = 4;

    // TODO: add more parameters as needed
public:
    /// Construct a new Particle
    /// \param p - Initial position
    /// \param s - Initial speed
    /// \param c - Color of particle
    IceParticle() {
        // First particle will initialize resources
        if (!shader) shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
        if (!mesh) mesh = std::make_unique<ppgso::Mesh>("sphere.obj");

        scale *= glm::linearRand(0.05f, 0.2f);
        position = {glm::linearRand(-20.0f, 20.0f), 20.0f, 0.0f};
        speed = {glm::linearRand(-1.0f, 1.0f), glm::linearRand(-20.0f, -30.0f), 0.0f};
        color = {0.0f, 0.0f, 1.0f};
    }

    bool update(float dTime, Scene &scene) override {
        // TODO: Animate position using speed and dTime.
        // - Return true to keep the object alive
        // - Returning false removes the object from the scene
        // - hint: you can add more particles to the scene here also
        age += dTime;

        // Animate position according to time

        if (bounces < 4 && bounces >= 0){
            speed.y -= 0.04;
        }

        position += speed * dTime;

        // Delete when alive longer than 10s or out of visibility
        if (age > 15.0f) return false;

        if (position.y <= -0.8f) {
            if (bounces > 0){
                speed.y = speed.y * -0.5f;
                bounces--;
            }
            else{
                speed = {0,0,0};
            }
        }

        modelMatrix = glm::translate(glm::mat4(1.0f), position)
                * glm::orientate4(glm::vec3{0.0f})
                * glm::scale(glm::mat4(1.0f), scale);

        return true;
    }

    void render(const Camera& camera) override {
        // TODO: Render the object
        // - Use the shader
        // - Setup all needed shader inputs
        // - hint: use OverallColor in the color_vert_glsl shader for color
        // - Render the mesh
        shader->use();

        // use camera
        shader->setUniform("ProjectionMatrix", camera.projectionMatrix);
        shader->setUniform("ViewMatrix", camera.viewMatrix);

        // render mesh
        shader->setUniform("ModelMatrix", modelMatrix);
        shader->setUniform("OverallColor", color);
        mesh->render();
    }
};

// Static resources need to be instantiated outside of the class as they are globals
std::unique_ptr<ppgso::Mesh> SnowParticle::mesh;
std::unique_ptr<ppgso::Shader> SnowParticle::shader;
std::unique_ptr<ppgso::Mesh> IceParticle::mesh;
std::unique_ptr<ppgso::Shader> IceParticle::shader;

class ParticleWindow : public ppgso::Window {
private:
    // Scene of objects
    Scene scene;
    float dt = 0;

    // Create camera
    Camera camera = {120.0f, (float)width/(float)height, 1.0f, 400.0f};

    // Store keyboard state
    std::map<int, int> keys;
public:
    ParticleWindow() : Window{"task7_particles", SIZE, SIZE} {
        // Initialize OpenGL state
        // Enable Z-buffer
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
    }

    void onKey(int key, int scanCode, int action, int mods) override {
        // Collect key state in a map
        keys[key] = action;
        if (keys[GLFW_KEY_SPACE]) {
            // TODO: Add renderable object to the scene
            float r = glm::linearRand(0.0f, 1.0f);

            if(r > 0.2f) {
                for(int i = 0; i < 15; i++) {
                    auto obj = std::make_unique<SnowParticle>();
                    scene.push_back(move(obj));
                }
            }

            else {
                for(int i = 0; i < 3; i++) {
                    auto obj = std::make_unique<IceParticle>();
                    scene.push_back(move(obj));
                }
            }
        }
    }

    void onIdle() override {
        // Track time
        static auto time = (float) glfwGetTime();
        // Compute time delta
        float dTime = (float)glfwGetTime() - time;
        time = (float) glfwGetTime();

        // Set gray background
        glClearColor(0.0f,0.0f,0.0f,1.0f);

        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        dt += dTime;

        // Add object to scene when time reaches certain level
        if (dt > .1) {
            for(int i = 0; i < 5; i++) {
                auto obj = std::make_unique<SnowParticle>();
                scene.push_back(move(obj));
            }
            dt = 0;
        }

        // Update all objects in scene
        // Because we need to delete while iterating this is implemented using c++ iterators
        // In most languages mutating the container during iteration is undefined behaviour
        auto i = std::begin(scene);
        while (i != std::end(scene)) {
            // Update object and remove from list if needed
            auto obj = i->get();
            if (!obj->update(dTime, scene))
                i = scene.erase(i);
            else
                ++i;
        }

        // Render every object in scene
        for(auto& object : scene) {
            object->render(camera);
        }
    }
};

int main() {
    // Create new window
    auto window = ParticleWindow{};

    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}
