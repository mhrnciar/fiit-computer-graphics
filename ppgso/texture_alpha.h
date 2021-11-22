#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>

#include <GL/glew.h>

#include "image_alpha.h"

namespace ppgso {

    class TextureAlpha {
    public:

        /*!
         * Create new empty texture and bind it to OpenGL.
         * @param width - Width in pixels.
         * @param height - Height in pixels.
         */
        TextureAlpha(int width, int height);

        /*!
         * Load from image.
         * @param image - Image to use
         */
        TextureAlpha(ImageAlpha&& image);

        ~TextureAlpha();

        /*!
         * Update the OpenGL texture in memory.
         */
        void update();

        /*!
         * Get OpenGL texture identifier number.
         * @return - OpenGL texture identifier number.
         */
        GLuint getTexture();

        /*!
         * Bind the OpenGL texture for use.
         * @param id - OpenGL Texture id to bind to (0 default)
         */
        void bind(int id = 0) const;

        ImageAlpha image;
    private:
        void initGL();
        GLuint texture;
    };
}

