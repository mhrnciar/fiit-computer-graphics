// Task 2 - Implement Bresenham drawing alg.
//        - Draw a star using lines
//        - Make use of std::vector to define the shape
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

#include <ppgso/ppgso.h>

// Size of the framebuffer
const unsigned int SIZE = 512;

struct Point {
    int x,y;
};

// Bresenham drawing algorithm
void drawLine(ppgso::Image& framebuffer, Point& from, Point& to) {
    // TODO: Implement Bresenham drawing algorithm
    int x, y;
    int x_delta = abs(to.x - from.x);
    int y_delta = abs(to.y - from.y);

    int lower_x, higher_x;
    int lower_y, higher_y;

    if (to.x >= from.x) {
        higher_x = to.x;
        lower_x = from.x;
    }
    else {
        higher_x = from.x;
        lower_x = to.x;
    }

    if (to.y >= from.y) {
        higher_y = to.y;
        lower_y = from.y;
    }
    else {
        higher_y = from.y;
        lower_y = to.y;
    }

    if (x_delta >= y_delta) {
        int y_increment;
        x = lower_x;

        if (lower_x == from.x)
            y = from.y;
        else
            y = to.y;

        if (y == lower_y)
            y_increment = 1;
        else
            y_increment = -1;

        int error = -(x_delta);
        while (x <= higher_x) {
            framebuffer.setPixel(x, y, 255, 255, 255);
            x += 1;
            error+= 2 * (y_delta);

            if (error > 0) {
                y += y_increment;
                error -= 2 * (x_delta);
            }
        }
    }

    else {
        int x_increment;
        y = lower_y;

        if (lower_y == from.y)
            x = from.x;
        else
            x = to.x;

        if (x == lower_x)
            x_increment = 1;
        else
            x_increment = -1;

        int error = -(y_delta);
        while (y <= higher_y) {
            framebuffer.setPixel(x, y, 255, 255, 255);
            y += 1;
            error += 2 * (x_delta);

            if (error > 0) {
                x += x_increment;
                error -= 2 * (y_delta);
            }
        }
    }
}

int main(){
    // Use ppgso::Image as our framebuffer
    ppgso::Image framebuffer(SIZE, SIZE);

    // TODO: Generate star points
    std::vector<Point> points;

    //float PI = 3.14;

    float r = 200;
    float centering_offset = SIZE / 2;
    float angle= 90;

    for (int i = 0; i < 6; i++){

        Point tmp_o;
        tmp_o.x = int(centering_offset + r * cos (angle * ppgso::PI / 180));
        tmp_o.y = int(centering_offset - r * sin (angle * ppgso::PI /180));

        points.push_back(tmp_o);
        angle += 72 * 2;

    }

    // Draw lines
    for(unsigned int i = 0; i < points.size() - 1; i++)
        drawLine(framebuffer, points[i], points[i+1]);

    // Save the result
    std::cout << "Generating task2_bresenham.bmp file ..." << std::endl;
    ppgso::image::saveBMP(framebuffer, "task2_bresenham.bmp");

    std::cout << "Done." << std::endl;
    return EXIT_SUCCESS;
}

