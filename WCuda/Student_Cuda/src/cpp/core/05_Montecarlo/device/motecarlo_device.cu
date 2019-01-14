#include "ReductionAddTools.h"
#include <curand_kernel.h>
#include <cmath>
#include "Indice1D.h"

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Imported	 	*|
 \*-------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

extern __global__ void montecarlo(curandState* tabDevGeneratorGM, int n, int* ptrResultGM);
extern __global__ void createGenerator(curandState* tabDevGeneratorGM, int deviceId);

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

__global__ void createGenerator(curandState* tabDevGeneratorGM, int deviceId)
    {
    // Customisation du generator:
    // Proposition, au lecteur de faire mieux !
    // Contrainte : Doit etre différent d'un GPU à l'autre
    // Contrainte : Doit etre différent d’un thread à l’autre
    const int TID = Indice1D::tid();
    int deltaSeed = deviceId * INT_MAX / 10000;
    int deltaSequence = deviceId * 100;
    int deltaOffset = deviceId * 100;
    int seed = 1234 + deltaSeed;
    int sequenceNumber = TID + deltaSequence;
    int offset = deltaOffset;
    curand_init(seed, sequenceNumber, offset, &tabDevGeneratorGM[TID]);
    }

__global__ void montecarlo(curandState* tabDevGeneratorGM, int n, int* ptrResultGM)
    {

    extern __shared__ int tabSM[];

    // Reduction intra thread
    const int TID = Indice1D::tid();
    const int TID_LOCAL = Indice1D::tidLocal();

    curandState localGenerator = tabDevGeneratorGM[TID];

    int n0 = 0;

    float xAlea;
    float yAlea;

    for (long i = 1; i <= n; i++)
	{
	xAlea = curand_uniform(&localGenerator) * 2.0 - 1.0;
	yAlea = curand_uniform(&localGenerator) * 2.0;

	if (sqrt(1 - xAlea * xAlea) > yAlea)
	    {
	    n0++;
	    }
	}

    tabDevGeneratorGM[TID] = localGenerator;

    tabSM[TID_LOCAL] = n0;

    __syncthreads();

    // Reduction intra block et interblock
    ReductionAddTools::reductionADD(tabSM, ptrResultGM);

    }

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
