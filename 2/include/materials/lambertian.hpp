#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "material.hpp"
#include "util.hpp"
#include "texture.hpp"

class lambertian : public material {
public:
    explicit lambertian(texture *_albedo) : albedo(_albedo) {}

    bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const override {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p, target - rec.p, r_in.time());
        double u, v;

        attenuation = albedo->value(0, 0, rec.p);
        return true;
    }

    texture *albedo;
};

#endif // LAMBERTIAN_H
