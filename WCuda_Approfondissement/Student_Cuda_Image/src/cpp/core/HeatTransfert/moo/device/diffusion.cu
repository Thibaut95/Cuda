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
    const int WH = w * h;

    int i;
    int j;

    int s = TID;

    while (s < WH)
	{
	IndiceTools::toIJ(s, w, &i, &j);

	if(i>1 && j >1 && i<w && j<h)
	    {
	    ptrDevOutput[s]=ptrDevInput[s]+0.25*(ptrDevInput[s+1]+ptrDevInput[s-1]+ptrDevInput[s+w]+ptrDevInput[s-w]-4*ptrDevInput[s]);
	    }



//	TODO
//	float hue=ptrTabFloatPixels[s];
//	calibreur.calibrer(&hue);
//	ColorTools::HSB_TO_RVB(hue, &ptrDevPixels[s]);

	s += NB_THREAD;
	}
    }

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

