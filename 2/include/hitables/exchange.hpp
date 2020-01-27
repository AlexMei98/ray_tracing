#ifndef EXCHANGE_HPP
#define EXCHANGE_HPP

#include "hitable.hpp"

class translate : public hitable {
public:
    translate(hitable *_ptr, const vec3 &_offset) : ptr(_ptr), offset(_offset) {}

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    bool bounding_box(double t0, double t1, aabb &box) const override;

    hitable *ptr;
    vec3 offset;
};

class rotate_y : public hitable {
public:
    rotate_y(hitable *_ptr, double angle);

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    bool bounding_box(double t0, double t1, aabb &box) const override;

    hitable *ptr;
    double sin_theta;
    double cos_theta;
    bool has_box;
    aabb _box;
};

bool translate::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    ray moved(r.origin() - offset, r.direction(), r.time());
    if (ptr->hit(moved, t_min, t_max, rec)) {
        rec.p += offset;
        return true;
    } else {
        return false;
    }
}

bool translate::bounding_box(double t0, double t1, aabb &box) const {
    if (ptr->bounding_box(t0, t1, box)) {
        box = aabb(box.min() + offset, box.max() + offset);
        return true;
    } else {
        return false;
    }
}

rotate_y::rotate_y(hitable *_ptr, double angle) : ptr(_ptr) {
    double radians = (M_PI / 180.0) * angle;
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    has_box = _ptr->bounding_box(0, 1, _box);
    vec3 min(MAXFLOAT, MAXFLOAT, MAXFLOAT);
    vec3 max(-MAXFLOAT, -MAXFLOAT, -MAXFLOAT);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                double x = i * _box.max().x() + (1 - i) * _box.min().x();
                double y = j * _box.max().y() + (1 - j) * _box.min().y();
                double z = k * _box.max().z() + (1 - k) * _box.min().z();
                double new_x = cos_theta * x + sin_theta * z;
                double new_z = -sin_theta * x + cos_theta * z;
                vec3 tester(new_x, y, new_z);
                for (int c = 0; c < 3; c++) {
                    if (tester[c] > max[c]) {
                        max[c] = tester[c];
                    }
                    if (tester[c] < min[c]) {
                        min[c] = tester[c];
                    }
                }
            }
        }
    }
    _box = aabb(min, max);
}

bool rotate_y::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    vec3 origin = r.origin();
    vec3 direction = r.direction();
    origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
    origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];
    direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
    direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];
    ray rotated(origin, direction, r.time());
    if (ptr->hit(rotated, t_min, t_max, rec)) {
        vec3 p = rec.p;
        vec3 normal = rec.normal;
        p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
        p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];
        normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
        normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];
        rec.p = p;
        rec.normal = normal;
        return true;
    } else {
        return false;
    }
}

bool rotate_y::bounding_box(double t0, double t1, aabb &box) const {
    box = _box;
    return has_box;
}

#endif // EXCHANGE_HPP
