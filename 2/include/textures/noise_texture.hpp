#ifndef NOISE_HPP
#define NOISE_HPP

#include "texture.hpp"
#include "perlin.hpp"

class noise_texture : public texture {
public:
    noise_texture() = default;

    explicit noise_texture(double _scale) : scale(_scale) {}

    vec3 value(double u, double v, const vec3 &p) const override;

    perlin noise;
    double scale{1.0};
};

vec3 noise_texture::value(double u, double v, const vec3 &p) const {
//    return vec3(1, 1, 1) * 0.5 * (1 + noise.noise(scale * p));
//    return vec3(1, 1, 1) * 0.5 * (1 + noise.turb(scale * p));
//    return vec3(1, 1, 1) * noise.turb(scale * p);
    return vec3(1, 1, 1) * 0.5 * (1 + sin(scale * (p.x() + p.z()) / 2 + 10 * noise.turb(p)));
}

#endif // NOISE_HPP
