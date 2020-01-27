#ifndef MOVING_SPHERE_HPP
#define MOVING_SPHERE_HPP

#include "hitable.hpp"
#include "material.hpp"

class moving_sphere : public hitable {
public:
    moving_sphere() = default;

    moving_sphere(vec3 _center0, vec3 _center1, double _time0, double _time1, double _radius, material *_mat_ptr)
            : center0(_center0), center1(_center1), radius(_radius), time0(_time0), time1(_time1), mat_ptr(_mat_ptr) {}

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;
    bool bounding_box(double t0, double t1, aabb &box) const override;

    vec3 center(double time) const;

    vec3 center0, center1;
    double time0{}, time1{};
    double radius{};
    material *mat_ptr{};
};

bool moving_sphere::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    vec3 oc = r.origin() - center(r.time());
    double a = dot(r.direction(), r.direction());
    double b = dot(oc, r.direction());
    double c = dot(oc, oc) - radius * radius;
    double discriminant = b * b - a * c;
    if (discriminant > 0) {
        double temp = (-b - sqrt(discriminant)) / a;
        if (temp > t_min && temp < t_max) {
            rec.t = temp;
            rec.p = r.point_at_parameter(temp);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp > t_min && temp < t_max) {
            rec.t = temp;
            rec.p = r.point_at_parameter(temp);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

vec3 moving_sphere::center(double time) const {
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool moving_sphere::bounding_box(double t0, double t1, aabb &box) const {
    aabb box0(center(t0) - vec3(radius, radius, radius), center(t0) + vec3(radius, radius, radius));
    aabb box1(center(t1) - vec3(radius, radius, radius), center(t1) + vec3(radius, radius, radius));
    box = surrounding_box(box0, box1);
    return true;
}

#endif // MOVING_SPHERE_HPP
