#ifndef AABB_HPP
#define AABB_HPP

#include <cstdlib>

inline double ddmin(double a, double b) { return a < b ? a : b; }

inline double ddmax(double a, double b) { return a > b ? a : b; }

class aabb {
public:
    aabb() = default;

    aabb(const vec3 &min, const vec3 &max) : _min(min), _max(max) {}

    vec3 min() const { return _min; }

    vec3 max() const { return _max; }

    bool hit(const ray &r, double t_min, double t_max) const;

    vec3 _min;
    vec3 _max;
};

bool aabb::hit(const ray &r, double t_min, double t_max) const {
    for (int a = 0; a < 3; a++) {
        double invD = 1.0f / r.direction()[a];
        double t0 = (min()[a] - r.direction()[a]) * invD;
        double t1 = (max()[a] - r.direction()[a]) * invD;
        if (invD < 0.0f) std::swap(t0, t1);
        t_min = ddmax(t0, t_min);
        t_max = ddmin(t1, t_max);
        if (t_max <= t_min) return false;
    }
    return true;
}

#endif //AABB_HPP
