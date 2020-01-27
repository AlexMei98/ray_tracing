#ifndef HITABLE_LIST_H
#define HITABLE_LIST_H

#include "hitable.hpp"

class hitable_list : public hitable {
public:
    hitable_list() = default;

    hitable_list(hitable **_list, int _n) : list(_list), n(_n) {}

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    hitable **list{};
    int n{};
};

bool hitable_list::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    double closest = t_max;
    for (int i = 0; i < n; i++) {
        if (list[i]->hit(r, t_min, closest, temp_rec)) {
            hit_anything = true;
            closest = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

#endif // HITABLE_LIST_H