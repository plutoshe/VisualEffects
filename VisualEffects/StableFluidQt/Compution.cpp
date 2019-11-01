#include "Compution.h"
#include <qdebug.h>
float Fluid::Computation::Clamp(float i_value, float i_min, float i_max)
{
	if (i_value < i_min)
	{
		return i_min;
	}
	if (i_value > i_max)
	{
		return  i_max;
	}
	return i_value;
}
int Fluid::Computation::get1Dpos(int i, int j, int stride)
{
	return i * stride + j;
}

void Fluid::Computation::Advect(int i_n, int i_m, float i_deltaTime, const float* const i_ux, const float* const i_uy, float* o_ux, float* o_uy)
{
	for (int i = 1; i <= i_n; i++)
	{
		for (int j = 1; j <= i_m; j++)
		{
			float bx = i - i_deltaTime * i_ux[get1Dpos(i, j, i_m + 2)] * i_n;
			float by = j - i_deltaTime * i_uy[get1Dpos(i, j, i_m + 2)] * i_m;
			bx = Clamp(bx, 0.5, i_n + 0.5);
			by = Clamp(by, 0.5, i_m + 0.5);
			
			int bi0 = (int)bx; int bj0 = by;
			int bi1 = bi0 + 1; int bj1 = bj0 + 1;
			float s1 = bx - bi0; float t1 = by - bj0;
			float s0 = 1 - s1; float t0 = 1 - t1;
			o_ux[get1Dpos(i, j, i_m + 2)] =
				s0 * (t0 * i_ux[get1Dpos(bi0, bj0, i_m + 2)] + t1 * i_ux[get1Dpos(bi0, bj1, i_m + 2)]) +
				s1 * (t0 * i_ux[get1Dpos(bi1, bj0, i_m + 2)] + t1 * i_ux[get1Dpos(bi1, bj1, i_m + 2)]);
			o_uy[get1Dpos(i, j, i_m + 2)] =
				s0 * (t0 * i_uy[get1Dpos(bi0, bj0, i_m + 2)] + t1 * i_uy[get1Dpos(bi0, bj1, i_m + 2)]) +
				s1 * (t0 * i_uy[get1Dpos(bi1, bj0, i_m + 2)] + t1 * i_uy[get1Dpos(bi1, bj1, i_m + 2)]);
		}
	}

}

void Fluid::Computation::Diffuse(int i_n, int i_m, float i_alpha, float i_beta, const float* const i_origin, const float* const i_grid, float* o_grid)
{
	for (int i = 1; i <= i_n; i++)
	{
		for (int j = 1; j <= i_m; j++)
		{
			o_grid[get1Dpos(i, j, i_m + 2)] = 
				(i_origin[get1Dpos(i, j, i_m + 2)] * i_alpha +
				i_grid[get1Dpos(i - 1, j, i_m + 2)] + 
				i_grid[get1Dpos(i, j - 1, i_m + 2)] + 
				i_grid[get1Dpos(i + 1, j, i_m + 2)] + 
				i_grid[get1Dpos(i, j + 1, i_m + 2)]) / i_beta;
		}
	}
}

void Fluid::Computation::AddForce(int i_n, int i_m, QVector2D i_forceOrigin, float i_forceExponennt, QVector2D i_forceVector, const float* const i_ux, const float* const i_uy, float* o_ux, float* o_uy)
{
	for (int i = 1; i <= i_n; i++)
	{
		for (int j = 1; j <= i_m; j++)
		{
			float sx = i_forceOrigin.x() - i * 1.0 / i_n;
			float sy = i_forceOrigin.y() - j * 1.0 / i_m;
			float amp = exp(-i_forceExponennt * sqrt(sx * sx + sy * sy));

			o_ux[get1Dpos(i, j, i_m + 2)] = i_ux[get1Dpos(i, j, i_m + 2)] + i_forceVector.x() * amp;
			o_uy[get1Dpos(i, j, i_m + 2)] = i_uy[get1Dpos(i, j, i_m + 2)] + i_forceVector.y() * amp;
		}

	}
}

void Fluid::Computation::SetBoundry(int i_n, int i_m, float* o_v, int i_status, bool i_border)
{
	for (int i = 1; i <= i_m; i++)
	{
		o_v[get1Dpos(0, i, i_m + 2)] = ((i_status == 1) ? -1 : 1) * o_v[get1Dpos(1, i, i_m + 2)];
		o_v[get1Dpos(i_n + 1, i, i_m + 2)] = ((i_status == 1) ? -1 : 1) * o_v[get1Dpos(i_n, i, i_m + 2)];
	}
	for (int i = 1; i <= i_n; i++)
	{
		o_v[get1Dpos(i, 0, i_m + 2)] = ((i_status == 2) ? -1 : 1) * o_v[get1Dpos(i, 1, i_m + 2)];
		o_v[get1Dpos(i, i_m + 1, i_m + 2)] = ((i_status == 2) ? -1 : 1) * o_v[get1Dpos(i, i_m, i_m + 2)];
	}
	if (i_border)
	{

		o_v[get1Dpos(0, 0, i_m + 2)] = 0.5f * (o_v[get1Dpos(1, 0, i_m + 2)] + o_v[get1Dpos(0, 1, i_m + 2)]);
		o_v[get1Dpos(0, i_m + 1, i_m + 2)] = 0.5f * (o_v[get1Dpos(1, i_m + 1, i_m + 2)] + o_v[get1Dpos(0, i_m, i_m + 2)]);
		o_v[get1Dpos(i_n + 1, 0, i_m + 2)] = 0.5f * (o_v[get1Dpos(i_n, 0, i_m + 2)] + o_v[get1Dpos(i_n + 1, 1, i_m + 2)]);
		o_v[get1Dpos(i_n + 1, i_m + 1, i_m + 2)] = 0.5f * (o_v[get1Dpos(i_n, i_m + 1, i_m + 2)] + o_v[get1Dpos(i_n + 1, i_m, i_m + 2)]);
	}
}

void Fluid::Computation::ProjectStart(int i_n, int i_m, float i_h, const float* const i_ux, const float* const i_uy, float* o_div, float* o_p)
{
	for (int i = 1; i <= i_n; i++) {
		for (int j = 1; j <= i_m; j++) {
			o_div[get1Dpos(i, j, i_m + 2)] =
				-0.5f * i_h * (
					i_ux[get1Dpos(i + 1, j, i_m + 2)] -
					i_ux[get1Dpos(i - 1, j, i_m + 2)] +
					i_uy[get1Dpos(i, j + 1, i_m + 2)] -
					i_uy[get1Dpos(i, j - 1, i_m + 2)]);
			o_p[get1Dpos(i, j, i_m + 2)] = 0;
		}
	}


}

void Fluid::Computation::ProjectFinish(int i_n, int i_m, float i_h, const float* const i_ux, const float* const i_uy, const float* const i_p, float* o_ux, float* o_uy)
{
	for (int i = 1; i <= i_n; i++) {
		for (int j = 1; j <= i_m; j++) {
			float p1 = i_p[get1Dpos(std::max(1, i - 1), j, i_m + 2)];
			float p2 = i_p[get1Dpos(std::min(i + 1, i_n), j, i_m + 2)];
			float p3 = i_p[get1Dpos(i, std::max(1, j - 1), i_m + 2)];
			float p4 = i_p[get1Dpos(i, std::min(i_m, j + 1), i_m + 2)];
			float ux = i_ux[get1Dpos(i, j, i_m + 2)] - 0.5 * (float)(p2 - p1) / i_h;
			float uy = i_uy[get1Dpos(i, j, i_m + 2)] - 0.5 * (float)(p4 - p3) / i_h;
			o_ux[get1Dpos(i, j, i_m + 2)] = ux;
			o_uy[get1Dpos(i, j, i_m + 2)] = uy;


			/*if (i == 1)
			{
				o_ux[get1Dpos(0, j, i_m + 2)] = -ux;
				o_uy[get1Dpos(0, j, i_m + 2)] = uy;
			}
			if (i == i_n)
			{
				o_ux[get1Dpos(0, j, i_m + 2)] = -ux;
				o_uy[get1Dpos(0, j, i_m + 2)] = uy;
			}
			if (j == 1)
			{
				o_ux[get1Dpos(0, j, i_m + 2)] = ux;
				o_uy[get1Dpos(0, j, i_m + 2)] = -uy;
			}
			if (j == i_m)
			{
				o_ux[get1Dpos(0, j, i_m + 2)] = ux;
				o_uy[get1Dpos(0, j, i_m + 2)] = -uy;
			}*/
		}	
	}

	//o_v[get1Dpos(0, 0, i_m + 2)] = 0.5 * (o_v[get1Dpos(1, 0, i_m + 2)] + o_v[get1Dpos(0, 1, i_m + 2)]);
	//o_v[get1Dpos(0, i_m + 1, i_m + 2)] = 0.5 * (o_v[get1Dpos(1, i_m + 1, i_m + 2)] + o_v[get1Dpos(0, i_m, i_m + 2)]);
	//o_v[get1Dpos(i_n + 1, 0, i_m + 2)] = 0.5 * (o_v[get1Dpos(i_n, 0, i_m + 2)] + o_v[get1Dpos(i_n + 1, 1, i_m + 2)]);
	//o_v[get1Dpos(i_n + 1, i_m + 1, i_m + 2)] = 0.5 * (o_v[get1Dpos(i_n, i_m + 1, i_m + 2)] + o_v[get1Dpos(i_n + 1, i_m, i_m + 2)]);
}
 