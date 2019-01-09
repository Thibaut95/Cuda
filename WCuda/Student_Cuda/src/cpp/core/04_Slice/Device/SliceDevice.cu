#include "Indice2D.h"
#include "ReductionAddTools.h"

__global__ void sliceDevice(int n, float* ptrResultGM);
__device__ void reductionIntraThreadSlice(float* tabSM, int n);
__device__ float f(float x);

__global__ void sliceDevice(int n, float* ptrResultGM)
    {
    __shared__ extern float tabSM[];

    reductionIntraThreadSlice(tabSM, n);

    __syncthreads();

    ReductionAddTools::reductionADD(tabSM, ptrResultGM);

    }

__device__ void reductionIntraThreadSlice(float* tabSM, int n)
    {
    const int TID = Indice2D::tid();
    int s = TID;
    const int NB_THREAD = Indice2D::nbThread();
    const int TID_LOCAL = Indice2D::tidLocal();

    float sum = 0.0;
    float xi;

    while (s < n)
	{
	xi = s / (float) n;
	sum += (1 / (float) n) * f(xi);
	s += NB_THREAD;
	}
    tabSM[TID_LOCAL] = sum;
    }

__device__ float f(float x)
    {
    // TODO SliceNaif
    return 4 / (float) (1 + x * x);
    }
