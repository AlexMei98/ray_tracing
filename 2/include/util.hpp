#ifndef UTIL_H
#define UTIL_H

#include <fstream>
#include <string>
#include <cstring>

#include "vec3.hpp"
#include "aabb.hpp"

std::ofstream get_out(char *argv[]) {
    std::string name(argv[0]);
    name += ".ppm";
    std::ofstream fout(name);
    return fout;
}

vec3 reflect(const vec3 &v, const vec3 &n) {
    return v - 2 * dot(v, n) * n;
}

bool refract(const vec3 &v, const vec3 &n, double ni_over_nt, vec3 &refracted) {
    vec3 uv = unit_vector(v);
    double dt = dot(uv, n);
    double discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
    if (discriminant > 0) {
        refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    } else {
        return false;
    }
}

double schlick(double cosine, double ref_idx) {
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 *= r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

vec3 random_in_unit_sphere() {
    vec3 p;
    do {
        p = 2.0 * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
    } while (p.squared_length() >= 1.0);
    return p;
}

vec3 random_in_unit_disk() {
    vec3 p;
    do {
        p = 2.0 * vec3(drand48(), drand48(), 0.0) - vec3(1, 1, 0);
    } while (p.length() >= 1.0);
    return p;
}

aabb surrounding_box(aabb box0, aabb box1) {
    vec3 small(
            ddmin(box0.min().x(), box1.min().x()),
            ddmin(box0.min().y(), box1.min().y()),
            ddmin(box0.min().z(), box1.min().z())
    );
    vec3 big(
            ddmax(box0.max().x(), box1.max().x()),
            ddmax(box0.max().y(), box1.max().y()),
            ddmax(box0.max().z(), box1.max().z())
    );

    return {small, big};
}

void get_sphere_uv(const vec3 &p, double &u, double &v) {
    double phi = atan2(p.z(), p.x());
    double theta = asin(p.y());
    u = 1 - (phi + M_PI) / (2 * M_PI);
    v = (theta + M_PI / 2) / M_PI;
}

int s2i(char *c) {
    int t = 0;
    int len = std::strlen(c);
    for (int i = 0; i < len; i++) {
        t = t * 10 + int(c[i] - '0');
    }
    return t;
}

#endif // UTIL_H
