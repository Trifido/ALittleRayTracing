// RayTracingOneWeekend.cpp: define el punto de entrada de la aplicación de consola.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <random>
#include <thread>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "Sphere.h"
#include "Hitable_list.h"
#include "Camera.h"

#include "Lambertian.h"
#include "Metal.h"
#include "Dielectric.h"

using namespace std;

vec3 color(const Ray &r, hitable *world, int depth)
{
	hit_record rec;

	if (world->hit(r, 0.001f, FLT_MAX, rec))
	{
		Ray scattered;
		vec3 attenuation;

		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * color(scattered, world, depth + 1);
		}
		else
		{
			return vec3(0.0f, 0.0f, 0.0f);
		}
	}
	else
	{
		vec3 unitDirection = unit_vector(r.direction());
		float t = 0.5f * (unitDirection.y() + 1.0f);
		return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
	}
}

hitable *random_scene()
{
	int n = 500;
	hitable **list = new hitable*[n+1];
	list[0] = new Sphere(vec3(0, -1000, 0), 1000, new Lambertian(vec3(0.5, 0.5, 0.5)));
	int i = 1;

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			float choose_mat = rand() / (RAND_MAX + 1.0);
			vec3 center(a + 0.9*(rand() / (RAND_MAX + 1.0)), 0.2, b + 0.9*(rand() / (RAND_MAX + 1.0)));
			if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
				if (choose_mat < 0.8) {  // diffuse
					list[i++] = new Sphere(center, 0.2, new Lambertian(vec3((rand() / (RAND_MAX + 1.0))*(rand() / (RAND_MAX + 1.0)), (rand() / (RAND_MAX + 1.0))*(rand() / (RAND_MAX + 1.0)), (rand() / (RAND_MAX + 1.0))*(rand() / (RAND_MAX + 1.0)))));
				}
				else if (choose_mat < 0.95) { // metal
					list[i++] = new Sphere(center, 0.2,
						new Metal(vec3(0.5*(1 + (rand() / (RAND_MAX + 1.0))), 0.5*(1 + (rand() / (RAND_MAX + 1.0))), 0.5*(1 + (rand() / (RAND_MAX + 1.0)))), 0.5*(rand() / (RAND_MAX + 1.0))));
				}
				else {  // glass
					list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new Sphere(vec3(0, 1, 0), 1.0, new Dielectric(1.5));
	list[i++] = new Sphere(vec3(-4, 1, 0), 1.0, new Lambertian(vec3(0.4, 0.2, 0.1)));
	list[i++] = new Sphere(vec3(4, 1, 0), 1.0, new Metal(vec3(0.7, 0.6, 0.5), 0.0));

	return new Hitable_list(list, i);
}

void Render(int nx, int ny, int ns, int start, int end, Camera &cam, hitable *world, unsigned int workSize, unsigned int idThread, vec3 *resultThreads)
{
	int aux = 0;
	for (int j = start; j > end; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			vec3 finalColor(0.0f, 0.0f, 0.0f);
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + (rand() / (RAND_MAX + 1.0))) / float(nx);
				float v = float(j + (rand() / (RAND_MAX + 1.0))) / float(ny);
				Ray r = cam.GetRay(u, v);
				vec3 p = r.point_at_parameter(2.0f);
				finalColor += color(r, world, 0);
			}

			finalColor /= float(ns);
			finalColor = vec3(sqrt(finalColor[0]), sqrt(finalColor[1]), sqrt(finalColor[2]));

			int ir = int(255.99*finalColor[0]);
			int ig = int(255.99*finalColor[1]);
			int ib = int(255.99*finalColor[2]);

			resultThreads[aux].e[0] = ir;
			resultThreads[aux].e[1] = ig;
			resultThreads[aux].e[2] = ib;
			//+= vec3(ir, ig, ib);// .push_back(vec3(ir, ig, ib));
			aux++;
			//myImage << ir << " " << ig << " " << ib << "\n";
		}
	}

	//myImage.close();
}

int main()
{
	const int nx = 600;
	const int ny = 400;
	const int ns = 100;

	//ofstream myImage;
	//myImage.open("image.ppm");
	//myImage << "P3\n" << nx << " " << ny << "\n255\n";

	hitable *list[5];

	//float R = cos(std::_Pi / 4);
	//list[0] = new Sphere(vec3(-R, 0, -1), R, new Lambertian(vec3(0, 0, 1)));
	//list[1] = new Sphere(vec3(R, 0, -1), R, new Lambertian(vec3(1, 0, 0)));
	list[0] = new Sphere(vec3(0.0f, 0.0f, -1.0f), 0.5f, new Lambertian(vec3(0.8f, 0.3f, 0.3f)));
	list[1] = new Sphere(vec3(0.0f, -100.5f, -1.0f), 100.0f, new Lambertian(vec3(0.8f, 0.8f, 0.0f)));
	list[2] = new Sphere(vec3(1.0f, 0.0f, -1.0f), 0.5f, new Metal(vec3(0.8f, 0.6f, 0.2f), 0.5f));
	list[3] = new Sphere(vec3(-1.0f, 0.0f, -1.0f), 0.5f, new Dielectric(1.5f));
	list[4] = new Sphere(vec3(-1.0f, 0.0f, -1.0f), -0.45f, new Dielectric(1.5f));

	//hitable *world = new Hitable_list(list, 5);
	
	hitable *world = random_scene();

	vec3 lookfrom(13, 2, 3);
	vec3 lookat(0, 0, 0);
	float vfov = 20;
	float dist_to_focus = 10.0f;// (lookfrom - lookat).length();
	float aperture = 0.1f;
	Camera cam(lookfrom, lookat, vec3(0,1,0), vfov, float(nx)/float(ny), aperture, dist_to_focus);

	ofstream myImage;
	myImage.open("image.ppm");
	myImage << "P3\n" << nx << " " << ny << "\n255\n";

	
	unsigned int numThreads = std::thread::hardware_concurrency();
	std::cout << " number of cores: " << numThreads << endl;

	unsigned int workSize = std::ceil((float)ny / (float)numThreads);

	std::vector<std::thread> renderThreads;
	const int TAM = nx * ny;
	std::vector< vec3* > resultThreads;
	resultThreads.resize(numThreads);

	for (int i = 0; i < numThreads; i++)
	{
		int start = ny - (workSize * i) /*- 1*/;
		int end;

		end = ny - (workSize * (i + 1)) /*- 1*/;

		if (end < 0)
			end = 0;

		resultThreads[i] = new vec3[nx * (start-end)];

		renderThreads.push_back(std::thread(Render, nx, ny, ns, start, end, cam, world, workSize, i, resultThreads[i]));
	}

	for (int i = 0; i < numThreads; i++)
	{
		renderThreads.at(i).join();
	}

	for (int j = 0; j < numThreads; j++)
	{
		int start = ny - (workSize *j) /*- 1*/;
		int end;

		end = ny - (workSize * (j + 1)) /*- 1*/;

		if (end < 0)
			end = 0;

		for (unsigned int i = 0; i < (nx * (start - end)); i++)
		{
			vec3 pixel = resultThreads[j][i];
			myImage << pixel.x() << " " << pixel.y() << " " << pixel.z() << "\n";
		}

		delete(resultThreads[j]);
	}

	myImage.close();

	//for (int j = ny - 1; j >= 0; j--)
	//{
	//	for (int i = 0; i < nx; i++)
	//	{
	//		vec3 finalColor(0.0f, 0.0f, 0.0f);
	//		for (int s = 0; s < ns; s++)
	//		{
	//			float u = float(i + (rand() / (RAND_MAX + 1.0))) / float(nx);
	//			float v = float(j + (rand() / (RAND_MAX + 1.0))) / float(ny);
	//			Ray r = cam.GetRay(u, v);
	//			vec3 p = r.point_at_parameter(2.0f);
	//			finalColor += color(r, world, 0);
	//		}

	//		finalColor /= float(ns);
	//		finalColor = vec3(sqrt(finalColor[0]), sqrt(finalColor[1]), sqrt(finalColor[2]));

	//		int ir = int(255.99*finalColor[0]);
	//		int ig = int(255.99*finalColor[1]);
	//		int ib = int(255.99*finalColor[2]);

	//		myImage << ir << " " << ig << " " << ib << "\n";
	//	}
	//}

	//myImage.close();

    return 0;
}

