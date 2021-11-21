#pragma once
#include "image_alpha.h"

namespace ppgso {
    namespace image {
/*!
 * Load BMP image from file. Only uncompressed RGB format is supported.
 *
 * @param bmp - File path to a BMP image.
 */
        ppgso::ImageAlpha loadPNG(const std::string &png);

/*!
 * Save as BMP image.
 * @param image - Image to save.
 * @param bmp - Name of the BMP file to save image to.
 */
        void savePNG(ppgso::ImageAlpha &image, const std::string &png);

    }
}
