#pragma once
#include <QVector2D>
namespace Fluid
{
	// no boundry funtion right now
	namespace Compution
	{
		typedef std::vector<QVector2D> vectorFiledGrid;
		typedef std::vector<float> scalarFieldGrid;

		float Clamp(float i_value, float i_min, float i_max);
		int get1Dpos(int i, int j, int stride);
		void Advect(int i_n, int i_m, int i_deltaTime, vectorFiledGrid i_u, vectorFiledGrid i_grid, vectorFiledGrid& o_grid);

		void Diffuse(int i_n, int i_m, float i_alpha, float i_beta, const vectorFiledGrid& i_origin, const vectorFiledGrid& i_grid, vectorFiledGrid& o_grid);

		void Diffuse(int i_n, int i_m, float i_alpha, float i_beta, const scalarFieldGrid& i_origin, const scalarFieldGrid& i_grid, scalarFieldGrid& o_grid);
		void AddForce(int i_n, int i_m, QVector2D i_forceOrigin, float i_forceExponennt, QVector2D i_forceVector, const vectorFiledGrid& i_grid, vectorFiledGrid& o_grid);

		void ProjectStart(int i_n, int i_m, float i_h, const vectorFiledGrid& i_grid, scalarFieldGrid& o_div, scalarFieldGrid& o_p);

		void ProjectFinish(int i_n, int i_m, float i_h, const vectorFiledGrid& i_v, const scalarFieldGrid& i_p, vectorFiledGrid& o_v);
	}
}

