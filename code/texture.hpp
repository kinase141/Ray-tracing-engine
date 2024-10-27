#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "utils.hpp"
using namespace std;
class Texture
{
public:
	Vec3 color, emission;
	Refl_t refl;
	double brdf;
	string filename;
	unsigned char *buf;
	int w, h, c;
	Texture(const Texture &t) : brdf(t.brdf), filename(t.filename), color(t.color), emission(t.emission), refl(t.refl)
	{
		buf = t.filename != "" ? stbi_load(filename.c_str(), &w, &h, &c, 0) : NULL;
	}
	Texture(string _, double b, Vec3 col, Vec3 e, Refl_t r) : brdf(b), filename(_), color(col), emission(e), refl(r)
	{
		buf = _ != "" ? stbi_load(filename.c_str(), &w, &h, &c, 0) : NULL;
	}
	pair<Refl_t, Vec3> getcol(double a, double b)
	{
		if (buf == NULL)
			return make_pair(refl, color);
		int x = buf[((int(a * w) % w + w) % w) * c + ((int(b * h) % h + h) % h) * w * c + 0];
		int y = buf[((int(a * w) % w + w) % w) * c + ((int(b * h) % h + h) % h) * w * c + 1];
		int z = buf[((int(a * w) % w + w) % w) * c + ((int(b * h) % h + h) % h) * w * c + 2];
		if (x == 233 && y == 233 && z == 233)
		{
			return make_pair(SPEC, Vec3(1, 1, 1) * .999);
		}
		else
			return make_pair(refl, Vec3(x, y, z) / 255.);
	}
};

#endif // __TEXTURE_H__
