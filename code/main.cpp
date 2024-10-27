#include "PT.hpp"
#include <chrono>
#include <iostream>

double aperture = 0.0;//景深参数

int main(int argc, char *argv[])
{
    auto start_time = std::chrono::high_resolution_clock::now();

    int image_width = atoi(argv[1]);
    int image_height = atoi(argv[2]);
    int samples = atoi(argv[4]);

	// Ray camera(Vec3(50, 52, 295.6), Vec3(0,-0.062612,-1).norm());   //陶瓷瓶效果图相机     
    Ray camera(Vec3(150, 28, 260), Vec3(-0.45, 0.001, -1).norm());//相机设置
    Vec3 cx = Vec3(image_width * 0.33 / image_height);
    Vec3 cy = (cx & Vec3(camera.d.x, 0, camera.d.z)).norm() * 0.33;
	Vec3 r ;
    cx *= 1.05;

    std::vector<Vec3> c(image_width * image_height, Vec3(0, 0, 0));

#pragma omp parallel for schedule(dynamic, 1)private(r)
    for (int y = 0; y < image_height; ++y)
    {
        fprintf(stderr, "\r%5.2f%%", 100. * y / image_height);

        for (int x = 0; x < image_width; ++x)
        {
            for (int sy = 0; sy < 2; ++sy)
            {
                for (int sx = 0; sx < 2; ++sx)
                {
                    unsigned short X[3] = {static_cast<unsigned short>(y + sx), static_cast<unsigned short>(y * x + sy), static_cast<unsigned short>(y * x * y + sx * sy + time(0))};
                    Vec3 r = {0, 0, 0};

                    for (int s = 0; s < samples; ++s)
                    {
                        double r1 = 2 * erand48(X);
                        double dx = r1 < 1 ? sqrt(r1) : 2 - sqrt(2 - r1);

                        double r2 = 2 * erand48(X);
                        double dy = r2 < 1 ? sqrt(r2) : 2 - sqrt(2 - r2);

                        Vec3 d = cx * ((sx + dx / 2 + x) / image_width - 0.5) + cy * ((sy + dy / 2 + y) / image_height - 0.5) + camera.d;
                        Vec3 pp = camera.o + d * 150;
                        Vec3 loc = camera.o + (Vec3(erand48(X) * 1.05, erand48(X)) - 0.5) * 2 * aperture;
                        r += PTradience(Ray(pp, (pp - loc).norm()), 0, X);
                    }

                    c[y * image_width + x] += (r / samples).clip() / 4;
                }
            }
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    int hours = duration / (1000 * 60 * 60);
    int minutes = (duration % (1000 * 60 * 60)) / (1000 * 60);
    int seconds = (duration % (1000 * 60)) / 1000;

    std::cout << std::endl
              << "Rendering time: " << hours << " hours, " << minutes << " minutes, " << seconds << " seconds" << std::endl;

    FILE *f = fopen(argv[3], "w");
    fprintf(f, "P6\n%d %d\n%d\n", image_width, image_height, 255);

    for (int y = image_height - 1; y >= 0; --y)
    {
        for (int x = image_width - 1; x >= 0; --x)
        {
            double r = c[y * image_width + x].x;
            double g = c[y * image_width + x].y;
            double b = c[y * image_width + x].z;

            r = std::pow((r < 0 ? 0 : (r > 1 ? 1 : r)), (1 / 2.2));
            g = std::pow((g < 0 ? 0 : (g > 1 ? 1 : g)), (1 / 2.2));
            b = std::pow((b < 0 ? 0 : (b > 1 ? 1 : b)), (1 / 2.2));

            int ir = int(.5 + 255 * r);
            int ig = int(.5 + 255 * g);
            int ib = int(.5 + 255 * b);

            fprintf(f, "%c%c%c", ir, ig, ib);
        }
    }

    fclose(f);
    return 0;
}
