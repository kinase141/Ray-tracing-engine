#ifndef __SCENE_H__
#define __SCENE_H__

#include "object3D.hpp"
#include "curve.hpp"
#include <unordered_map>

using namespace std;
Object *rabbit[] = {
	new Sphere(Vec3(1e5 + 1, 40.8, 81.6), 1e5, DIFF, 1.5, Vec3(.1, .25, .25)),															   // Left
	new Sphere(Vec3(50, 40.8, 1e5), 1e5, DIFF, 1.5, Vec3(.75, .75, .75), Vec3(), "resource/wallls.com_156455.png"),						   // Back
	new Sphere(Vec3(50, 40.8, -1e5 + 170), 1e5, DIFF, 1.5, Vec3(), Vec3(), "resource/wallls.com_156455.png"),							   // Frnt
	new Sphere(Vec3(50, 1e5, 81.6), 1e5, DIFF, 1.5, Vec3(.75, .75, .75), Vec3(), "resource/star.png"),									   // Botm
	new Sphere(Vec3(50, -1e5 + 81.6, 81.6), 1e5, DIFF, 1.5, Vec3(.75, .75, .75)),														   // Top
	new Sphere(Vec3(27, 16.5, 47), 16.5, REFR, 1.5, Vec3(1, 1, 1) * .999, Vec3(), "resource/rainbow.png"),								  
	new Mesh("resource/bunny_1k.obj", DIFF, 1.177, Vec3(0.1, 0.2, 0.4) * .999, Vec3(), "resource/pinkwall.png", 100.0, Vec3(85, -10, 78)), 
	new Sphere(Vec3(15, 681.6 - .27, 61.6), 600, DIFF, 1.5, Vec3(), Vec3(12, 12, 12)) // Lite

};

double c_x[] = {9.0, 5.0, 6.6667, 8.3333, 10.0, 16.6667, 13.3333, 5.0, 10.0};
double c_y[] = {0.0, 0.0, 4.0, 8.0, 12.0, 16.0, 24.0, 28.0, 32.0};
CurveBezier vaseCurve(c_x, c_y, 9, 9, .365);
Object *vase[] = {
	new Sphere(Vec3(1e5 + 1, 40.8, 81.6), 1e5, DIFF, 1.5, Vec3(.75, .25, .25)),											   // Left
	new Sphere(Vec3(-1e5 + 99, 40.8, 81.6), 1e5, DIFF, 1.5, Vec3(.25, .25, .75)),										   // Right
	new Sphere(Vec3(50, 40.8, 1e5), 1e5, SPEC, 1.5, Vec3(.75, .75, .75), Vec3(), "resource/wallls.com_156455.png"),		   // Back
	new Sphere(Vec3(50, 40.8, -1e5 + 190), 1e5, DIFF, 1.5, Vec3(.25, .25, .25), Vec3(), "resource/wallls.com_156455.png"), // Front
	new Sphere(Vec3(50, 1e5, 81.6), 1e5, DIFF, 1.5, Vec3(.75, .75, .75), Vec3(), "resource/star.png"),					   // Bottom
	new Sphere(Vec3(50, -1e5 + 81.6, 81.6), 1e5, DIFF, 1.5, Vec3(.75, .75, .75)),										   // Top
	new Sphere(Vec3(27, 16.5, 47), 16.5, REFR, 1.5, Vec3(1, 1, 1) * .999), 
	new Cube(Vec3(0, 8, 84), Vec3(34, 10, 116), DIFF, 1.5, Vec3(76 / 255., 34 / 255., 27 / 255.)),
	new BezierObject(Vec3(20, 9.99, 100), vaseCurve, DIFF, 1.5, Vec3(1, 1, 1) * .999, Vec3(), "resource/vase.png"),
	new Sphere(Vec3(73, 16.5, 78), 16.5, DIFF, 1.5, Vec3(1, 1, 1) * .999, Vec3(), "resource/venus.png"), 
	new Sphere(Vec3(50, 681.6 - .27, 81.6), 600, DIFF, 1.5, Vec3(), Vec3(12, 12, 12)) // Lite
};

Object *pink[] = {
	new Sphere(Vec3(1e5 + 1, 40.8, 81.6), 1e5, DIFF, 1.5, Vec3(.99, .25, .20), Vec3(), "resource/pinkwall.png"), // Left
	new Sphere(Vec3(-1e5 + 299, 40.8, 81.6), 1e5, DIFF, 1.5, Vec3(.23, .55, .23)),								 // Right
	new Sphere(Vec3(50, 40.8, 1e5), 1e5, DIFF, 1.5, Vec3(1, 1, 1) * .999, Vec3(), "resource/pink.png"),			 // Back
	new Sphere(Vec3(50, 40.8, -1e5 + 190), 1e5, DIFF, 1.5, Vec3(.25, .25, .25)),								 // Front
	new Sphere(Vec3(50, 1e5, 81.6), 1e5, DIFF, 1.5, Vec3(.75, .25, .25), Vec3(), "resource/star.png"),			 // Bottom
	new Sphere(Vec3(50, -1e5 + 81.6, 81.6), 1e5, DIFF, 1.5, Vec3(.75, .75, .75)),								 // Top
	new Sphere(Vec3(73, 16.5, 40), 16.5, DIFF, 1.7, Vec3(1, 1, 1) * .999, Vec3(), "resource/bricks.jpg"), 
	new Sphere(Vec3(45, 6, 45), 6, REFR, 1.7, Vec3(.5, .5, 1) * .999),									  
	new Sphere(Vec3(52, 3, 75), 3, REFR, 1.7, Vec3(1, .5, .5) * .999),	 
	new Sphere(Vec3(65.5, 3, 88), 3, REFR, 1.7, Vec3(.5, 1, .5) * .999), 
	new Sphere(Vec3(77, 3, 92), 3, REFR, 1.7, Vec3(1, 1, .5) * .999),	 
	new Sphere(Vec3(50, 681.6 - .27, 81.6), 600, DIFF, 1.5, Vec3(), Vec3(1, 1, 1) * 20) // Lite
};

Object **scene = pink; // rabbit,vase,pink
int scene_num = 12; //rabbit:8 vase:11 pink:12

pair<Refl_t, Vec3> get_feature(Object *obj, Texture &texture, Vec3 x, unsigned short *X)
{
	static std::unordered_map<std::string, int> texture_cases = {
		{"resource/star.png", 1},
		{"resource/pinkwall.png", 2},
		{"resource/pink.png", 3},
		{"resource/wallls.com_156455.png", 4},
		{"resource/vase.png", 5},
		{"resource/rainbow.png", 6},
		{"resource/venus.png", 7},
		{"resource/bricks.jpg", 8}};

	pair<Refl_t, Vec3> feature;
	int texture_case = texture_cases[texture.filename];

	switch (texture_case)
	{
	case 1:
		feature = texture.getcol(x.z / 15, x.x / 15);
		break;
	case 2:
		feature = texture.getcol(x.x / 30, x.z / 30);
		break;
	case 3:
		feature = texture.getcol(-x.x / 125, -x.y / 80 - 0.05);
		break;
	case 4:
		feature = texture.getcol(-x.z / 150, -x.y / 100);
		break;
	case 5:
	{
		Vec3 tmp = obj->bezier_trans(x);
		feature = texture.getcol(tmp.x / 2 / PI + .5, tmp.y);
		if (erand48(X) < 0.2)
			feature.first = SPEC;
	}
	break;
	case 6:
	{
		double px = (x.x - 73) / 16.5, py = (x.y - 16.5) / 16.5;
		feature = texture.getcol((py * cos(-0.3) + px * sin(-0.3)) * .6 - .25, x.z);
	}
	break;
	case 7:
	{
		double px = (x.x - 43) / 16.5, py = (x.y - 16.5) / 16.5;
		feature = texture.getcol((py * cos(-0.3) + px * sin(-0.3)) * .6 - .25, x.z);
	}
	break;
	case 8:
		feature = texture.getcol(x.y / 32 + 0.25, x.z);
		break;
	default:
		feature = texture.getcol(x.z, x.x);
		break;
	}
	return feature;
}

#endif // __SCENE_H__
