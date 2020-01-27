#include "ray.hpp"
#include "util.hpp"
#include "camera.hpp"

#include "hitable.hpp"
#include "hitables/sphere.hpp"
#include "hitables/moving_sphere.hpp"
#include "hitables/histable_list.hpp"

#include "materials/lambertian.hpp"
#include "materials/metal.hpp"
#include "materials/dielectric.hpp"

#include "aabb.hpp"
#include "textures/constant_texture.hpp"

vec3 color(const ray &r, hitable *world, int depth) {
    hit_record rec;
    if (world->hit(r, 0.0001, MAXFLOAT, rec)) {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth + 1);
        } else {
            return {0, 0, 0};
        }
    } else {
        vec3 unit_direction = unit_vector(r.direction());
        double t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

hitable *random_scene() {
    int n = 500;
    auto **list = new hitable *[n + 1];
    list[0] = new sphere(vec3(0, -1000.5, 0), 1000, new lambertian(new constant_texture(vec3(0.5, 0.5, 0.5))));
    int i = 1;
    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            double choose_mat = drand48();
            vec3 center(a + 0.9 * drand48(), 0.2, b + 0.9 * drand48());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                if (choose_mat < 0.8) {
                    list[i++] = new moving_sphere(center, center + vec3(0, 0.5 * drand48(), 0), 0.0, 1.0, 0.2,
                                                  new lambertian(new constant_texture(
                                                          vec3(drand48() * drand48(), drand48() * drand48(),
                                                               drand48() * drand48()))));
                } else if (choose_mat < 0.95) {
                    list[i++] = new sphere(center, 0.2, new metal(
                            vec3(0.5 * (1 + drand48()), 0.5 * (1 + drand48()), 0.5 * (1 + drand48())),
                            0.5 * drand48()));
                } else {
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }
    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
    return new hitable_list(list, i);
}

int main(int argv, char *argc[]) {
    int nx = 1920, ny = 1080, ns = 60;
    std::ofstream fout = get_out(argc);
    fout << "P3\n" << nx << " " << ny << "\n255\n";

    vec3 look_from(13, 2, 3);
    vec3 look_at(0, 0, 0);
    double dist_to_focus = 10;
    double aperture = 0.1;
    camera cam(look_from, look_at, vec3(0, 1, 0), 20, double(nx) / double(ny), aperture, dist_to_focus, 0.0, 1.0);
    hitable *world = random_scene();
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
