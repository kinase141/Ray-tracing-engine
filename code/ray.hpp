#ifndef __RAY_H__
#define __RAY_H__ 

#include "utils.hpp"

class Ray
{
public:
	Vec3 o, d;
	Ray(Vec3 o_, Vec3 d_): o(o_), d(d_) {}
	Vec3 get(double t) { return o + d * t; }
};

#endif // __RAY_H__