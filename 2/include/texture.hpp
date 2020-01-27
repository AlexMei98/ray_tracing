#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "vec3.hpp"

class texture {
public:
    virtual vec3 value(double u, double v, const vec3 &p) const = 0;
};

#endif // TEXTURE_HPP
