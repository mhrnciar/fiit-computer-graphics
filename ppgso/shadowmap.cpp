#include <iostream>

#include "shadowmap.h"

ppgso::Shadowmap::Shadowmap() {
    initGL();
}

ppgso::Shadowmap::~Shadowmap() {
    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &shadowMap);
}

void ppgso::Shadowmap::initGL() {
    glGenFramebuffers(1, &fbo);

    // Create new texture object
    glGenTextures(1, &shadowMap);
    glBindTexture(GL_TEXTURE_2D, shadowMap);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    // Set up mipmapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_EQUAL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMap, 0);

    glDrawBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("NOT COMPLETE\n");

    update();
}

void ppgso::Shadowmap::update() {
    BindForWriting();

    //glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 1024, 1024, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

    // Re-generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);
}

void ppgso::Shadowmap::BindForWriting() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void ppgso::Shadowmap::BindForReading(GLenum TextureUnit) {
    glActiveTexture(TextureUnit);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
}
