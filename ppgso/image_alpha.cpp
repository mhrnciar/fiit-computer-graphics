#include <algorithm>
#include "image_alpha.h"

uint8_t ppgso::ImageAlpha::clamp(float value) {
    return (uint8_t) (std::min(std::max(value, 0.0f), 1.0f) * 255.0f);
}

ppgso::ImageAlpha::ImageAlpha(int width, int height) : width{width}, height{height} {
    framebuffer.resize((size_t) (width * height));
}

std::vector<ppgso::ImageAlpha::Pixel>& ppgso::ImageAlpha::getFramebuffer() {
    return framebuffer;
}

ppgso::ImageAlpha::Pixel& ppgso::ImageAlpha::getPixel(int x, int y) {
    return framebuffer[x + y * width];
}

void ppgso::ImageAlpha::setPixel(int x, int y, const ImageAlpha::Pixel& color) {
    framebuffer[x + y * width] = color;
}

void ppgso::ImageAlpha::clear(const ppgso::ImageAlpha::Pixel &color) {
    framebuffer = std::vector<Pixel>(framebuffer.size(), color);
}

void ppgso::ImageAlpha::setPixel(int x, int y, int r, int g, int b, int a) {
    setPixel(x, y,{(uint8_t)r, (uint8_t)g, (uint8_t)b, (uint8_t)a});
}

void ppgso::ImageAlpha::setPixel(int x, int y, float r, float g, float b, float a) {
    setPixel(x, y,{clamp(r), clamp(g), clamp(b), clamp(a)});
}
