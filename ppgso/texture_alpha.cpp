#include <iostream>
#include "texture_alpha.h"

ppgso::TextureAlpha::TextureAlpha(int width, int height) : image{width, height} {
    initGL();
    update();
}

ppgso::TextureAlpha::TextureAlpha(ImageAlpha&& image) : image{std::move(image)} {
    initGL();
    update();
}

ppgso::TextureAlpha::~TextureAlpha() {
    glDeleteTextures(1, &texture);
}

void ppgso::TextureAlpha::initGL() {
    // Create new texture object
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Reserve texture storage
    glTexStorage2D(GL_TEXTURE_2D, 3, GL_RGBA8, image.width, image.height);

    // Set up mipmapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // Update texture with data from image framebuffer
    update();
}

void ppgso::TextureAlpha::update() {
    bind();
    // Upload texture to GPU
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.width, image.height, GL_RGBA, GL_UNSIGNED_BYTE, image.getFramebuffer().data());

    // Re-generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);
}

void ppgso::TextureAlpha::bind(int id) const {
    glActiveTexture((GLenum) (GL_TEXTURE0 + id));
    glBindTexture(GL_TEXTURE_2D, texture);
}

GLuint ppgso::TextureAlpha::getTexture() {
    return texture;
}
