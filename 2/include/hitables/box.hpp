#ifndef BOX_HPP
#define BOX_HPP

#include "hitable.hpp"
#include "rectangle.hpp"
#include "flip_normals.hpp"
#include "hitable_list.hpp"

class box : public hitable {
public:
    box() = default;

    box(const vec3 &p0, const vec3 &p1, material *ptr);

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    bool bounding_box(double t0, double t1, aabb &box) const override;

    vec3 p_min, p_max;
    hitable *list_ptr{};
};

box::box(const vec3 &p0, const vec3 &p1, material *ptr) {
    p_min = p0;
    p_max = p1;
    auto **list = new hitable*[6];
    list[0] = new xy_rectangle(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), ptr);
    list[1] = new flip_normals(new xy_rectangle(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), ptr));
    list[2] = new xz_rectangle(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), ptr);
    list[3] = new flip_normals(new xz_rectangle(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), ptr));
    list[4] = new yz_rectangle(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), ptr);
    list[5] = new flip_normals(new yz_rectangle(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), ptr));
    list_ptr = new hitable_list(list, 6);
}

bool box::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    return list_ptr->hit(r, t_min, t_max, rec);
}

bool box::bounding_box(double t0, double t1, aabb &box) const {
    box = aabb(p_min, p_max);
    return true;
}

#endif // BOX_HPP
