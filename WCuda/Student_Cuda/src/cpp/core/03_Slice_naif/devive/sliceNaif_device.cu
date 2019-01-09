#include "Indice2D.h"
#include "Indice1D.h"
#include "cudaTools.h"

#include <stdio.h>

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Imported	 	*|
 \*-------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

__global__ void reductionIntraThread(float* tabGM, int nbSlice);
__global__ void ecrasementGM(float* tabGM, int moitier);

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

static __device__ float aireRectangle(float xs, int nbSlice);
static __device__ float f(float x);

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

/**
 * Chaque thread effecteur une reduction avec le patern d'entrelacement,
 * puis stocke son résultat dans SA case dans tabGM
 * tabGM est un tableau promu, qui a autant de case que de thread
 */
__global__ void reductionIntraThread(float* tabGM, int nbSlice)
    {
    // TODO SliceNaif
    const int TID = Indice2D::tid();
    int s = TID;
    const int NB_THREAD = Indice2D::nbThread();
    float sum = 0.0;
    float xi;
    while (s < nbSlice)
	{
	xi = s / (float) nbSlice;
	sum += (1 / (float) nbSlice) * f(xi);
	s += NB_THREAD;
	}
    tabGM[TID] = sum;
    }

/**
 * Pour version NaivePLus
 *
 * Ramene la semantique du tableau pour une reduction, dans la premiere moitier, par ecraseemt
 * Doit etre appeler plusieurs fois coter host, pour tout ramener dans tabGM[0]
 */
__global__ void ecrasementGM(float* tabGM, int moitier)
    {
    // TODO SliceNaif
    const int TID = Indice2D::tid();
    int s = TID;
    const int NB_THREAD = Indice2D::nbThread();
    while (s < moitier)
	{
	tabGM[s] += tabGM[moitier + s];
	s += NB_THREAD;
	}
    }

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/*-----------------------*\
 | Reduction Intrathread  *|
 \*-----------------------*/

__device__ float aireRectangle(float xs, int nbSlice)
    {
    // TODO SliceNaif
    return ((float) 1 / (float) nbSlice) * f(xs);
    }

__device__ float f(float x)
    {
    // TODO SliceNaif
    return 4 / (float) (1 + x * x);
    }

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

