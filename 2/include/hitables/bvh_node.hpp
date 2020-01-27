#ifndef BVH_NODE_HPP
#define BVH_NODE_HPP

#include "hitable.hpp"

class bvh_node : public hitable {
public:
    bvh_node() = default;
    bvh_node(hitable **list, int n, double time0, double time1);

    bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

    bool bounding_box(double t0, double t1, aabb &box) const override;

    hitable *left{};
    hitable *right{};
    aabb _box;
};

int box_x_compare(const void *a, const void *b) {
    aabb box_left, box_right;
    hitable *ah = *(hitable**)a;
    hitable *bh = *(hitable**)b;
    if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right)) {
        std::cerr << "no bounding box in bvh_node constructor\n";
    }
    if (box_left.min().x() - box_right.min().x() < 0.0) {
        return -1;
    } else if (box_left.min().x() - box_right.min().x() > 0.0) {
        return 1;
    } else {
        return 0;
    }
}

int box_y_compare(const void *a, const void *b) {
    aabb box_left, box_right;
    hitable *ah = *(hitable**)a;
    hitable *bh = *(hitable**)b;
    if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right)) {
        std::cerr << "no bounding box in bvh_node constructor\n";
    }
    if (box_left.min().y() - box_right.min().y() < 0.0) {
        return -1;
    } else if (box_left.min().y() - box_right.min().y() > 0.0) {
        return 1;
    } else {
        return 0;
    }
}

int box_z_compare(const void *a, const void *b) {
    aabb box_left, box_right;
    hitable *ah = *(hitable**)a;
    hitable *bh = *(hitable**)b;
    if (!ah->bounding_box(0, 0, box_left) || !bh->bounding_box(0, 0, box_right)) {
        std::cerr << "no bounding box in bvh_node constructor\n";
    }
    if (box_left.min().z() - box_right.min().z() < 0.0) {
        return -1;
    } else if (box_left.min().z() - box_right.min().z() > 0.0) {
        return 1;
    } else {
        return 0;
    }
}

bvh_node::bvh_node(hitable **list, int n, double time0, double time1) {
    int axis = int(3 * drand48());
    if (axis == 0) {
        qsort(list, n, sizeof(hitable*), box_x_compare);
    } else if (axis == 1) {
        qsort(list, n, sizeof(hitable*), box_y_compare);
    } else {
        qsort(list, n, sizeof(hitable*), box_z_compare);
    }
    if (n == 1) {
        left = right = list[0];
    } else if (n == 2) {
        left = list[0];
        right = list[1];
    } else {
        left = new bvh_node(list, n / 2, time0, time1);
        right = new bvh_node(list + n / 2, n - n / 2, time0, time1);
    }
    aabb box_left, box_right;
    if (!left->bounding_box(time0, time1, box_left) || (!right->bounding_box(time0, time1, box_right))) {
        std::cerr << "no bounding box in bvh_node constructor\n";
    }
    _box = surrounding_box(box_left, box_right);
}

bool bvh_node::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    if (_box.hit(r, t_min, t_max)) {
        hit_record left_rec, right_rec;
        bool hit_left = left->hit(r, t_min, t_max, left_rec);
        bool hit_right = right->hit(r, t_min, t_max, right_rec);
        if (hit_left && hit_right) {
            if (left_rec.t < right_rec.t) {
                rec = left_rec;
            } else {
                rec = right_rec;
            }
        } else if (hit_left) {
            rec = left_rec;
        } else if (hit_right) {
            rec = right_rec;
        } else {
            return false;
        }
        return true;
    } else {
        return false;
    }
}

bool bvh_node::bounding_box(double t0, double t1, aabb &box) const {
    box = _box;
    return true;
}

#endif // BVH_NODE_HPP
