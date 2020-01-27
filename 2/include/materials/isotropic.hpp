#ifndef ISOTROPIC_HPP
#define ISOTROPIC_HPP

#include "material.hpp"
#include "texture.hpp"
#include "util.hpp"

class isotropic : public material {
public:
    explicit isotropic(texture *_albedo) : albedo(_albedo) {}

    bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const override;

    texture *albedo;
};

bool isotropic::scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const {
    scattered = ray(rec.p, random_in_unit_sphere(), r_in.time());
    attenuation = albedo->value(rec.u, rec.v, rec.p);
    return true;
}

#endif // ISOTROPIC_HPP
