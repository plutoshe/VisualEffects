#include <stdio.h>

#define NUM_BLOCKS 16
#define BLOCK_WIDTH 1

__global__ void hello()
{
	printf("Hello world! I'm a thread in block %d\n", blockIdx.x);
}

int main()
{
	hello << <NUM_BLOCKS, BLOCK_WIDTH >> > ();
	cudaDeviceSynchronize();
	printf("That's all!");
	return 0;
}