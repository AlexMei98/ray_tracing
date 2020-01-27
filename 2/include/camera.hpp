#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include "util.hpp"

class camera {
public:
    camera(vec3 look_from, vec3 look_at, vec3 vup, double fov, double aspect, double aperture, double focus_dist,
           double _time0, double _time1) : origin(look_from), lens_radius(aperture / 2), time0(_time0), time1(_time1) {
        double theta = fov * M_PI / 180;
        double half_height = tan(theta / 2);
        double half_width = aspect * half_height;
        w = unit_vector(look_from - look_at);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);
        lower_left_corner = vec3(-half_width, -half_height, -1.0);
        lower_left_corner = origin - half_height * focus_dist * v - half_width * focus_dist * u - w * focus_dist;
        horizontal = 2 * half_width * u * focus_dist;
        vertical = 2 * half_height * v * focus_dist;
    }

    ray get_ray(double s, double t) {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();
        double time = time0 + drand48() * (time1 - time0);
        return {origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset, time};
    }

    double lens_radius{};
    double time0{}, time1{};

    vec3 u, v, w;
    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};

#endif // CAMERA_H
