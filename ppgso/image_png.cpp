#include <iostream>
#include "image_png.h"
#include "lodepng.h"

namespace ppgso {
    namespace image {

        ImageAlpha loadPNG(const std::string &png) {
            std::vector<unsigned char> result;
            unsigned int width, height, k = 0;

            unsigned int error = lodepng::decode(result, width, height, png);
            if (error) std::cout << "decoder error";

            ppgso::ImageAlpha image{static_cast<int>(width), static_cast<int>(height)};
            auto &framebuffer = image.getFramebuffer();

            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; j++) {
                    image.setPixel(j, i, {result[k++], result[k++], result[k++], result[k++]});
                }
            }

            return image;
        }
    }
}
