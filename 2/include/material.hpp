#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.hpp"
#include "hitable.hpp"

class material {
public:
    virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const = 0;

    virtual vec3 emitted(double u, double v, const vec3 &p) const { return {0, 0, 0}; }
};

#endif // MATERIAL_H
