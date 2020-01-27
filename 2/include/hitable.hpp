#ifndef HITABLE_H
#define HITABLE_H

#include "ray.hpp"
#include "aabb.hpp"

class material;

class hit_record {
public:
    double t{}, u{}, v{};
    vec3 p;
    vec3 normal;
    material *mat_ptr{};
};

class hitable {
public:
    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const = 0;

    virtual bool bounding_box(double t0, double t1, aabb &box) const = 0;
};

#endif // HITABLE_H