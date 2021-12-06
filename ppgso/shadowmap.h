#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>

#include <GL/glew.h>

#include "image.h"

namespace ppgso {

    class Shadowmap {
    public:

        /*!
         * Load from image.
         *
         */
        Shadowmap();

        ~Shadowmap();

        void BindForWriting();

        void BindForReading(GLenum TextureUnit);

    private:
        void initGL();
        GLuint shadowMap, fbo;
        unsigned int width = 1280;
        unsigned int height = 720;
    };
}

