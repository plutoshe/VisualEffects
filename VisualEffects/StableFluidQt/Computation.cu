#include "Compution.h"
#include <cuda.h>
#include <cuda_runtime.h>
#include <qdebug.h>
#include <cmath>
//
__device__ int clamp(float x, float a, float b)
{
	
    return max(a, min(b, x));
}

__device__ int get1Dpos(int i, int j, int delta)
{
    return i * delta + j;
}

__global__
void Advect(int i_n,
	int i_m,
	float i_deltaTime,
    const float* const i_ux,
    const float* const i_uy,
    float* o_ux,
    float* o_uy)
{
    int i = clamp(blockIdx.x * blockDim.x + threadIdx.x + 1, 1, i_n);
    int j = clamp(blockIdx.y * blockDim.y + threadIdx.y + 1, 1, i_m);
	float bx = i - i_deltaTime * i_ux[get1Dpos(i, j, i_m + 2)] * i_n;
	float by = j - i_deltaTime * i_uy[get1Dpos(i, j, i_m + 2)] * i_m;
	bx = clamp(bx, 0.5, i_n + 0.5);
	by = clamp(by, 0.5, i_m + 0.5);

	int bi0 = (int)bx; int bj0 = by;
	int bi1 = bi0 + 1; int bj1 = bj0 + 1;
	float s1 = bx - bi0; float t1 = by - bj0;
	float s0 = 1 - s1; float t0 = 1 - t1;
	o_ux[get1Dpos(i, j, i_m + 2)] =
		s0 * (t0 * i_ux[get1Dpos(bi0, bj0, i_m + 2)] + t1 * i_ux[get1Dpos(bi0, bj1, i_m + 2)]) +
		s1 * (t0 * i_ux[get1Dpos(bi1, bj0, i_m + 2)] + t1 * i_ux[get1Dpos(bi1, bj1, i_m + 2)]);
	o_uy[get1Dpos(i, j, i_m + 2)] =
		s0 * (t0 * i_uy[get1Dpos(bi0, bj0, i_m + 2)] + t1 * i_uy[get1Dpos(bi0, bj1, i_m + 2)]) +
		s1 * (t0 * i_uy[get1Dpos(bi1, bj0, i_m + 2)] + t1 * i_uy[get1Dpos(bi1, bj1, i_m + 2)]);;
}

__global__ void test(int i_n, int i_m, const float* const i_in, float* i_out)
{
	int i = clamp(blockIdx.x * blockDim.x + threadIdx.x + 1, 1, i_n);
	int j = clamp(blockIdx.y * blockDim.y + threadIdx.y + 1, 1, i_m);
    
    i_out[get1Dpos(i, j, i_m + 2)] = i_in[get1Dpos(i, j, i_m + 2)] + 10;
	
}

__global__ void AddForce(int i_n, int i_m, float2 i_forceOrigin, float i_forceExponennt, float2 i_forceVector, const float* const i_ux, const float* const i_uy, float* o_ux, float* o_uy)
{
	int i = clamp(blockIdx.x * blockDim.x + threadIdx.x + 1, 1, i_n);
	int j = clamp(blockIdx.y * blockDim.y + threadIdx.y + 1, 1, i_m);
	float sx =  i_forceOrigin.x - i * 1.0 / i_n;
	float sy = i_forceOrigin.y - j * 1.0 / i_m;
	float amp = exp(-i_forceExponennt*sqrt(sx*sx + sy * sy));

	o_ux[get1Dpos(i, j, i_m + 2)] = i_ux[get1Dpos(i, j, i_m + 2)] + i_forceVector.x * amp;
	o_uy[get1Dpos(i, j, i_m + 2)] = i_uy[get1Dpos(i, j, i_m + 2)] + i_forceVector.y * amp;
}


__global__ void Diffuse(int i_n, int i_m, float i_alpha, float i_beta, const float* const i_origin, const float* const i_grid, float* o_grid)
{
	int i = clamp(blockIdx.x * blockDim.x + threadIdx.x + 1, 1, i_n);
	int j = clamp(blockIdx.y * blockDim.y + threadIdx.y + 1, 1, i_m);
	o_grid[get1Dpos(i, j, i_m + 2)] =
		(i_origin[get1Dpos(i, j, i_m + 2)] * i_alpha +
			i_grid[get1Dpos(i - 1, j, i_m + 2)] +
			i_grid[get1Dpos(i, j - 1, i_m + 2)] +
			i_grid[get1Dpos(i + 1, j, i_m + 2)] +
			i_grid[get1Dpos(i, j + 1, i_m + 2)]) * i_beta;
}



__global__ void ProjectStart(int i_n, int i_m, float i_h, const float* const i_ux, const float* const i_uy, float* o_div, float* o_p)
{
	int i = clamp(blockIdx.x * blockDim.x + threadIdx.x + 1, 1, i_n);
	int j = clamp(blockIdx.y * blockDim.y + threadIdx.y + 1, 1, i_m);
	o_div[get1Dpos(i, j, i_m + 2)] =
		0.5f * i_h * (
			i_ux[get1Dpos(i + 1, j, i_m + 2)] -
			i_ux[get1Dpos(i - 1, j, i_m + 2)] +
			i_uy[get1Dpos(i, j + 1, i_m + 2)] -
			i_uy[get1Dpos(i, j - 1, i_m + 2)]);
	o_p[get1Dpos(i, j, i_m + 2)] = 0;

	/*SetBoundry(i_n, i_m, o_div);
	SetBoundry(i_n, i_m, o_p);*/

}

__global__ void ProjectFinish(int i_n, int i_m, float i_h, const float* const i_ux, const float* const i_uy, const float* const i_p, float* o_ux, float* o_uy)
{
	int i = clamp(blockIdx.x * blockDim.x + threadIdx.x + 1, 1, i_n);
	int j = clamp(blockIdx.y * blockDim.y + threadIdx.y + 1, 1, i_m);
	float p1 = i_p[get1Dpos((i <= 2? 1 : i - 1), j, i_m + 2)];
	float p2 = i_p[get1Dpos((i < i_n - 1) ? i+ 1 : i_n, j, i_m + 2)];
	float p3 = i_p[get1Dpos(i, j <= 2?1: j - 1, i_m + 2)];
	float p4 = i_p[get1Dpos(i, (j < i_m - 1)?j + 1: i_m, i_m + 2)];
	float ux = i_ux[get1Dpos(i, j, i_m + 2)] - 0.5 * (p2 - p1) / i_h;
	float uy = i_uy[get1Dpos(i, j, i_m + 2)] - 0.5 * (p4 - p3) / i_h;
	o_ux[get1Dpos(i, j, i_m + 2)] = ux;
	o_uy[get1Dpos(i, j, i_m + 2)] = uy;
	if (i == 1)
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
	}
	

	//SetBoundry(i_n, i_m, o_v);
	
	//o_v[get1Dpos(0, 0, i_m + 2)] = 0.5 * (o_v[get1Dpos(1, 0, i_m + 2)] + o_v[get1Dpos(0, 1, i_m + 2)]);
	//o_v[get1Dpos(0, i_m + 1, i_m + 2)] = 0.5 * (o_v[get1Dpos(1, i_m + 1, i_m + 2)] + o_v[get1Dpos(0, i_m, i_m + 2)]);
	//o_v[get1Dpos(i_n + 1, 0, i_m + 2)] = 0.5 * (o_v[get1Dpos(i_n, 0, i_m + 2)] + o_v[get1Dpos(i_n + 1, 1, i_m + 2)]);
	//o_v[get1Dpos(i_n + 1, i_m + 1, i_m + 2)] = 0.5 * (o_v[get1Dpos(i_n, i_m + 1, i_m + 2)] + o_v[get1Dpos(i_n + 1, i_m, i_m + 2)]);
}


__global__ void SetBoundry(int i_n, int i_m, float* o_v, int i_status1, int i_status2)
{
	int i = clamp(blockIdx.x * blockDim.x + threadIdx.x + 1, 1, i_n + i_m + 1);
	//int j = blockIdx.y * blockDim.y + threadIdx.y + 1;
	if (i <= i_m)
	{ 
	
		o_v[get1Dpos(0, i, i_m + 2)] = i_status1 * o_v[get1Dpos(1, i, i_m + 2)];
		o_v[get1Dpos(i_n + 1, i, i_m + 2)] = i_status1 * o_v[get1Dpos(i_n, i, i_m + 2)];
	}
	else
	{
		i -= i_m;
		o_v[get1Dpos(i, 0, i_m + 2)] = i_status2 * o_v[get1Dpos(i, 1, i_m + 2)];
		o_v[get1Dpos(i, i_m + 1, i_m + 2)] = i_status2 * o_v[get1Dpos(i, i_m, i_m + 2)];
	}
	
}

void Initialization()
{

}

void CalculateNewVelocity(const int i_n, const int i_m, float* o_ux, float* o_uy, float i_deltaTime, float i_vicosityParam, float i_forceExponentParam, QVector2D i_mousePosition, QVector2D i_forceVector, bool i_gpu)
{
	//m_interVelocity1 = m_velocity;
	//m_interVelocity2 = m_velocity;
	
	if (i_deltaTime < -1e7)
	{
		i_deltaTime = 1e-7;
	}
	double paramB = i_deltaTime * i_vicosityParam;
	if (paramB == 0)
	{
		paramB = 0.00000000001f;
	}
	// initialization
	
	const dim3 blockSize((i_n - 1) / 32 + 1, (i_m - 1) / 32 + 1, 1);
	const dim3 gridSize(32, 32, 1);
	const dim3 borderBlockSize((i_n + i_m) / 1024);
	const dim3 borderGridSize(1024, 1, 1);
	float* ux1 = nullptr;
	float* uy1 = nullptr;
	float* ux2 = nullptr;
	float* uy2 = nullptr;
	float* ux3 = nullptr;
	float* uy3 = nullptr;
	float* p1 = nullptr;
	float* p2 = nullptr;
	float* div = nullptr;
	int totalNum = (i_n + 2) * (i_m + 2);
	int totalSize = sizeof(float) * (i_n + 2) * (i_m + 2);
	if (i_gpu)
	{
		cudaMalloc(&ux1, totalSize);
		cudaMalloc(&ux2, totalSize);
		cudaMalloc(&uy1, totalSize);
		cudaMalloc(&uy2, totalSize);
		cudaMalloc(&ux3, totalSize);
		cudaMalloc(&uy3, totalSize);
		cudaMalloc(&p1, totalSize);
		cudaMalloc(&p2, totalSize);
		cudaMalloc(&div, totalSize);


		cudaMemcpy(ux1, o_ux, totalSize, cudaMemcpyHostToDevice);
		cudaMemcpy(uy1, o_uy, totalSize, cudaMemcpyHostToDevice);
	}
	else
	{
		ux1 = new float[totalNum];
		uy1 = new float[totalNum];
		ux2 = new float[totalNum];
		uy2 = new float[totalNum];
		ux3 = new float[totalNum];
		uy3 = new float[totalNum];
		p1 = new float[totalNum];
		p2 = new float[totalNum];
		div = new float[totalNum];
		cudaMemset(div, 0, totalSize * sizeof(float));
		cudaMemcpy(ux1, o_ux, totalSize, cudaMemcpyHostToHost);
		cudaMemcpy(uy1, o_uy, totalSize, cudaMemcpyHostToHost);

	}

	double dx = 1.0 / i_n;
	double dif_alpha = (dx * dx) / (paramB);
	if (i_gpu)
	{ 
		Advect << < gridSize, blockSize >> > (i_n, i_m, i_deltaTime, ux1, uy1, ux2, uy2);
		cudaMemcpy(ux1, ux2, totalSize, cudaMemcpyDeviceToDevice);
		cudaMemcpy(uy1, uy2, totalSize, cudaMemcpyDeviceToDevice);
	}
	else
	{
		Fluid::Computation::Advect(i_n, i_m, i_deltaTime, ux1, uy1, ux2, uy2);
		cudaMemcpy(ux1, ux2, totalSize, cudaMemcpyHostToHost);
		cudaMemcpy(uy1, uy2, totalSize, cudaMemcpyHostToHost);

	}
	

	for (int i = 0; i < 20; i++)
	{
		if (i_gpu)
		{
			Diffuse << <gridSize, blockSize >> > (i_n, i_m, dif_alpha, 1 / (4 + dif_alpha), ux1, ux2, ux3);
			Diffuse << <gridSize, blockSize >> > (i_n, i_m, dif_alpha, 1 / (4 + dif_alpha), uy1, uy2, uy3);
			SetBoundry << <borderGridSize, borderBlockSize >> > (i_n, i_m, ux3, -1, 1);
			SetBoundry << <borderGridSize, borderBlockSize >> > (i_n, i_m, uy3, 1, -1);
			Diffuse << <gridSize, blockSize >> > (i_n, i_m, dif_alpha, 1 / (4 + dif_alpha), ux1, ux3, ux2);
			Diffuse << <gridSize, blockSize >> > (i_n, i_m, dif_alpha, 1 / (4 + dif_alpha) , uy1, uy3, uy2);
			SetBoundry << <borderGridSize, borderBlockSize >> > (i_n, i_m, ux2, -1, 1);
			SetBoundry << <borderGridSize, borderBlockSize >> > (i_n, i_m, uy2, 1, -1);
		}
		else
		{
			Fluid::Computation::Diffuse(i_n, i_m, dif_alpha, 1.0 / (4.0 + dif_alpha), ux1, ux2, ux3);
			Fluid::Computation::Diffuse(i_n, i_m, dif_alpha, 1.0 / (4.0 + dif_alpha), uy1, uy2, uy3);
			Fluid::Computation::SetBoundry(i_n, i_m, ux3, 1, false);
			Fluid::Computation::SetBoundry(i_n, i_m, uy3, 2, false);
			Fluid::Computation::Diffuse(i_n, i_m, dif_alpha, 1.0 / (4.0 + dif_alpha), ux1, ux3, ux2);
			Fluid::Computation::Diffuse(i_n, i_m, dif_alpha, 1.0 / (4.0 + dif_alpha), uy1, uy3, uy2);
			Fluid::Computation::SetBoundry(i_n, i_m, ux2, 1, false);
			Fluid::Computation::SetBoundry(i_n, i_m, uy2, 2, false);
			//Fluid::Computation::SetBoundry(m_height, m_width, m_interVelocity1);
		}
	}
	
	if (i_gpu)
	{
		AddForce << <gridSize, blockSize >> > (i_n, i_m, make_float2(i_mousePosition.x(), i_mousePosition.y()), i_forceExponentParam, make_float2(i_forceVector.x(), i_forceVector.y()), ux2, uy2, ux3, uy3);
		ProjectStart << <gridSize, blockSize >> > (i_n, i_m, 1.0 / i_n, ux3, uy3, div, p1);
		SetBoundry << <borderGridSize, borderBlockSize >> > (i_n, i_m, div, 1, 1);
		SetBoundry << <borderGridSize, borderBlockSize >> > (i_n, i_m, p1, 1, 1);
	}
	else
	{
		Fluid::Computation::AddForce(i_n, i_m, i_mousePosition, i_forceExponentParam, i_forceVector, ux2, uy2, ux3, uy3);
		Fluid::Computation::ProjectStart(i_n, i_m, 0.5 * dx, ux3, uy3, div, p1);
		Fluid::Computation::SetBoundry(i_n, i_m, div, 0, true);
		Fluid::Computation::SetBoundry(i_n, i_m, p1, 0, true);
	}
	
	//	
	

	for (int i = 0; i < 20; i++)
	{
		if (i_gpu)
		{
		
			Diffuse << <gridSize, blockSize >> > (i_n, i_m, 1.0, 1.0/4, div, p1, p2);
			
			SetBoundry << <borderGridSize, borderBlockSize >> > (i_n, i_m, p2, 1, 1);
			Diffuse << <gridSize, blockSize >> > (i_n, i_m, 1.0, 1.0 / 4, div, p2, p1);
			SetBoundry << <borderGridSize, borderBlockSize >> > (i_n, i_m, p1, 1, 1);
		}
		else
		{
			Fluid::Computation::Diffuse(i_n, i_m, -1, 1.0 / 4, div, p1, p2);
			Fluid::Computation::SetBoundry(i_n, i_m, p2, 0, true);
			//Fluid::Compution::SetBoundry(m_height, m_width, m_P2);
			Fluid::Computation::Diffuse(i_n, i_m, -1, 1.0 / 4, div, p2, p1);
			Fluid::Computation::SetBoundry(i_n, i_m, p1, 0, true);
			//Fluid::Compution::SetBoundry(m_height, m_width, m_P1);
			qDebug() << p1[0] << p2[0];
		}
	}
	if (i_gpu)
	{
		ProjectFinish << <gridSize, blockSize >> > (i_n, i_m, dx, ux3, uy3, p1, ux1, uy1);
		cudaMemcpy(o_ux, ux1, totalSize, cudaMemcpyDeviceToHost);
		cudaMemcpy(o_uy, uy1, totalSize, cudaMemcpyDeviceToHost);
		Fluid::Computation::SetBoundry(i_n, i_m, o_ux, 1, true);
		Fluid::Computation::SetBoundry(i_n, i_m, o_uy, 2, true);
	}
	else
	{
		Fluid::Computation::ProjectFinish(i_n, i_m, 0.5 * dx, ux3, uy3, p1, ux1, uy1);
		cudaMemcpy(o_ux, ux1, totalSize, cudaMemcpyHostToHost);
		cudaMemcpy(o_uy, uy1, totalSize, cudaMemcpyHostToHost);
		Fluid::Computation::SetBoundry(i_n, i_m, o_ux, 1, true);
		Fluid::Computation::SetBoundry(i_n, i_m, o_uy, 2, true);
	}
}

void test1(int i_n, int i_m)
{
	int m_height = 100;
	int m_width = 100;
	
	float* result = new float[sizeof(float) * (m_height + 2) * (m_width + 2)];
	const dim3 blockSize((m_height - 1) / 32 + 1, (m_width - 1) / 32 + 1, 1);
	const dim3 gridSize(32, 32, 1);
	float* d_a = nullptr;
	float* d_b = nullptr;
	
	auto a = cudaMalloc(&d_a, sizeof(float) * (m_height + 2) * (m_width + 2));
	auto b = cudaMalloc(&d_b, sizeof(float) * (m_height + 2) * (m_width + 2));
	cudaMemset(d_a, 0, sizeof(float) * (m_height + 2) * (m_width + 2));
	cudaMemset(d_b, 0, sizeof(float) * (m_height + 2) * (m_width + 2));
	cudaMemcpy(result, d_a, sizeof(float) * (m_height + 2) * (m_width + 2), cudaMemcpyDeviceToHost);
	//cudaMemcpy(d_b, result, sizeof(float) * (m_height + 2) * (m_width + 2), cudaMemcpyHostToDevice);
	
	test<<<gridSize, blockSize>>>(m_height, m_width, d_a, d_b);
	cudaMemcpy(result, d_b, sizeof(float) * (m_height + 2) * (m_width + 2), cudaMemcpyDeviceToHost);
	
	cudaDeviceSynchronize();
	for (int i = 1; i <= 100; i++ )
		for (int j = 1; j <= 100; j++)
			if (result[i * 102 + 100] != 10)
			{
			
			}
}
