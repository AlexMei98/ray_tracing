#ifndef FLIP_NORMALS_HPP
#define FLIP_NORMALS_HPP

#include "hitable.hpp"

class flip_normals : public hitable {
public:
    flip_normals(hitable *_ptr) : ptr(_ptr) {}

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    bool bounding_box(double t0, double t1, aabb &box) const override;

    hitable *ptr;
};

bool flip_normals::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    if (ptr->hit(r, t_min, t_max, rec)) {
        rec.normal = -rec.normal;
        return true;
    } else {
        return false;
    }
}

bool flip_normals::bounding_box(double t0, double t1, aabb &box) const {
    return ptr->bounding_box(t0, t1, box);
}

#endif // FLIP_NORMALS_HPP
