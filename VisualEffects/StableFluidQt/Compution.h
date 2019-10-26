#pragma once
#include <QVector2D>
namespace Fluid
{
	// no boundry funtion right now
	namespace Compution
	{
		
		typedef std::vector<std::vector<QVector2D>> vectorFiledGrid;
		typedef std::vector<std::vector<float>> scalraFieldGrid;
		void Clamp(float &o_value, float i_min, float i_max)
		{
			if (o_value < i_min)
			{
				o_value = i_min;
			}
			if (o_value > i_max)
			{
				o_value = i_max;
			}

		}

		void Advect(int i_n, int i_m, int i_deltaTime, vectorFiledGrid i_u, vectorFiledGrid i_grid, vectorFiledGrid &o_grid)
		{
			for (int i = 1; i <= i_n; i++)
			{
				for (int j = 1; j <= i_m; j++)
				{
					QVector2D backtracePosition = QVector2D(i, j) - i_deltaTime * u[i][j];
					backtracePosition.x;
					Clamp(backtracePosition.x, 0.5, i_n + 0.5);
					Clamp(backtracePosition.y, 0.5, i_n + 0.5);
					int bi0 = (int)backtracePosition.x; int bj0 = (int)backtracePosition.y;
					int bi1 = bi0 + 1; int bj1 = bj0 + 1;
					float s0 = backtracePosition.x - bi0; float t0 = backtracePosition.y - bj0;
					o_grid[i][j] =
						s0 * (t0 * i_grid[bi0][bj0] + (1 - t0) * i_grid[bi0][bj1]) +
						(1 - s0) * (t0 * i_grid[bi1][bj0] + (1 - t0) * i_grid[bi1][bj1]);
				}
			}
	
		}

		void Diffuse(int i_n, int i_m, float i_alpha, float i_beta, const vectorFiledGrid& i_origin, const vectorFiledGrid& i_grid, vectorFiledGrid& o_grid)
		{
			for (int i = 1; i <= i_n; i++)
			{
				for (int j = 1; j <= i_m; j++)
				{
					o_grid[i][j] = (i_origin[i][j] + i_alpha * (i_grid[i - 1][j] + i_grid[i][j - 1] + i_grid[i + 1][j] + i_grid[i][j + 1])) / i_beta;
				}
			}
		}

		void Diffuse(int i_n, int i_m, float i_alpha, float i_beta, const vectorFiledGrid &i_origin, const scalraFieldGrid &i_grid, scalraFieldGrid&o_grid)
		{
			for (int i = 1; i <= i_n; i++)
			{
				for (int j = 1; j <= i_m; j++)
				{
					o_grid[i][j] = (i_origin[i][j] + i_alpha * (i_grid[i - 1][j] + i_grid[i][j - 1] + i_grid[i + 1][j] + i_grid[i][j + 1])) / i_beta;
				}
			}
		}
		void AddForce(int i_n, int i_m, QVector2D i_forceOrigin, float i_forceExponennt, QVector2D i_forceVector, const vectorFiledGrid& i_grid, vectorFiledGrid& o_grid)
		{
			for (int i = 1; i <= i_n; i++)
			{
				for (int j = 1; j <= i_m; j++)
				{
					float amp = exp(-i_forceExponennt * i_forceOrigin.distanceToPoint(QVector2D(i, j)));
					o_grid[i][j] = i_grid[i][j] + i_forceVector * amp;
				}

			}
		}

		void ProjectStart(int i_n, int i_m, float i_h, const vectorFiledGrid& i_grid, vectorFiledGrid& o_div, scalraFieldGrid& o_p)
		{
			for (int i = 1; i <= i_n; i++) {
				for (int j = 1; j <= i_m; j++) {
					o_div[i][j] = -0.5 * i_h * (i_grid[i + 1][j].x - i_grid[i - 1][j].x + i_grid[i][j + 1].y - i_grid[i][j - 1].y);
					o_p[i][j] = 0;
				}
			}

		}
		
		void ProjectFinish(int i_n, int i_m, float i_h, const vectorFiledGrid& i_v, const scalraFieldGrid& i_p, vectorFiledGrid& o_v)
		{
			for (int i = 1; i <= i_n; i++) {
				for (int j = 1; j <= i_m; j++) {
					o_v[i][j] = i_v[i][j], 0.5 * QVector2D(i_p[i + 1][j] - i_p[i - 1][j], i_p[i][j + 1] - i_p[i][j - 1]) / i_h;
				}
			}
		}

	}
}