#pragma once
#include <QVector2D>

namespace Fluid
{
	// no boundry funtion right now
	namespace Computation
	{
		float Clamp(float i_value, float i_min, float i_max);
		int get1Dpos(int i, int j, int stride);
		void Advect(int i_n, int i_m, float i_deltaTime, const float* const i_ux, const float* const i_uy, float* o_ux, float* o_uy);
		void Diffuse(int i_n, int i_m, float i_alpha, float i_beta, const float* const i_origin, const float* const i_grid, float* o_grid);
		void AddForce(int i_n, int i_m, QVector2D i_forceOrigin, float i_forceExponennt, QVector2D i_forceVector, const float* const i_ux, const float* const i_uy, float* o_ux, float* o_uy);
		void ProjectStart(int i_n, int i_m, float i_h, const float* const i_ux, const float* const i_uy, float* o_div, float* o_p);
		void ProjectFinish(int i_n, int i_m, float i_h, const float* const i_ux, const float* const i_uy, const float* const i_p, float* o_ux, float* o_uy);
		void SetBoundry(int i_n, int i_m, float* o_v, int i_status, bool i_border);
		
	}

}
