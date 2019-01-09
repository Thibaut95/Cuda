#include <assert.h>

#include "Indice2D.h"
#include "cudaTools.h"
#include "Device.h"

#include "RaytracingMath.h"
#include "Sphere.h"
#include "nbSphere.h"

#include "IndiceTools_GPU.h"
using namespace gpu;

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

__constant__ Sphere TAB_SPHERES_CM[NB_SPHERE]; // NB_SPHERE define in nbSphere.h

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

__global__ void kernelRaytacingGM(uchar4* ptrDevPixels, uint w, uint h, float t, Sphere* ptrTabSpheresGM, int nbSpheres);
__global__ void kernelRaytacingSM(uchar4* ptrDevPixels, uint w, uint h, float t, Sphere* ptrTabSpheresGM, int nbSpheres);
__global__ void kernelRaytacingCM(uchar4* ptrDevPixels, uint w, uint h, float t, int nbSpheres);

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

static __device__ void work(uchar4* ptrDevPixels, uint w, uint h, float t, Sphere* ptrDevTabSpheres, int nbSpheres);
static __device__ void copyGMtoSM(Sphere* tabGM, Sphere* tabSM, int nbSpheres);

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		host			*|
 \*-------------------------------------*/

/**
 * must be called by host
 * ptrTabSpheres est un tableau de sphere cote host
 */
__host__ void uploadToCM(Sphere* ptrTabSpheres, int nbSpheres)
    {
    assert(nbSpheres == NB_SPHERE);

    // TODO Raytracing GPU CM
    // mettre ptrTabSpheres dans TAB_SPHERES_CM (line 16)
    size_t size = NB_SPHERE * sizeof(Sphere);
    int offset = 0;
    HANDLE_ERROR(cudaMemcpyToSymbol(TAB_SPHERES_CM, ptrTabSpheres, size, offset, cudaMemcpyHostToDevice));
    }

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

__global__ void kernelRaytacingGM(uchar4* ptrDevPixels, uint w, uint h, float t, Sphere* ptrTabSpheresGM, int nbSpheres)
    {
    // TODO Raytracing GPU GM
    // call work with good input
    work(ptrDevPixels, w, h, t, ptrTabSpheresGM, nbSpheres);
    }

__global__ void kernelRaytacingSM(uchar4* ptrDevPixels, uint w, uint h, float t, Sphere* ptrTabSpheresGM, int nbSpheres)
    {
    // TODO Raytracing GPU SM
    // call work with good input
    extern  __shared__  Sphere tabSM[];
    copyGMtoSM(ptrTabSpheresGM, tabSM, nbSpheres);
    __syncthreads();
    work(ptrDevPixels, w, h, t, tabSM, nbSpheres);
    }

__global__ void kernelRaytacingCM(uchar4* ptrDevPixels, uint w, uint h, float t, int nbSpheres)
    {
    // TODO Raytracing GPU CM
    // call work with good input
    work(ptrDevPixels, w, h, t, TAB_SPHERES_CM, nbSpheres);
    }

/*--------------------------------------*\
 |*		private			*|
 \*-------------------------------------*/

/**
 * Methode commune au 3 kernel ci-dessus.
 * Ici on ne sait pas si derriere ptrDevTabSpheres, c'est
 * 	- de la GM?
 * 	- de la SM?
 * 	- de la CM?
 * Pas d'importance, c'est un pointeur et on travail avec!
 */
__device__ void work(uchar4* ptrDevPixels, uint w, uint h, float t, Sphere* ptrDevTabSpheres, int nbSpheres)
    {
    // TODO Raytracing GPU device side
    // create RaytracingMath
    // entrelacement
    RaytracingMath rayTracingMath = RaytracingMath(ptrDevTabSpheres, nbSpheres);

    const int TID = Indice2D::tid();
    const int NB_THREAD = Indice2D::nbThread();
    const int WH = w * h;

    int i;
    int j;

    int s = TID;
    // TODO Rippling GPU  pattern entrelacement
    while (s < WH)
	{
	IndiceTools::toIJ(s, w, &i, &j);

	rayTracingMath.color(&ptrDevPixels[s], i, j, t);

	s += NB_THREAD;
	}
    }

__device__ void copyGMtoSM(Sphere* tabGM, Sphere* tabSM, int nbSpheres)
    {
	const int TID = Indice2D::tidLocal();
        const int NB_THREAD_LOCAL = Indice2D::nbThreadLocal();

        int s = TID;

        while(s<nbSpheres)
            {
            tabSM[s]=tabGM[s];
            s += NB_THREAD_LOCAL;
            }
    }

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

