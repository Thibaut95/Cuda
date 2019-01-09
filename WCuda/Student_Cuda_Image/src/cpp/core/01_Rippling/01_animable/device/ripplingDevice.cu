#include "Indice2D.h"
#include "cudaTools.h"
#include "Device.h"
#include "RipplingMath.h"

//#include "RipplingMath.h" // a decommenter une fois coder

#include "IndiceTools_GPU.h"
using namespace gpu;

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

__global__ void rippling(uchar4* ptrDevPixels, uint w, uint h, float t);

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

__global__ void rippling(uchar4* ptrDevPixels, uint w, uint h, float t)
    {
    // TODO instacier RipplingMath
    RipplingMath ripplingMath = RipplingMath(w, h);

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

	ripplingMath.colorIJ(&ptrDevPixels[s], i, j, t);

	s += NB_THREAD;
	}
    }

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

