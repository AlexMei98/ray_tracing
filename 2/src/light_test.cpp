#include "ray.hpp"
#include "util.hpp"
#include "camera.hpp"

#include "hitable.hpp"
#include "hitables/sphere.hpp"
#include "hitables/histable_list.hpp"
#include "hitables/rectangle.hpp"

#include "materials/lambertian.hpp"
#include "materials/diffuse_light.hpp"

#include "aabb.hpp"
#include "textures/constant_texture.hpp"
#include "textures/noise_texture.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.hpp"

vec3 color(const ray &r, hitable *world, int depth) {
    hit_record rec;
    if (world->hit(r, 0.0001, MAXFLOAT, rec)) {
        ray scattered;
        vec3 attenuation;
        vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth + 1) + emitted;
        } else {
            return emitted;
        }
    } else {
        return {0, 0, 0};
    }
}

hitable *light_scene() {
    texture *perlin_texture = new noise_texture(2);
    auto **list = new hitable *[4];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(perlin_texture));
    list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(perlin_texture));
    list[2] = new sphere(vec3(0, 7, 0), 2, new diffuse_light(new constant_texture(vec3(2, 2, 2))));
    list[3] = new xy_rectangle(3, 5, 1, 3, -2, new diffuse_light(new constant_texture(vec3(2, 2, 2))));
    return new hitable_list(list, 4);
}

int main(int argv, char *argc[]) {
    int nx = 1920, ny = 1080, ns = 60;
    std::ofstream fout = get_out(argc);
    fout << "P3\n" << nx << " " << ny << "\n255\n";

//    vec3 look_from(278, 278, -800);
//    vec3 look_at(278, 278, 0);
    vec3 look_from(23, 5, 12);
    vec3 look_at(0, 2, 0);
    double dist_to_focus = (look_at - look_from).length();
    double aperture = 0.0;
    double fov = 20.0;
    camera cam(look_from, look_at, vec3(0, 1, 0), fov, double(nx) / double(ny), aperture, dist_to_focus, 0.0, 1.0);
    hitable *world = light_scene();
    for (int j = ny - 1; j > 0; j--) {
        for (int i = 0; i < nx; i++) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                double u = (double(i) + drand48()) / double(nx);
                double v = (double(j) + drand48()) / double(ny);
                col += color(cam.get_ray(u, v), world, 0);
            }
            col /= double(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            fout << ir << " " << ig << " " << ib << "\n";
        }
    }
    return 0;
}
