#include "Indice2D.h"
#include "cudaTools.h"
#include "Device.h"

#include "HeatTransfertMath.h"  // a decommenter une fois coder

#include "DomaineMath_GPU.h"
#include "IndiceTools_GPU.h"
using namespace gpu;

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public		 	*|
 \*-------------------------------------*/

__global__ void heatTransfert(uchar4* ptrDevPixels, uint w, uint h, DomaineMath domaineMath, int n);

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

__global__ void heatTransfert(uchar4* ptrDevPixels, uint w, uint h, DomaineMath domaineMath, int n)
    {
    // TODO Mandelbrot GPU :
    // build MandelbrotMath
    // entrelacement
    HeatTransfertMath heatTransfertMath = HeatTransfertMath(n);

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

	heatTransfertMath.colorIJ(&ptrDevPixels[s], i, j, n, domaineMath);

	s += NB_THREAD;
	}
    }

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

