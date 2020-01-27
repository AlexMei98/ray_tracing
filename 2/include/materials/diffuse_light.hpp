#ifndef RAY_TRACING_DIFFUSE_LIGHT_HPP
#define RAY_TRACING_DIFFUSE_LIGHT_HPP

#include "material.hpp"
#include "texture.hpp"

class diffuse_light : public material {
public:
    diffuse_light(texture *_emit) : emit(_emit) {}

    bool scatter(const ray &r, const hit_record &rec, vec3 &attenuation, ray &scattered) const override;

    virtual vec3 emitted(double u, double v, const vec3 &p) const;

    texture *emit;
};

bool diffuse_light::scatter(const ray &r, const hit_record &rec, vec3 &attenuation, ray &scattered) const {
    return false;
}

vec3 diffuse_light::emitted(double u, double v, const vec3 &p) const {
    return emit->value(u, v, p);
}

#endif //RAY_TRACING_DIFFUSE_LIGHT_HPP
