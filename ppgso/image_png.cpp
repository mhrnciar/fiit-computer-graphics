#include <iostream>
#include "image_png.h"
#include "lodepng.h"

namespace ppgso {
    namespace image {

        ImageAlpha loadPNG(const std::string &png) {
            std::vector<unsigned char> result;
            unsigned int width, height;

            unsigned int error = lodepng::decode(result, width, height, png);
            if (error) std::cout << "decoder error";

            ppgso::ImageAlpha image{static_cast<int>(width), static_cast<int>(height)};
            auto &framebuffer = image.getFramebuffer();

            for (unsigned long i = 0; i < png.size(); i += 4) {
                framebuffer.push_back({result[i], result[i + 1], result[i + 2], result[i + 3]});
            }

            return image;
        }
    }
}
