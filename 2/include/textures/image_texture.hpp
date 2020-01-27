#ifndef IMAGE_TEXTURE_HPP
#define IMAGE_TEXTURE_HPP

#include "texture.hpp"

class image_texture : public texture {
public:
    image_texture() = default;

    image_texture(unsigned char *_pixels, int A, int B) : pixels(_pixels), nx(A), ny(B) {}

    vec3 value(double u, double v, const vec3 &p) const override;

    unsigned char *pixels{};
    int nx{}, ny{};
};

vec3 image_texture::value(double u, double v, const vec3 &p) const {
    int i = int(u * nx);
    int j = int ((1 - v) * ny - 0.001);
    if (i < 0) {
        i = 0;
    }
    if (j < 0) {
        j = 0;
    }
    if (i > nx - 1) {
        i = nx - 1;
    }
    if (j > ny - 1) {
        j = ny - 1;
    }
    double r = int(pixels[3 * i + 3 * nx * j]) / 255.0;
    double g = int(pixels[3 * i + 3 * nx * j + 1]) / 255.0;
    double b = int(pixels[3 * i + 3 * nx * j + 2]) / 255.0;
    return {r, g, b};
}

#endif // IMAGE_TEXTURE_HPP
