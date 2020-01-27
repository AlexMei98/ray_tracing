#include "ray.hpp"
#include "util.hpp"
#include "camera.hpp"

#include "hitable.hpp"
#include "hitables/sphere.hpp"
#include "hitables/hitable_list.hpp"

#include "materials/lambertian.hpp"

#include "aabb.hpp"
#include "textures/constant_texture.hpp"
#include "textures/noise_texture.hpp"
#include "textures/image_texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.hpp"

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

hitable *perlin_scene() {
    int nx, ny, nn;
    unsigned char *image_data = stbi_load("earth.jpg", &nx, &ny, &nn, 0);
    texture *perlin_texture = new noise_texture(1);
    texture *earth_texture = new image_texture(image_data, nx, ny);
    auto **list = new hitable*[2];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(perlin_texture));
    list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(earth_texture));
    return new hitable_list(list, 2);
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
    hitable *world = perlin_scene();
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
