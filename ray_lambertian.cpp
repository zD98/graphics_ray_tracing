#include <iostream>
#include <fstream>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "float.h"


vec3 random_int_unit_sphere() {
    vec3 p;
    do {
        p = 2.0 * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
    } while (p.squared_length() >= 1.0);
    return p;
}

vec3 color(const ray& r, hitable *world) {
    hit_record rec;
    if (world->hit(r, 0.0, MAXFLOAT, rec)) {
        vec3 target = rec.p + rec.normal + random_int_unit_sphere();
        return 0.5 * color(ray(rec.p, target - rec.p), world);
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
        
    
    }
}
int main() {
    std::ofstream testfile;
    testfile.open("test_ray_lambertian.ppm");
    int nx = 200;
    int ny = 100;
    int ns = 100;
    testfile << "P3\n" << nx << " " << ny << "\n255\n";
    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);
    
    hitable *list[2];
    list[0] = new sphere(vec3(0,0,-1), 0.5);
    list[1] = new sphere(vec3(0,-100.5,-1), 100);
    hitable *world = new hitable_list(list, 2);
    camera cam;
    for(int j = ny -1; j >= 0; j--) {
        for(int i = 0; i < nx; i ++ ) {
            vec3 col(0,0,0);
            for(int s=0; s<ns;s++) {
                float u = float(i + drand48()) / float(nx);
                float v = float(j + drand48()) / float(ny);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0);
                col += color(r, world);
            }

            col /= float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            // 原文示例将r扩大了2倍, 未做归一化, 所以在hit计算时, 也没有*2            // 但是我们保持了正常的算法 
            // 不知道原文在搞什么鬼
            // vec3 p = r.point_at_parameter(2.0);

            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            testfile << ir << " " << ig << " " << ib << "\n";
        }
    }
    testfile.close();
    return 0;
}
