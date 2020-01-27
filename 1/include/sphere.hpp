#ifndef SPHERE_H
#define SPHERE_H

#include "hitable.hpp"
#include "material.hpp"

class sphere : public hitable {
public:
    sphere() = default;

    sphere(vec3 _center, double _radius, material *_mat_ptr) : center(_center), radius(_radius), mat_ptr(_mat_ptr) {}

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    vec3 center;
    double radius{};
    material *mat_ptr{};
};

bool sphere::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    vec3 oc = r.origin() - center;
    double a = dot(r.direction(), r.direction());
    double b = dot(oc, r.direction());
    double c = dot(oc, oc) - radius * radius;
    double discriminant = b * b - a * c;
    if (discriminant > 0) {
        double temp = (-b - sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(temp);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(temp);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

#endif // SPHERE_H
