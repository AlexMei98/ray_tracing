#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "material.hpp"
#include "util.hpp"

class dielectric : public material {
public:
    explicit dielectric(double _ref_idx) : ref_idx(_ref_idx) {}

    bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const override {
        vec3 outward_normal;
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        double ni_over_nt;
        attenuation = vec3(1.0, 1.0, 1.0);
        vec3 refracted;
        double reflect_prob;
        double cosine;
        if (dot(r_in.direction(), rec.normal) > 0) {
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;
            cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
        } else {
            outward_normal = rec.normal;
            ni_over_nt = 1.0 / ref_idx;
            cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
            reflect_prob = schlick(cosine, ref_idx);
        } else {
            reflect_prob = 1.0;
        }
        if (drand48() < reflect_prob) {
            scattered = ray(rec.p, reflected, r_in.time());
        } else {
            scattered = ray(rec.p, refracted, r_in.time());
        }
        return true;
    }

    double ref_idx;
};

#endif // DIELECTRIC_H
