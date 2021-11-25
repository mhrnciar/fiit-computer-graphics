#pragma once
#include "image_alpha.h"

namespace ppgso {
    namespace image {
        /*!
         * Load PNG image from file. RGBA images are supported
         * @param png - File path to a PNG image.
         */
        ppgso::ImageAlpha loadPNG(const std::string &png);

        /*!
         * Save as PNG image - not implemented yet!!
         * @param image - Image to save.
         * @param png - Name of the PNG file to save image to.
         */
        void savePNG(ppgso::ImageAlpha &image, const std::string &png);

    }
}
