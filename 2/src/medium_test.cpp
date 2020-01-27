#include "ray.hpp"
#include "util.hpp"
#include "camera.hpp"

#include "hitable.hpp"
#include "hitables/sphere.hpp"
#include "hitables/hitable_list.hpp"
#include "hitables/rectangle.hpp"
#include "hitables/flip_normals.hpp"
#include "hitables/box.hpp"
#include "hitables/exchange.hpp"
#include "hitables/medium.hpp"

#include "materials/lambertian.hpp"
#include "materials/diffuse_light.hpp"

#include "aabb.hpp"
#include "textures/constant_texture.hpp"

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

hitable *cornell_scene() {
    auto **list = new hitable *[8];
    int i = 0;
    material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
    material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
    material *green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
    material *light = new diffuse_light(new constant_texture(vec3(25, 25, 25)));
    list[i++] = new flip_normals(new yz_rectangle(0, 555, 0, 555, 555, green));
    list[i++] = new yz_rectangle(0, 555, 0, 555, 0, red);
    list[i++] = new xz_rectangle(220, 340, 210, 330, 554, light);
    list[i++] = new flip_normals(new xz_rectangle(0, 555, 0, 555, 555, white));
    list[i++] = new xz_rectangle(0, 555, 0, 555, 0, white);
    list[i++] = new flip_normals(new xy_rectangle(0, 555, 0, 555, 555, white));
    hitable *T0 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18),
                              vec3(130, 0, 65));
    hitable *T1 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15),
                              vec3(265, 0, 295));
    list[i++] = new constant_medium(T0, 0.01, new constant_texture(vec3(1.0, 1.0, 1.0)));
    list[i++] = new constant_medium(T1, 0.01, new constant_texture(vec3(0.0, 0.0, 0.0)));
    return new hitable_list(list, i);
}

int main(int argv, char *argc[]) {
    int nx = 1920, ny = 1080, ns = 120;
    std::ofstream fout = get_out(argc);
    fout << "P3\n" << nx << " " << ny << "\n255\n";

    vec3 look_from(278, 278, -800);
    vec3 look_at(278, 278, 0);
    double dist_to_focus = 10;
    double aperture = 0.0;
    double fov = 40.0;
    camera cam(look_from, look_at, vec3(0, 1, 0), fov, double(nx) / double(ny), aperture, dist_to_focus, 0.0, 1.0);
    hitable *world = cornell_scene();
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
