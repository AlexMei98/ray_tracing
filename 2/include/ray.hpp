#ifndef RAY_H
#define RAY_H

#include "vec3.hpp"

class ray {
public:
    ray() = default;

    ray(const vec3 &_A, const vec3 &_B, double time) : A(_A), B(_B), _time(time) {}

    vec3 origin() const { return A; }

    vec3 direction() const { return B; }

    double time() const { return _time; }

    vec3 point_at_parameter(double t) const { return A + t * B; }

    vec3 A;
    vec3 B;

    double _time{};
};


#endif // RAY_H