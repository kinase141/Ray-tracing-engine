#ifndef __UTILS_H__
#define __UTILS_H__


#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <bits/stdc++.h>
#include <omp.h>
#include "stb_image.h"
using namespace std;
#endif // STB_IMAGE_IMPLEMENTATION

const double PI = acos(-1);
const double eps = 1e-6;
const double INF = 1 << 20;
enum Refl_t { DIFF, SPEC, REFR };
double sqr(double x) {return x * x;}

struct Vec3{
	double x, y, z;
	Vec3(double x_=0, double y_=0, double z_=0): x(x_), y(y_), z(z_) {}
	Vec3 operator-() const {return Vec3(-x, -y, -z);}
	Vec3 operator+(const Vec3&a) const {return Vec3(x+a.x, y+a.y, z+a.z);}
	Vec3 operator-(const Vec3&a) const {return Vec3(x-a.x, y-a.y, z-a.z);}
	Vec3 operator+(double p) const {return Vec3(x+p, y+p, z+p);}
	Vec3 operator-(double p) const {return Vec3(x-p, y-p, z-p);}
	Vec3 operator*(double p) const {return Vec3(x*p, y*p, z*p);}
	Vec3 operator/(double p) const {return Vec3(x/p, y/p, z/p);}
	bool operator==(const Vec3&a) const {return x==a.x && y==a.y && z==a.z;}
	bool operator!=(const Vec3&a) const {return x!=a.x || y!=a.y || z!=a.z;}
	Vec3&operator+=(const Vec3&a) {return *this = *this + a;}
	Vec3&operator-=(const Vec3&a) {return *this = *this - a;}
	Vec3&operator+=(double p) {return *this = *this + p;}
	Vec3&operator-=(double p) {return *this = *this - p;}
	Vec3&operator*=(double p) {return *this = *this * p;}
	Vec3&operator/=(double p) {return *this = *this / p;}
	double operator|(const Vec3&a) const {return x*a.x + y*a.y + z*a.z;}
	double dot(const Vec3&a) const {return x*a.x + y*a.y + z*a.z;}
	double max() const {return x>y&&x>z?x:y>z?y:z;}
	Vec3 max(const Vec3&a) const {return Vec3(std::max(x,a.x), std::max(y,a.y), std::max(z,a.z));}
	Vec3 min(const Vec3&a) const {return Vec3(std::min(x,a.x), std::min(y,a.y), std::min(z,a.z));}
	double len() const {return sqrt(x*x + y*y + z*z);}
	double len2() const {return x*x + y*y + z*z;}
	Vec3 mult(const Vec3&a) const {return Vec3(x*a.x, y*a.y, z*a.z);}
	Vec3 operator&(const Vec3&a) const {return Vec3(y*a.z-z*a.y, z*a.x-x*a.z, x*a.y-y*a.x);}
	Vec3 cross(const Vec3&a) const {return Vec3(y*a.z-z*a.y, z*a.x-x*a.z, x*a.y-y*a.x);}
	Vec3 norm() const {return (*this)/len();}
	Vec3 clip(double r0=0, double r1=1) const {return Vec3(x>r1?r1:x<r0?r0:x, y>r1?r1:y<r0?r0:y, z>r1?r1:z<r0?r0:z);}
	Vec3 reflect(const Vec3&n) const {return (*this)-n*2.*n.dot(*this);}
	Vec3 refract(const Vec3&n, double ni, double nr) const { 
		double cosi = this->norm().dot(n);
		double nir = ni / nr;
		double cosr2 = 1. - nir*nir*(1-cosi*cosi);
		if (cosr2 <= 0)
			return Vec3();
		double cosr = sqrt(cosr2);
		if (cosi > 0) 
			cosr = -cosr;
		return ((*this)*nir - n*(nir*cosi+cosr)).norm();
	}
};

Vec3 min(Vec3 a, Vec3 b) {return Vec3(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));}
Vec3 max(Vec3 a, Vec3 b) {return Vec3(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));}


#endif // __UTILS_H__
