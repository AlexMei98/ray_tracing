#ifndef MEDIUM_HPP
#define MEDIUM_HPP

#include "hitable.hpp"
#include "materials/isotropic.hpp"

class constant_medium : public hitable {
public:
    constant_medium(hitable *_boundary, double _density, texture *_mat_ptr)
            : boundary(_boundary), density(_density) { mat_ptr = new isotropic(_mat_ptr); }

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    bool bounding_box(double t0, double t1, aabb &box) const override;

    hitable *boundary;
    double density;
    material *mat_ptr;
};

bool constant_medium::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    hit_record rec1, rec2;
    if (boundary->hit(r, -MAXFLOAT, MAXFLOAT, rec1)) {
        if (boundary->hit(r, rec1.t + 0.0001, MAXFLOAT, rec2)) {
            if (rec1.t < t_min) {
                rec1.t = t_min;
            }
            if (rec2.t > t_max) {
                rec2.t = t_max;
            }
            if (rec1.t >= rec2.t) {
                return false;
            }
            if (rec1.t < 0) {
                rec1.t = 0;
            }
            double distant_inside_boundary = (rec2.t - rec1.t) * r.direction().length();
            double hit_distance = -(1 / density) * log(drand48());
            if (hit_distance < distant_inside_boundary) {
                rec.t = rec1.t + hit_distance / r.direction().length();
                rec.p = r.point_at_parameter(rec.t);
                rec.normal = vec3(1, 0, 0);
                rec.mat_ptr = mat_ptr;
                return true;
            }
        }
    }
    return false;
}

bool constant_medium::bounding_box(double t0, double t1, aabb &box) const {
    return boundary->bounding_box(t0, t1, box);
}

#endif // MEDIUM_HPP
