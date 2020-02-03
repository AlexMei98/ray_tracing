#include <thread>
#include <mutex>
#include <unistd.h>
#include <chrono>

#include "ray.hpp"
#include "util.hpp"
#include "camera.hpp"

#include "hitable.hpp"
#include "hitables/sphere.hpp"
#include "hitables/moving_sphere.hpp"
#include "hitables/hitable_list.hpp"
#include "hitables/rectangle.hpp"
#include "hitables/box.hpp"
#include "hitables/exchange.hpp"
#include "hitables/medium.hpp"
#include "hitables/bvh_node.hpp"

#include "materials/metal.hpp"
#include "materials/lambertian.hpp"
#include "materials/diffuse_light.hpp"
#include "materials/dielectric.hpp"

#include "aabb.hpp"
#include "textures/constant_texture.hpp"
#include "textures/image_texture.hpp"
#include "textures/noise_texture.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.hpp"

#define time_stub(t) auto t = std::chrono::system_clock::now()
#define delta(start, end) std::chrono::duration<double>(end - start).count()


int x, y, s, p, total;
vec3 *map;
bool *flag;
volatile int front = 0;
std::mutex mut;
std::ofstream fout;

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

hitable *final_scene() {
    int nb = 20;
    int ns = 1000;
    auto **list = new hitable*[30];
    auto **box_list = new hitable*[nb * nb];
    auto **box_list_2 = new hitable*[ns];
    material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
    material *ground = new lambertian(new constant_texture(vec3(0.48, 0.83, 0.53)));
    int b = 0;
    for (int i = 0; i < nb; i++) {
        for (int j = 0; j < nb; j++) {
            double w = 100;
            double x0 = -1000 + i * w;
            double z0 = -1000 + j * w;
            double y0 = 0;
            double x1 = x0 + w;
            double y1 = 100 * (drand48() + 0.01);
            double z1 = z0 + w;
            box_list[b++] = new box(vec3(x0, y0, z0), vec3(x1, y1, z1), ground);
        }
    }
    int l = 0;
    list[l++] = new bvh_node(box_list, b, 0, 1);
    material *light = new diffuse_light(new constant_texture(vec3(7, 7, 7)));
    list[l++] = new xz_rectangle(123, 423, 147, 412, 554, light);
    vec3 center(400, 400, 200);
    list[l++] = new moving_sphere(center, center + vec3(30, 0, 0), 0, 1, 50, new lambertian(new constant_texture(vec3(0.7, 0.3, 0.1))));
    list[l++] = new sphere(vec3(260, 150, 45), 50, new dielectric(1.5));
    list[l++] = new sphere(vec3(0, 150, 145), 50, new metal(vec3(0.8, 0.8, 0.9), 10.0));
    hitable *boundary = new sphere(vec3(360, 150, 145), 70, new dielectric(1.5));
    list[l++] = boundary;
    list[l++] = new constant_medium(boundary, 0.2, new constant_texture(vec3(0.2, 0.4, 0.9)));
    boundary = new sphere(vec3(0, 0, 0), 5000, new dielectric(1.5));
    list[l++] = new constant_medium(boundary, 0.0001, new constant_texture(vec3(1.0, 1.0, 1.0)));
    int nx, ny, nn;
    unsigned char *earth_data = stbi_load("earth.jpg", &nx, &ny, &nn, 0);
    material *earth_mat = new lambertian(new image_texture(earth_data, nx, ny));
    list[l++] = new sphere(vec3(400, 200, 400), 100, earth_mat);
    texture *perlin = new noise_texture(0.2);
    list[l++] = new sphere(vec3(220, 280, 300), 80, new lambertian(perlin));
    for (int j = 0; j < ns; j++) {
        box_list_2[j] = new sphere(vec3(165 * drand48(), 165 * drand48(), 165 * drand48()), 10, white);
    }
    list[l++] = new translate(new rotate_y(new bvh_node(box_list_2, ns, 0.0, 1.0), 15), vec3(-100, 270, 395));
    return new hitable_list(list, l);
}

void compute(camera cam, hitable *world) {
    while (true) {
        int cur;
        {
            std::lock_guard<std::mutex> guard_front(mut);
            if (front < total) {
                cur = front++;
            } else {
                break;
            }
        }

        int tx = cur % x, ty = y - 1 - cur / x;
        for (int is = 0; is < s; is++) {
            double u = (double(tx) + drand48()) / double(x);
            double v = (double(ty) + drand48()) / double(y);
            map[cur] += color(cam.get_ray(u, v), world, 0);
        }
        map[cur] /= double(s);
        for (int i = 0; i < 3; i++) {
            map[cur][i] = sqrt(map[cur][i]);
        }
        flag[cur] = true;
    }
}

void final() {
    int cur = 0;
    fout << "P3\n" << x << " " << y << "\n255\n";
    while (cur < total) {
        if (flag[cur]) {
            int ir = int(255.99 * map[cur][0]);
            int ig = int(255.99 * map[cur][1]);
            int ib = int(255.99 * map[cur][2]);
            fout << ir << " " << ig << " " << ib << "\n";
            fout.flush();
            cur++;
            if (cur % 10000 == 0) {
                std::cout << "finish " << cur << " / " << total << std::endl;
                std::cout.flush();
            }
        } else {
            usleep(10);
        }
    }
}

int main(int argv, char *argc[]) {
    x = 2 * s2i(argc[1]), y = 2 * s2i(argc[2]), s = s2i(argc[3]), p = s2i(argc[4]);
    total = x * y;
    flag = new bool[total];
    std::memset(flag, 0, sizeof(bool));
    map = new vec3[total];
    front = 0;

    fout = get_out(argc);
    vec3 look_from(478, 278, -600);
    vec3 look_at(278, 278, 0);
    double dist_to_focus = (look_at - look_from).length();
    double aperture = 0.0;
    double fov = 40.0;
    camera cam(look_from, look_at, vec3(0, 1, 0), fov, double(x) / double(y), aperture, dist_to_focus, 0.0, 1.0);
    time_stub(scene_start);
    hitable *world = final_scene();
    time_stub(scene_end);
    std::cout << delta(scene_start, scene_end) << std::endl;

    std::cout << "process" << std::endl;
    std::cout.flush();
    std::thread t[p];

    time_stub(process_start);
    for (int i = 0; i < p; ++i) {
        t[i] = std::thread(compute, cam, world);
    }
    std::thread out(final);
    for (int i = 0; i < p; ++i) {
        t[i].join();
    }
    out.join();
    time_stub(process_end);
    std::cout << delta(process_start, process_end) << std::endl;

    return 0;
}
