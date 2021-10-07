// Task 1 - Load a 512x512 image lena.raw
//        - Apply specified per-pixel transformation to each pixel
//        - Save as result.raw
#include <fstream>
#include <iostream>

// Size of the framebuffer
const unsigned int SIZE = 512;

// A simple RGB struct will represent a pixel in the framebuffer
struct Pixel {
    // TODO: Define correct data type for r, g, b channel
    uint8_t r, g, b;
};

int main()
{
    // Initialize a framebuffer
    auto framebuffer = new Pixel[SIZE][SIZE];

    // TODO: Open file lena.raw (this is 512x512 RAW GRB format)
    std::ifstream file("lena.raw", std::ios::binary);

    // TODO: Read data to framebuffer and close the file
    file.read(reinterpret_cast<char *>(framebuffer), sizeof(Pixel) * SIZE * SIZE);
    file.close();

    unsigned int border = SIZE / 3;

    // Traverse the framebuffer
    for (unsigned int y = 0; y < SIZE; y++) {
        for (unsigned int x = 0; x < SIZE; x++) {
            if (x < border) {
                if (y < border) {
                    framebuffer[x][y].g = 0;
                    framebuffer[x][y].b = 0;
                }
                else if (y < 2 * border) {
                    framebuffer[x][y].r = 0;
                    framebuffer[x][y].b = 0;
                }
                else {
                    framebuffer[x][y].r = 0;
                    framebuffer[x][y].g = 0;
                }
            }
            else if (x < 2 * border) {
                if (y < border) {
                    framebuffer[x][y].r = 0;
                    framebuffer[x][y].g = 0;
                }
                else if (y < 2 * border) {
                    framebuffer[x][y].g = 0;
                    framebuffer[x][y].b = 0;
                }
                else {
                    framebuffer[x][y].r = 0;
                    framebuffer[x][y].b = 0;
                }
            }
            else {
                if (y < border) {
                    framebuffer[x][y].r = 0;
                    framebuffer[x][y].b = 0;
                }
                else if (y < 2 * border) {
                    framebuffer[x][y].r = 0;
                    framebuffer[x][y].g = 0;
                }
                else {
                    framebuffer[x][y].g = 0;
                    framebuffer[x][y].b = 0;
                }
            }
        }
    }

    std::cout << "Generating task1_filter.raw file ..." << std::endl;
    std::ofstream output("task1_filter.raw", std::ios::binary);
    output.write(reinterpret_cast<char *>(framebuffer), sizeof(Pixel)*SIZE*SIZE);
    output.close();

    std::cout << "Done." << std::endl;
    delete[] framebuffer;
    return EXIT_SUCCESS;
}
