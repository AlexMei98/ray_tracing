#ifndef CONSTANT_HPP
#define CONSTANT_HPP

#include "texture.hpp"

class constant_texture : public texture {
public:
    constant_texture() = default;
    explicit constant_texture(vec3 _color) : color(_color) {}

    vec3 value(double u, double v, const vec3 &p) const override;

    vec3 color;
};

vec3 constant_texture::value(double u, double v, const vec3 &p) const {
    return color;
}

#endif // CONSTANT_HPP
