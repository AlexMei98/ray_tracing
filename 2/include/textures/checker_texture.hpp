#ifndef CHECKER_HPP
#define CHECKER_HPP

#include "texture.hpp"

class checker_texture : public texture {
public:
    checker_texture() = default;

    checker_texture(texture *_even, texture *_odd) : even(_even), odd(_odd) {}

    vec3 value(double u, double v, const vec3 &p) const override;

    texture *odd{};
    texture *even{};
};

vec3 checker_texture::value(double u, double v, const vec3 &p) const {
    double sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
    if (sines < 0) {
        return odd->value(u, v, p);
    } else {
        return even->value(u, v, p);
    }
}

#endif // CHECKER_HPP
