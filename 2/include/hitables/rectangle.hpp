#ifndef XY_RECTANGLE_HPP
#define XY_RECTANGLE_HPP

#include "hitable.hpp"

class xy_rectangle : public hitable {
public:
    xy_rectangle() = default;

    xy_rectangle(double _x0, double _x1, double _y0, double _y1, double _k, material *_mat_ptr)
            : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mat_ptr(_mat_ptr) {}

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    bool bounding_box(double t0, double t1, aabb &box) const override;

    material *mat_ptr{};
    double x0{}, x1{}, y0{}, y1{}, k{};
};

class xz_rectangle : public hitable {
public:
    xz_rectangle() = default;

    xz_rectangle(double _x0, double _x1, double _z0, double _z1, double _k, material *_mat_ptr)
            : x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mat_ptr(_mat_ptr) {}

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    bool bounding_box(double t0, double t1, aabb &box) const override;

    material *mat_ptr{};
    double x0{}, x1{}, z0{}, z1{}, k{};
};

class yz_rectangle : public hitable {
public:
    yz_rectangle() = default;

    yz_rectangle(double _y0, double _y1, double _z0, double _z1, double _k, material *_mat_ptr)
            : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mat_ptr(_mat_ptr) {}

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    bool bounding_box(double t0, double t1, aabb &box) const override;

    material *mat_ptr{};
    double y0{}, y1{}, z0{}, z1{}, k{};
};

bool xy_rectangle::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    double t = (k - r.origin().z()) / r.direction().z();
    if (t < t_min || t > t_max) return false;
    double x = r.origin().x() + t * r.direction().x();
    double y = r.origin().y() + t * r.direction().y();
    if (x < x0 || x > x1 || y < y0 || y > y1) return false;
    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    rec.t = t;
    rec.mat_ptr = mat_ptr;
    rec.p = r.point_at_parameter(t);
    rec.normal = vec3(0, 0, 1);
    return true;
}

bool xy_rectangle::bounding_box(double t0, double t1, aabb &box) const {
    box = aabb(vec3(x0, y0, k - 0.0001), vec3(x1, y1, k + 0.0001));
    return true;
}

bool xz_rectangle::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    double t = (k - r.origin().y()) / r.direction().y();
    if (t < t_min || t > t_max) return false;
    double x = r.origin().x() + t * r.direction().x();
    double z = r.origin().z() + t * r.direction().z();
    if (x < x0 || x > x1 || z < z0 || z > z1) return false;
    rec.u = (x - x0) / (x1 - x0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    rec.mat_ptr = mat_ptr;
    rec.p = r.point_at_parameter(t);
    rec.normal = vec3(0, 1, 0);
    return true;
}

bool xz_rectangle::bounding_box(double t0, double t1, aabb &box) const {
    box = aabb(vec3(x0, k - 0.0001, z0), vec3(x1, k + 0.0001, z1));
    return true;
}

bool yz_rectangle::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    double t = (k - r.origin().x()) / r.direction().x();
    if (t < t_min || t > t_max) return false;
    double y = r.origin().y() + t * r.direction().y();
    double z = r.origin().z() + t * r.direction().z();
    if (y < y0 || y > y1 || z < z0 || z > z1) return false;
    rec.u = (y - y0) / (y1 - y0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    rec.mat_ptr = mat_ptr;
    rec.p = r.point_at_parameter(t);
    rec.normal = vec3(1, 0, 0);
    return true;
}

bool yz_rectangle::bounding_box(double t0, double t1, aabb &box) const {
    box = aabb(vec3(k - 0.0001, y0, z0), vec3(k + 0.0001, y1, z1));
    return true;
}

#endif // XY_RECTANGLE_HPP
