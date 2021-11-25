#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>

namespace ppgso {

    class ImageAlpha {
    public:
        struct Pixel {
            uint8_t r, g, b, a;
        };

        /*!
         * Create new empty image.
         *
         * @param width - Width in pixels.
         * @param height - Height in pixels.
         */
        ImageAlpha(int width, int height);

        static uint8_t clamp(float value);

        /*!
         * Get raw access to the image data.
         *
         * @return - Pointer to the raw RGBA framebuffer data.
         */
        std::vector<Pixel>& getFramebuffer();

        /*!
         * Get single pixel from the framebuffer.
         *
         * @param x - X position of the pixel in the framebuffer.
         * @param y - Y position of the pixel in the framebuffer.
         * @return - Reference to the pixel.
         */
        Pixel& getPixel(int x, int y);

        /*!
         * Set pixel on coordinates x and y
         * @param x Horizontal coordinate
         * @param y Vertical coordinate
         * @param color Pixel color to set
         */
        void setPixel(int x, int y, const Pixel& color);

        /*!
         * Set pixel on coordinates x and y
         * @param x Horizontal coordinate
         * @param y Vertical coordinate
         * @param r Red channel <0, 255>
         * @param g Green channel <0, 255>
         * @param b Blue channel <0, 255>
         * @param a Alpha channel <0, 255>
         */
        void setPixel(int x, int y, int r, int g, int b, int a);

        /*!
         * Set pixel on coordinates x and y
         * @param x Horizontal coordinate
         * @param y Vertical coordinate
         * @param r Red channel <0, 1>
         * @param g Green channel <0, 1>
         * @param b Blue channel <0, 1>
         * @param a Alpha channel <0, 1>
         */
        void setPixel(int x, int y, float r, float g, float b, float a);

        /*!
         * Clear the image using single color
         * @param color Pixel color to set the image to
         */
        void clear(const Pixel& color = {0, 0, 0, 0});

        int width, height;
    private:
        std::vector<Pixel> framebuffer;
    };
}

