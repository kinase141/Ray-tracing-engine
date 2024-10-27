#ifndef __RENDER_H__
#define __RENDER_H__ 

#include "texture.hpp"
#include "scene.hpp"
using namespace std;

Vec3 PTradience(Ray ray, int dep, unsigned short *X) {

	int into = 0;
	double t = INF;
	int id = -1;
	for (int i = 0; i < scene_num; ++i) {
		pair<double, Vec3> tmp = scene[i]->intersect(ray);
		if (tmp.first < INF / 2 && tmp.second.len2() > eps && tmp.first < t) {
			t = tmp.first;
			id = i;
		}
	}
	pair<int, double> intersect_result = make_pair(id, t);
	if (intersect_result.first == -1)
		return Vec3();
	Object* obj = scene[intersect_result.first];
	Texture& texture = obj->texture;
	Vec3 x = ray.get(intersect_result.second);
	pair<Refl_t, Vec3> feature = get_feature(obj, texture, x, X);
	Vec3 f = feature.second, n = obj->norm(x), nl = n.dot(ray.d) < 0 ? into = 1, n : -n;
	double p = f.max();
	if (f.max() < eps)
		return texture.emission;
	if (++dep > 5)
		if (erand48(X) < p) f /= p;
		else return texture.emission;
	if (feature.first == DIFF) {
		double r1 = 2 * PI * erand48(X), r2 = erand48(X), r2s = sqrt(r2);
		Vec3 w = nl, u=((fabs(w.x) > .1 ? Vec3(0, 1) : Vec3(1)) & w).norm(), v = w & u;
		Vec3 d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).norm();

		return f.mult(PTradience(Ray(x, d), dep, X));
	}
	else {
		Ray reflray = Ray(x, ray.d.reflect(nl));
		if (feature.first == SPEC) {
			return f.mult(PTradience(reflray, dep, X));
		}
		else {
			Vec3 d = ray.d.refract(n, into ? 1 : texture.brdf, into ? texture.brdf : 1);
			if (d.len2() < eps) 
				return f.mult(PTradience(reflray, dep, X));
			double a = texture.brdf - 1, b = texture.brdf + 1;
			double R0 = a * a / (b * b), c = 1 - (into ? -ray.d.dot(nl) : d.dot(n));
			double Re = R0 + (1 - R0) * c * c  * c * c * c, Tr = 1 - Re;
			double P = .25 + .5 * Re, RP = Re / P, TP = Tr / (1 - P);
			return f.mult(dep > 2 ? (erand48(X) < P ?   
				PTradience(reflray, dep, X) * RP : PTradience(Ray(x, d), dep, X) * TP)
			  : PTradience(reflray, dep, X) * Re + PTradience(Ray(x, d), dep, X) * Tr);
		}
	}
}


#endif // __RENDER_H__
