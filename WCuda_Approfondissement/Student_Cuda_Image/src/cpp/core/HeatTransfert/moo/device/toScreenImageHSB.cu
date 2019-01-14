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

__global__ void toScreenImageHSB(uchar4* ptrDevPixels, float* ptrTabFloatPixels, uint w, uint h, Calibreur<float> calibreur);

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

__global__ void toScreenImageHSB(uchar4* ptrDevPixels, float* ptrTabFloatPixels, uint w, uint h, Calibreur<float> calibreur)
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

	float hue=ptrTabFloatPixels[s];
	calibreur.calibrer(&hue);
	ColorTools::HSB_TO_RVB(hue, &ptrDevPixels[s]);

	s += NB_THREAD;
	}
    }

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

