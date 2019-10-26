#include "Compution.h"

float Fluid::Compution::Clamp(float i_value, float i_min, float i_max)
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
int Fluid::Compution::get1Dpos(int i, int j, int stride)
{
	return i * stride + j;
}

void Fluid::Compution::Advect(int i_n, int i_m, int i_deltaTime, vectorFiledGrid i_u, vectorFiledGrid i_grid, vectorFiledGrid& o_grid)
{
	for (int i = 1; i <= i_n; i++)
	{
		for (int j = 1; j <= i_m; j++)
		{
			QVector2D backtracePosition = QVector2D(i, j) - i_deltaTime * i_u[get1Dpos(i, j, i_m + 2)];
			backtracePosition.setX(Clamp(backtracePosition.x(), 0.5, i_n + 0.5));
			backtracePosition.setY(Clamp(backtracePosition.y(), 0.5, i_n + 0.5));
			int bi0 = (int)backtracePosition.x(); int bj0 = (int)backtracePosition.y();
			int bi1 = bi0 + 1; int bj1 = bj0 + 1;
			float s0 = backtracePosition.x() - bi0; float t0 = backtracePosition.y() - bj0;
			o_grid[get1Dpos(i, j, i_m + 2)] =
				s0 * (t0 * i_grid[get1Dpos(bi0, bj0, i_m + 2)] + (1 - t0) * i_grid[get1Dpos(bi0, bj1, i_m + 2)]) +
				(1 - s0) * (t0 * i_grid[get1Dpos(bi1, bj0, i_m + 2)] + (1 - t0) * i_grid[get1Dpos(bi1, bj1, i_m + 2)]);
		}
	}

}

void Fluid::Compution::Diffuse(int i_n, int i_m, float i_alpha, float i_beta, const vectorFiledGrid& i_origin, const vectorFiledGrid& i_grid, vectorFiledGrid& o_grid)
{
	for (int i = 1; i <= i_n; i++)
	{
		for (int j = 1; j <= i_m; j++)
		{
			o_grid[get1Dpos(i, j, i_m + 2)] = (i_origin[get1Dpos(i, j, i_m + 2)] + i_alpha * (i_grid[get1Dpos(i - 1, j, i_m + 2)] + i_grid[get1Dpos(i, j - 1, i_m + 2)] + i_grid[get1Dpos(i + 1, j, i_m + 2)] + i_grid[get1Dpos(i, j + 1, i_m + 2)])) / i_beta;
		}
	}
}

void Fluid::Compution::Diffuse(int i_n, int i_m, float i_alpha, float i_beta, const scalarFieldGrid& i_origin, const scalarFieldGrid& i_grid, scalarFieldGrid& o_grid)
{
	for (int i = 1; i <= i_n; i++)
	{
		for (int j = 1; j <= i_m; j++)
		{
			o_grid[get1Dpos(i, j, i_m + 2)] = (i_origin[get1Dpos(i, j, i_m + 2)] + i_alpha * (i_grid[get1Dpos(i - 1, j, i_m + 2)] + i_grid[get1Dpos(i, j - 1, i_m + 2)] + i_grid[get1Dpos(i + 1, j, i_m + 2)] + i_grid[get1Dpos(i, j + 1, i_m + 2)])) / i_beta;
		}
	}
}
void Fluid::Compution::AddForce(int i_n, int i_m, QVector2D i_forceOrigin, float i_forceExponennt, QVector2D i_forceVector, const vectorFiledGrid& i_grid, vectorFiledGrid& o_grid)
{
	for (int i = 1; i <= i_n; i++)
	{
		for (int j = 1; j <= i_m; j++)
		{
			float amp = exp(-i_forceExponennt * i_forceOrigin.distanceToPoint(QVector2D(i, j)));
			o_grid[get1Dpos(i, j, i_m + 2)] = i_grid[get1Dpos(i, j, i_m + 2)] + i_forceVector * amp;
		}

	}
}

void Fluid::Compution::ProjectStart(int i_n, int i_m, float i_h, const vectorFiledGrid& i_grid, scalarFieldGrid& o_div, scalarFieldGrid& o_p)
{
	for (int i = 1; i <= i_n; i++) {
		for (int j = 1; j <= i_m; j++) {
			o_div[get1Dpos(i, j, i_m + 2)] =
				-0.5 * i_h * (
					i_grid[get1Dpos(i + 1, j, i_m + 2)].x() -
					i_grid[get1Dpos(i - 1, j, i_m + 2)].x() +
					i_grid[get1Dpos(i, j + 1, i_m + 2)].y() -
					i_grid[get1Dpos(i, j - 1, i_m + 2)].y());
			o_p[get1Dpos(i, j, i_m + 2)] = 0;
		}
	}

}

void Fluid::Compution::ProjectFinish(int i_n, int i_m, float i_h, const vectorFiledGrid& i_v, const scalarFieldGrid& i_p, vectorFiledGrid& o_v)
{
	for (int i = 1; i <= i_n; i++) {
		for (int j = 1; j <= i_m; j++) {
			o_v[get1Dpos(i, j, i_m + 2)] = i_v[get1Dpos(i, j, i_m + 2)], 0.5 * QVector2D(i_p[get1Dpos(i + 1, j, i_m + 2)] - i_p[get1Dpos(i - 1, j, i_m + 2)], i_p[get1Dpos(i, j + 1, i_m + 2)] - i_p[get1Dpos(i, j - 1, i_m + 2)]) / i_h;
		}
	}
}
 