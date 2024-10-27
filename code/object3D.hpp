#ifndef __OBJ_H__
#define __OBJ_H__

#include "utils.hpp"
#include "ray.hpp"
#include "curve.hpp"
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <utility>
#include <sstream>
using namespace std;
class Object
{
public:
	Texture texture;
	Object(Texture t) : texture(t) {}
	Object(Refl_t refl, Vec3 color, Vec3 emission, double brdf, string tname) : texture(tname, brdf, color, emission, refl) {}
	virtual pair<double, Vec3> intersect(Ray) {}
	virtual pair<Vec3, Vec3> AABBbox() {}
	virtual Vec3 norm(Vec3) {}
	virtual Vec3 bezier_trans(Vec3 p){};
};

class Sphere : public Object
{
public:
	Vec3 o;
	double r;
	Sphere(Vec3 o_, double r_, Texture t) : o(o_), r(r_), Object(t) {}
	Sphere(Vec3 o_, double r_, Refl_t refl, double brdf = 1.5, Vec3 color = Vec3(), Vec3 emission = Vec3(), string tname = "") : o(o_), r(r_), Object(refl, color, emission, brdf, tname) {}
	virtual pair<double, Vec3> intersect(Ray ray)
	{
		Vec3 ro = o - ray.o;
		double b = ray.d.dot(ro);
		double d = sqr(b) - ro.dot(ro) + sqr(r);
		if (d < 0)
			return make_pair(INF, Vec3());
		else
			d = sqrt(d);
		double t = b - d > eps ? b - d : b + d > eps ? b + d
													 : -1;
		if (t < 0)
			return make_pair(INF, Vec3());
		return make_pair(t, ray.get(t));
	}
	virtual pair<Vec3, Vec3> AABBbox()
	{
		return make_pair(o - r, o + r);
	}
	virtual Vec3 norm(Vec3 p)
	{
		double d = abs((p - o).len() - r);
		return (p - o).norm();
	}
};

class Triangle : public Object
{
public:
	Vec3 v0, v1, v2; // Vertices
	Vec3 normal;	 // Normal

	Triangle(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2, Refl_t refl, double brdf = 1.5, Vec3 color = Vec3(), Vec3 emission = Vec3(), string tname = "") : v0(v0), v1(v1), v2(v2), Object(refl, color, emission, brdf, tname)
	{
		normal = (v1 - v0).cross(v2 - v0).norm();
	}

	virtual pair<double, Vec3> intersect(Ray ray)
	{
		Vec3 edge1 = v1 - v0;
		Vec3 edge2 = v2 - v0;

		Vec3 h = ray.d.cross(edge2);
		double a = edge1.dot(h);

		if (a > -eps && a < eps)
			return make_pair(INF, Vec3());

		double f = 1.0 / a;
		Vec3 s = ray.o - v0;
		double u = f * s.dot(h);

		if (u < 0.0 || u > 1.0)
			return make_pair(INF, Vec3());

		Vec3 q = s.cross(edge1);
		double v = f * ray.d.dot(q);

		if (v < 0.0 || u + v > 1.0)
			return make_pair(INF, Vec3());

		double t = f * edge2.dot(q);

		if (t > eps)
			return make_pair(t, ray.get(t));
		else
			return make_pair(INF, Vec3());
	}

	virtual pair<Vec3, Vec3> AABBbox()
	{
		Vec3 min = v0.min(v1).min(v2);
		Vec3 max = v0.max(v1).max(v2);
		return make_pair(min, max);
	}

	virtual Vec3 norm(Vec3 p)
	{
		return normal;
	}
};

class Mesh : public Object
{
public:
	vector<Triangle> triangles;
	pair<Vec3, Vec3> boundingBox;
	double scale_factor;
	Vec3 translation;

	Mesh(const char *filename, Refl_t refl, double brdf = 1.5, Vec3 color = Vec3(), Vec3 emission = Vec3(), string tname = "", double scale = 1.0, const Vec3 &translation = Vec3(0, 0, 0)) : Object(refl, color, emission, brdf, tname), scale_factor(scale), translation(translation)
	{
		loadObj(filename);
		calculateBoundingBox();
	}
	void loadObj(const char *filename)
	{
		ifstream objFile(filename);

		if (!objFile)
		{
			cerr << "Error: Unable to open file \"" << filename << "\"." << endl;
			return;
		}

		vector<Vec3> vertices;
		vector<Vec3> normals;

		string line;
		while (getline(objFile, line))
		{
			istringstream iss(line);
			string token;

			iss >> token;

			if (token == "v")
			{
				Vec3 vertex;
				iss >> vertex.x >> vertex.y >> vertex.z;
				// Apply scale and translation
				vertex = vertex * scale_factor + translation;
				vertices.push_back(vertex);
			}
			else if (token == "vn")
			{
				Vec3 normal;
				iss >> normal.x >> normal.y >> normal.z;
				normals.push_back(normal);
			}
			else if (token == "f")
			{
				int v0, v1, v2;
				int n0, n1, n2;
				char slash;

				iss >> v0 >> v1 >> v2;
				triangles.push_back(Triangle(vertices[v0 - 1], vertices[v1 - 1], vertices[v2 - 1],
											 DIFF, 1.7, Vec3(.5, 1, .5) * .999));
			}
		}
	}

	void calculateBoundingBox()
	{
		if (triangles.empty())
		{
			boundingBox = make_pair(Vec3(), Vec3());
			return;
		}

		Vec3 min = triangles[0].AABBbox().first;
		Vec3 max = triangles[0].AABBbox().second;

		for (Triangle &triangle : triangles)
		{
			Vec3 t_min = triangle.AABBbox().first;
			Vec3 t_max = triangle.AABBbox().second;

			min = min.min(t_min);
			max = max.max(t_max);
		}

		boundingBox = make_pair(min, max);
	}

	virtual pair<double, Vec3> intersect(Ray ray)
	{
		// Check if ray intersects with the bounding box
		auto [t_min, t_max] = boundingBox;
		double t1 = (t_min.x - ray.o.x) / ray.d.x;
		double t2 = (t_max.x - ray.o.x) / ray.d.x;
		double t3 = (t_min.y - ray.o.y) / ray.d.y;
		double t4 = (t_max.y - ray.o.y) / ray.d.y;
		double t5 = (t_min.z - ray.o.z) / ray.d.z;
		double t6 = (t_max.z - ray.o.z) / ray.d.z;

		double t_enter = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
		double t_exit = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));

		if (t_exit < 0 || t_enter > t_exit)
		{
			// No intersection with the bounding box
			return make_pair(INF, Vec3());
		}

		double t_min_obj = INF;
		Vec3 p_min_obj;

		int tmp = 0;

		for (Triangle &triangle : triangles)
		{
			// Check if ray intersects with the triangle
			auto [t, p] = triangle.intersect(ray);
			if (t < t_min_obj)
			{
				if (tmp == 2)
					break;
				tmp++;
				t_min_obj = t;
				p_min_obj = p;
			}
		}

		return make_pair(t_min_obj, p_min_obj);
	}

	virtual pair<Vec3, Vec3> AABBbox()
	{
		if (triangles.empty())
			return make_pair(Vec3(), Vec3());

		Vec3 min = triangles[0].AABBbox().first;
		Vec3 max = triangles[0].AABBbox().second;

		for (Triangle &triangle : triangles)
		{
			Vec3 t_min = triangle.AABBbox().first;
			Vec3 t_max = triangle.AABBbox().second;

			min = min.min(t_min);
			max = max.max(t_max);
		}

		return make_pair(min, max);
	}

	virtual Vec3 norm(Vec3 p)
	{
		for (Triangle &triangle : triangles)
		{

			if (triangle.intersect(Ray(p, -triangle.normal)).first < INF)
			{
				return triangle.norm(p);
			}
		}
	}
};

class BezierObject : public Object
{
public:
	CurveBezier curve;
	Vec3 pos;
	BezierObject(Vec3 pos_, CurveBezier c_, Texture t) : pos(pos_), curve(c_), Object(t) {}
	BezierObject(Vec3 pos_, CurveBezier c_, Refl_t refl, double brdf = 1.5, Vec3 color = Vec3(), Vec3 emission = Vec3(), string tname = "") : pos(pos_), curve(c_), Object(refl, color, emission, brdf, tname) {}
	virtual Vec3 bezier_trans(Vec3 point)
	{
		double t_val = 0.5, f_t, df_t;
		double u_val = atan2(point.z - pos.z, point.x - pos.x);
		if (u_val < 0)
			u_val += 2 * PI;

		for (int i = 0; i < 10; ++i)
		{
			if (t_val < 0)
				t_val = 0;
			else if (t_val > 1)
				t_val = 1;
			f_t = curve.getpos(t_val).y - (point.y - pos.y);
			df_t = curve.getdir(t_val).y;
			if (abs(f_t) < eps)
				return Vec3(u_val, t_val);
			t_val -= f_t / df_t;
		}
		return Vec3(u_val, -1);
	}

	virtual pair<double, Vec3> intersect(Ray ray)
	{
		double final_dis = INF;
		double a = 0, b = 0, c = 0;
		double t1_offset = ray.o.x - pos.x - ray.d.x / ray.d.y * ray.o.y;
		double t2_offset = ray.d.x / ray.d.y;
		a += t2_offset * t2_offset;
		b += 2 * t1_offset * t2_offset;
		c += t1_offset * t1_offset;

		t1_offset = ray.o.z - pos.z - ray.d.z / ray.d.y * ray.o.y;
		t2_offset = ray.d.z / ray.d.y;
		a += t2_offset * t2_offset;
		b += 2 * t1_offset * t2_offset;
		c += t1_offset * t1_offset;

		c = c - b * b / (4 * a);
		b = -b / (2 * a) - pos.y;

		if ((0 <= b && b <= curve.height && c > (curve.max * curve.max)) || ((b < 0 || b > curve.height) && min(b * b, (curve.height - b) * (curve.height - b)) * a + c > (curve.max * curve.max)))
		{
			return make_pair(INF, Vec3());
		}

		for (int ind = 0; ind <= curve.num; ++ind)
		{
			double t0 = curve.data[ind].t0;
			double t1 = curve.data[ind].t1;
			double t_mid1 = (t0 + t1 + t0) / 3;
			double t_mid2 = (t1 + t0 + t1) / 3;

			double t = newton(t_mid1, a, b, c, t0, t1);
			Vec3 loc = curve.getpos(t);
			double dis = (pos.y + loc.y - ray.o.y) / ray.d.y;
			Vec3 inter_p = ray.get(dis);
			if (t > 0 && t < 1 && abs(loc.x - sqrt(a * sqr(loc.y - b) + c)) <= eps && dis >= eps && abs((Vec3(pos.x, pos.y + loc.y, pos.z) - inter_p).len2() - loc.x * loc.x) <= eps && dis < final_dis)
			{
				final_dis = dis;
			}

			t = newton(t_mid2, a, b, c, t0, t1);
			loc = curve.getpos(t);
			dis = (pos.y + loc.y - ray.o.y) / ray.d.y;
			inter_p = ray.get(dis);
			if (t > 0 && t < 1 && abs(loc.x - sqrt(a * sqr(loc.y - b) + c)) <= eps && dis >= eps && abs((Vec3(pos.x, pos.y + loc.y, pos.z) - inter_p).len2() - loc.x * loc.x) <= eps && dis < final_dis)
			{
				final_dis = dis;
			}
		}

		if (final_dis < INF / 2)
			return make_pair(final_dis, ray.get(final_dis));
		else
			return make_pair(INF, Vec3());
	}
	double newton(double t, double a, double b, double c, double low = eps, double upp = 1 - eps)
	{
		double ft, dft, x, y, dx, dy, sq;
		Vec3 loc, dir;
		for (int i = 10; i--;)
		{
			if (t < 0)
				t = low;
			if (t > 1)
				t = upp;
			loc = curve.getpos(t), dir = curve.getdir(t);
			x = loc.x, dx = dir.x;
			y = loc.y, dy = dir.y;
			sq = sqrt(a * sqr(y - b) + c);
			ft = x - sq;
			dft = dx - a * (y - b) * dy / sq;
			if (abs(ft) < eps)
				return t;
			t -= ft / dft;
		}
		return -1;
	}
	virtual pair<Vec3, Vec3> AABBbox()
	{
		return make_pair(Vec3(pos.x - curve.max, pos.y, pos.z - curve.max), Vec3(pos.x + curve.max, pos.y + curve.height, pos.z + curve.max));
	}
	virtual Vec3 norm(Vec3 point)
	{
		double t_val = 0.5, f_t, df_t;
		double u_val = atan2(point.z - pos.z, point.x - pos.x);
		if (u_val < 0)
			u_val += 2 * PI;

		for (int i = 0; i < 10; ++i)
		{
			if (t_val < 0)
				t_val = 0;
			else if (t_val > 1)
				t_val = 1;
			f_t = curve.getpos(t_val).y - (point.y - pos.y);
			df_t = curve.getdir(t_val).y;
			if (abs(f_t) < eps)
				break;
			t_val -= f_t / df_t;
		}

		Vec3 tmp = Vec3(u_val, t_val);
		Vec3 dir = curve.getdir(tmp.y);
		Vec3 d_surface = Vec3(cos(tmp.x), dir.y / dir.x, sin(tmp.x));
		Vec3 d_circ = Vec3(-sin(tmp.x), 0, cos(tmp.x));
		return d_circ.cross(d_surface).norm();
	}
};


class Cube : public Object
{
public:
	Vec3 m0, m1;
	Cube(Vec3 m0_, Vec3 m1_, Texture t) : m0(min(m0_, m1_)), m1(max(m0_, m1_)), Object(t) {}
	Cube(Vec3 m0_, Vec3 m1_, Refl_t refl, double brdf = 1.5, Vec3 color = Vec3(), Vec3 emission = Vec3(), string tname = "") : m0(min(m0_, m1_)), m1(max(m0_, m1_)), Object(refl, color, emission, brdf, tname) {}
	virtual Vec3 norm(Vec3 p)
	{
		if (abs(p.x - m0.x) < eps || abs(p.x - m1.x) < eps)
			return Vec3(abs(p.x - m1.x) < eps ? 1 : -1, 0, 0);
		if (abs(p.y - m0.y) < eps || abs(p.y - m1.y) < eps)
			return Vec3(0, abs(p.y - m1.y) < eps ? 1 : -1, 0);
		if (abs(p.z - m0.z) < eps || abs(p.z - m1.z) < eps)
			return Vec3(0, 0, abs(p.z - m1.z) < eps ? 1 : -1);
		assert(1 == 0);
	}
	virtual pair<double, Vec3> intersect(Ray ray)
	{
		double ft = INF, t;
		Vec3 fq = Vec3(), q;
		t = (m0.x - ray.o.x) / ray.d.x;
		if (0 < t && t < ft)
		{
			q = ray.get(t);
			if (m0.y <= q.y && q.y <= m1.y && m0.z <= q.z && q.z <= m1.z)
				ft = t, fq = q;
		}
		t = (m1.x - ray.o.x) / ray.d.x;
		if (0 < t && t < ft)
		{
			q = ray.get(t);
			if (m0.y <= q.y && q.y <= m1.y && m0.z <= q.z && q.z <= m1.z)
				ft = t, fq = q;
		}
		t = (m0.y - ray.o.y) / ray.d.y;
		if (0 < t && t < ft)
		{
			q = ray.get(t);
			if (m0.x <= q.x && q.x <= m1.x && m0.z <= q.z && q.z <= m1.z)
				ft = t, fq = q;
		}
		t = (m1.y - ray.o.y) / ray.d.y;
		if (0 < t && t < ft)
		{
			q = ray.get(t);
			if (m0.x <= q.x && q.x <= m1.x && m0.z <= q.z && q.z <= m1.z)
				ft = t, fq = q;
		}
		t = (m0.z - ray.o.z) / ray.d.z;
		if (0 < t && t < ft)
		{
			q = ray.get(t);
			if (m0.x <= q.x && q.x <= m1.x && m0.y <= q.y && q.y <= m1.y)
				ft = t, fq = q;
		}
		t = (m1.z - ray.o.z) / ray.d.z;
		if (0 < t && t < ft)
		{
			q = ray.get(t);
			if (m0.x <= q.x && q.x <= m1.x && m0.y <= q.y && q.y <= m1.y)
				ft = t, fq = q;
		}
		return make_pair(ft, fq);
	}
	virtual pair<Vec3, Vec3> AABBbox()
	{
		return make_pair(m0, m1);
	}
};


#endif // __OBJ_H__
