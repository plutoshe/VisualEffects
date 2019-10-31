#define NUM_BLOCKS 16
#define BLOCK_WIDTH 1
#include <stdio.h>

__global__ void hello()
{
	printf("Hello world! I'm a thread in block %d\n", blockIdx.x);
}
void hello1()
{
	hello << <NUM_BLOCKS, BLOCK_WIDTH >> > ();
	cudaDeviceSynchronize();
}
