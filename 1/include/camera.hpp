#ifndef CAMERA_H
#define CAMERA_H

#include "ray.hpp"
#include "util.hpp"

class camera {
public:
    camera() {
        lower_left_corner = vec3(-2.0, -1.0, -1.0);
        horizontal = vec3(4.0, 0.0, 0.0);
        vertical = vec3(0.0, 2.0, 0.0);
        origin = vec3(0.0, 0.0, 0.0);
    }

    camera(double fov, double aspect) {
        double theta = fov * M_PI / 180;
        double half_height = tan(theta / 2);
        double half_width = aspect * half_height;
        lower_left_corner = vec3(-half_width, -half_height, -1.0);
        horizontal = vec3(2 * half_width, 0.0, 0.0);
        vertical = vec3(0.0, 2 * half_height, 0.0);
        origin = vec3(0.0, 0.0, 0.0);
    }

    camera(vec3 look_from, vec3 look_at, vec3 vup, double fov, double aspect) {
        double theta = fov * M_PI / 180;
        double half_height = tan(theta / 2);
        double half_width = aspect * half_height;
        origin = look_from;
        w = unit_vector(look_from - look_at);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);
        lower_left_corner = vec3(-half_width, -half_height, -1.0);
        lower_left_corner = origin - half_height * v - half_width * u - w;
        horizontal = 2 * half_width * u;
        vertical = 2 * half_height * v;
    }

    camera(vec3 look_from, vec3 look_at, vec3 vup, double fov, double aspect, double aperture, double focus_dist) {
        lens_radius = aperture / 2;
        double theta = fov * M_PI / 180;
        double half_height = tan(theta / 2);
        double half_width = aspect * half_height;
        origin = look_from;
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
        return {origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset};
    }

    double lens_radius{};
    vec3 u, v, w;

    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
};

#endif // CAMERA_H
