#include "Indice2D.h"
#include "cudaTools.h"
#include "Device.h"

#include "DomaineMath_GPU.h"
#include "IndiceTools_GPU.h"
#include "Calibreur_GPU.h"
#include "ColorTools_GPU.h"
using namespace gpu;

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public		 	*|
 \*-------------------------------------*/

__global__ void diffusion(float* ptrDevInput, float* ptrDevOutput, uint w, uint h);

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

__global__ void diffusion(float* ptrDevInput, float* ptrDevOutput, uint w, uint h)
    {
    const int TID = Indice2D::tid();
    const int NB_THREAD = Indice2D::nbThread();
    const int WH = (w-2) * (h-2);

    int s = TID;

    while (s < WH)
	{
	int sReal = s+w+1;
	ptrDevOutput[sReal]=ptrDevInput[sReal]+0.25*(ptrDevInput[sReal+1]+ptrDevInput[sReal-1]+ptrDevInput[sReal+w]+ptrDevInput[sReal-w]-4*ptrDevInput[sReal]);

	s += NB_THREAD;
	}
    }

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

