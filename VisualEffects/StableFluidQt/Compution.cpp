#include "Compution.h"
#include <qdebug.h>
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

void Fluid::Compution::Advect(int i_n, int i_m, float i_deltaTime, const vectorFiledGrid& i_u, vectorFiledGrid& o_u)
{
	for (int i = 1; i <= i_n; i++)
	{
		for (int j = 1; j <= i_m; j++)
		{
			QVector2D backtracePosition = QVector2D(i - i_deltaTime * i_u[get1Dpos(i, j, i_m + 2)].x() * i_n, j - i_deltaTime * i_u[get1Dpos(i, j, i_m + 2)].y() * i_m);

			backtracePosition.setX(Clamp(backtracePosition.x(), 0.5, i_n + 0.5));
			backtracePosition.setY(Clamp(backtracePosition.y(), 0.5, i_m + 0.5));
			int bi0 = (int)backtracePosition.x(); int bj0 = (int)backtracePosition.y();
			int bi1 = bi0 + 1; int bj1 = bj0 + 1;
			float s1 = backtracePosition.x() - bi0; float t1 = backtracePosition.y() - bj0;
			float s0 = 1 - s1; float t0 = 1 - t1;
			o_u[get1Dpos(i, j, i_m + 2)] =
				s0 * (t0 * i_u[get1Dpos(bi0, bj0, i_m + 2)] + t1 * i_u[get1Dpos(bi0, bj1, i_m + 2)]) +
				s1 * (t0 * i_u[get1Dpos(bi1, bj0, i_m + 2)] + t1 * i_u[get1Dpos(bi1, bj1, i_m + 2)]);
		}
	}

}

void Fluid::Compution::Diffuse(int i_n, int i_m, float i_alpha, float i_beta, const vectorFiledGrid& i_origin, const vectorFiledGrid& i_grid, vectorFiledGrid& o_grid)
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

void Fluid::Compution::Diffuse(int i_n, int i_m, float i_alpha, float i_beta, const scalarFieldGrid& i_origin, const scalarFieldGrid& i_grid, scalarFieldGrid& o_grid)
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
void Fluid::Compution::AddForce(int i_n, int i_m, QVector2D i_forceOrigin, float i_forceExponennt, QVector2D i_forceVector, const vectorFiledGrid& i_grid, vectorFiledGrid& o_grid)
{
	for (int i = 1; i <= i_n; i++)
	{
		for (int j = 1; j <= i_m; j++)
		{
			float amp = exp(-i_forceExponennt * i_forceOrigin.distanceToPoint(QVector2D(i * 1.0 / i_n, j * 1.0 / i_m)));
			
			o_grid[get1Dpos(i, j, i_m + 2)] = i_grid[get1Dpos(i, j, i_m + 2)] + i_forceVector * amp;
		}

	}
}

void Fluid::Compution::SetBoundry(int i_n, int i_m, vectorFiledGrid& o_v)
{
	for (int i = 1; i <= i_m; i++)
	{
		int l = get1Dpos(1, i, i_m + 2);
		QVector2D data = o_v[get1Dpos(1, i, i_m + 2)];
		o_v[get1Dpos(0, i, i_m + 2)] = QVector2D(-data.x(), data.y());
		data = o_v[get1Dpos(i_n + 1, i, i_m + 2)];
		o_v[get1Dpos(i_n + 1, i, i_m + 2)] = QVector2D(-data.x(), data.y());
	}
	for (int i = 1; i <= i_n; i++)
	{
		QVector2D data = o_v[get1Dpos(i, 1, i_m + 2)];
		o_v[get1Dpos(i, 0, i_m + 2)] = QVector2D(data.x(), -data.y());
		data = o_v[get1Dpos(i, i_m, i_m + 2)];
		o_v[get1Dpos(i, i_m + 1, i_m + 2)] = QVector2D(data.x(), -data.y());
	}
	o_v[get1Dpos(0, 0, i_m + 2)] = 0.5 * (o_v[get1Dpos(1, 0, i_m + 2)] + o_v[get1Dpos(0, 1, i_m + 2)]);
	o_v[get1Dpos(0, i_m + 1, i_m + 2)] = 0.5 * (o_v[get1Dpos(1, i_m + 1, i_m + 2)] + o_v[get1Dpos(0, i_m, i_m + 2)]);
	o_v[get1Dpos(i_n + 1, 0, i_m + 2)] = 0.5 * (o_v[get1Dpos(i_n, 0, i_m + 2)] + o_v[get1Dpos(i_n + 1, 1, i_m + 2)]);
	o_v[get1Dpos(i_n + 1, i_m + 1, i_m + 2)] = 0.5 * (o_v[get1Dpos(i_n, i_m + 1, i_m + 2)] + o_v[get1Dpos(i_n + 1, i_m, i_m + 2)]);
}

void Fluid::Compution::SetBoundry(int i_n, int i_m, scalarFieldGrid& o_v)
{
	for (int i = 1; i <= i_m; i++)
	{
		o_v[get1Dpos(0, i, i_m + 2)] = o_v[get1Dpos(1, i, i_m + 2)];
		o_v[get1Dpos(i_n + 1, i, i_m + 2)] = o_v[get1Dpos(i_n, i, i_m + 2)];
	}
	for (int i = 1; i <= i_n; i++)
	{
		o_v[get1Dpos(i, 0, i_m + 2)] = o_v[get1Dpos(i, 1, i_m + 2)];
		o_v[get1Dpos(i, i_m + 1, i_m + 2)] = o_v[get1Dpos(i, i_m, i_m + 2)];
	}
	o_v[get1Dpos(0, 0, i_m + 2)] = 0.5f * (o_v[get1Dpos(1, 0, i_m + 2)] + o_v[get1Dpos(0, 1, i_m + 2)]);
	o_v[get1Dpos(0, i_m + 1, i_m + 2)] = 0.5f * (o_v[get1Dpos(1, i_m + 1, i_m + 2)] + o_v[get1Dpos(0, i_m, i_m + 2)]);
	o_v[get1Dpos(i_n + 1, 0, i_m + 2)] = 0.5f * (o_v[get1Dpos(i_n, 0, i_m + 2)] + o_v[get1Dpos(i_n + 1, 1, i_m + 2)]);
	o_v[get1Dpos(i_n + 1, i_m + 1, i_m + 2)] = 0.5f * (o_v[get1Dpos(i_n, i_m + 1, i_m + 2)] + o_v[get1Dpos(i_n + 1, i_m, i_m + 2)]);
}

void Fluid::Compution::ProjectStart(int i_n, int i_m, float i_h, const vectorFiledGrid& i_grid, scalarFieldGrid& o_div, scalarFieldGrid& o_p)
{
	for (int i = 1; i <= i_n; i++) {
		for (int j = 1; j <= i_m; j++) {
			/*(W_in[tid + int2(1, 0)].x - W_in[tid - int2(1, 0)].x +
				W_in[tid + int2(0, 1)].y - W_in[tid - int2(0, 1)].y)* dim.y / 2;*/
			o_div[get1Dpos(i, j, i_m + 2)] =
				-0.5f * i_h * (
				(float)(i_grid[get1Dpos(i + 1, j, i_m + 2)].x()) -
					(float)(i_grid[get1Dpos(i - 1, j, i_m + 2)].x()) +
					(float)(i_grid[get1Dpos(i, j + 1, i_m + 2)].y()) -
					(float)i_grid[get1Dpos(i, j - 1, i_m + 2)].y());
			o_p[get1Dpos(i, j, i_m + 2)] = 0;
		}
	}
	SetBoundry(i_n, i_m, o_div);
	SetBoundry(i_n, i_m, o_p);

}

void Fluid::Compution::ProjectFinish(int i_n, int i_m, float i_h, const vectorFiledGrid& i_v, const scalarFieldGrid& i_p, vectorFiledGrid& o_v)
{
	for (int i = 1; i <= i_n; i++) {
		for (int j = 1; j <= i_m; j++) {
			float p1 = i_p[get1Dpos(std::max(1, i - 1), j, i_m + 2)];
			float p2 = i_p[get1Dpos(std::min(i + 1, i_n), j, i_m + 2)];
			float p3 = i_p[get1Dpos(i, std::max(1, j - 1), i_m + 2)];
			float p4 = i_p[get1Dpos(i, std::min(i_m, j + 1), i_m + 2)];
			
			QVector2D u = i_v[get1Dpos(i, j, i_m + 2)] -
				0.5 * QVector2D(
					p2 - p1,
					p4 - p3) / i_h ;
			o_v[get1Dpos(i, j, i_m + 2)] = u;
			//if (i == 1) o_v[get1Dpos(0, j, i_m + 2)] = QVector2D(-u.x(), u.y());
			//if (j == 1) o_v[get1Dpos(i, 0, i_m + 2)] = QVector2D(u.x(), -u.y());
			//if (i == i_n) o_v[get1Dpos(i_n + 1, j, i_m + 2)] = QVector2D(-u.x(), u.y());
			//if (j == i_m) o_v[get1Dpos(i, i_m + 1, i_m + 2)] = QVector2D(u.x(), -u.y());
			
		}

	
	}
	SetBoundry(i_n, i_m, o_v);
	//o_v[get1Dpos(0, 0, i_m + 2)] = 0.5 * (o_v[get1Dpos(1, 0, i_m + 2)] + o_v[get1Dpos(0, 1, i_m + 2)]);
	//o_v[get1Dpos(0, i_m + 1, i_m + 2)] = 0.5 * (o_v[get1Dpos(1, i_m + 1, i_m + 2)] + o_v[get1Dpos(0, i_m, i_m + 2)]);
	//o_v[get1Dpos(i_n + 1, 0, i_m + 2)] = 0.5 * (o_v[get1Dpos(i_n, 0, i_m + 2)] + o_v[get1Dpos(i_n + 1, 1, i_m + 2)]);
	//o_v[get1Dpos(i_n + 1, i_m + 1, i_m + 2)] = 0.5 * (o_v[get1Dpos(i_n, i_m + 1, i_m + 2)] + o_v[get1Dpos(i_n + 1, i_m, i_m + 2)]);
}
 