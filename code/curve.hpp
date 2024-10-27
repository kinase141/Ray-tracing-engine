#ifndef __CURVE_H__
#define __CURVE_H__

#include "utils.hpp"
#include "ray.hpp"
struct Data
{
	double t0, t1, width, y0, y1;
};

class CurveBezier
{
public:
	int n;
	double *dx, *dy, max, height, r, num;
	Data data[20];
	CurveBezier(double *px, double *py, int n_, int num_, double r_) : num(num_), n(n_), r(r_)
	{
		dx = new double[n];
		dy = new double[n];
		--n;
		calculateDifferences(px, py);
		calculateCoefficients();
		calculateMaxWidth();
		calculateMaxHeight();
	}

	void calculateDifferences(double *px, double *py)
	{
		for (int i = 0; i <= n; ++i)
		{
			dx[i] = px[0];
			dy[i] = py[0];
			for (int j = 0; j <= n - i; ++j)
			{
				px[j] = px[j + 1] - px[j];
				py[j] = py[j + 1] - py[j];
			}
		}
	}

	void calculateCoefficients()
	{
		double n_down = 1, fac = 1, nxt = n;
		for (int i = 0; i <= n; ++i, --nxt)
		{
			fac = fac * (i == 0 ? 1 : i);
			dx[i] = dx[i] * n_down / fac;
			dy[i] = dy[i] * n_down / fac;
			n_down *= nxt;
		}
	}

	void calculateMaxWidth()
	{
		max = 0;
		double interval = 1. / (num - 1), c = 0;
		for (int cnt = 0; cnt <= num; c += interval, ++cnt)
		{
			data[cnt].width = 0;
			data[cnt].t0 = std::max(0., c - r);
			data[cnt].t1 = std::min(1., c + r);
			data[cnt].y0 = getpos(data[cnt].t0).y;
			data[cnt].y1 = getpos(data[cnt].t1).y;
			for (double t = data[cnt].t0; t <= data[cnt].t1; t += 0.00001)
			{
				Vec3 pos = getpos(t);
				if (data[cnt].width < pos.x)
					data[cnt].width = pos.x;
			}
			if (max < data[cnt].width)
				max = data[cnt].width;
			data[cnt].width += eps;
		}
		max += eps;
	}

	void calculateMaxHeight()
	{
		height = getpos(1).y;
	}
	Vec3 getpos(double t)
	{
		double ans_x = 0, ans_y = 0, t_pow = 1;
		for (int i = 0; i <= n; ++i)
		{
			ans_x += dx[i] * t_pow;
			ans_y += dy[i] * t_pow;
			t_pow *= t;
		}
		return Vec3(ans_x, ans_y);
	}
	Vec3 getdir(double t)
	{
		double ans_x = 0, ans_y = 0, t_pow = 1;
		for (int i = 1; i <= n; ++i)
		{
			ans_x += dx[i] * i * t_pow;
			ans_y += dy[i] * i * t_pow;
			t_pow *= t;
		}
		return Vec3(ans_x, ans_y);
	}
};

#endif