#ifndef METAL_H
#define METAL_H

#include <cmath>

#include "material.hpp"

class metal : public material {
public:
    explicit metal(const vec3 &_albedo, double _fuzz) : albedo(_albedo), fuzz(fabs(_fuzz < 1 ? _fuzz : 1)) {}

    bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere(), r_in.time());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

    vec3 albedo;
    double fuzz;
};

#endif // METAL_HPP
