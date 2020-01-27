#include <iostream>

#include "sphere.hpp"
#include "histable_list.hpp"
#include "camera.hpp"
#include "util.hpp"
#include "materials/lambertian.hpp"

vec3 color(const ray &r, hitable *world) {
    hit_record rec;
    if (world->hit(r, 0.0001, MAXFLOAT, rec)) {
        vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        return 0.5 * color(ray(rec.p, target - rec.p), world);
    } else {
        vec3 unit_direction = unit_vector(r.direction());
        double t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

int main(int argc, char* argv[]) {
    int nx = 2000, ny = 1000, ns = 100;
    std::ofstream fout = get_out(argv);
    fout << "P3\n" << nx << " " << ny << "\n255\n";
    camera cam;
    hitable *list[2];
    list[0] = new sphere(vec3(0.0, 0.0, -1), 0.5, new lambertian(vec3(0.8, 0.6, 0.2)));
    list[1] = new sphere(vec3(0.0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    hitable_list *world;
    world = new hitable_list(list, 2);
    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; ++s) {
                double u = double(i + drand48()) / double(nx);
                double v = double(j + drand48()) / double(ny);
                ray r = cam.get_ray(u, v);
                col += color(r, world);
            }
            col /= double(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            fout << ir << " " << ig << " " << ib << "\n";
        }
    }
}
