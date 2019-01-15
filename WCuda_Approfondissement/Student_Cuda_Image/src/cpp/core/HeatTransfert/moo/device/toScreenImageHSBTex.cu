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
 texture<float,2,cudaReadModeElementType> textureAB;
__global__ void toScreenImageHSBTex(uchar4* ptrDevPixels, uint w, uint h, Calibreur<float> calibreur);
__host__ void initTextureABHSB(float* ptrDevImageAB, int w, int h);

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

 __host__ void initTextureAB(float* ptrDevImageAB, int w, int h)
 {
    textureAB.addressMode[0]=cudaAddressModeClamp;
    textureAB.addressMode[1]=cudaAddressModeClamp;
    textureAB.filterMode=cudaFilterModePoint;
    textureAB.normalized=false;

 size_t pitch = w * sizeof(float);
 cudaChannelFormatDesc channelDesc = cudaCreateChannelDesc<float>();
 HANDLE_ERROR(cudaBindTexture2D(NULL,textureAB,ptrDevImageAB,channelDesc,w,h,pitch));
 }

__global__ void toScreenImageHSBTex(uchar4* ptrDevPixels, uint w, uint h, Calibreur<float> calibreur)
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

	float hue=tex2D(textureAB,j,i);
	calibreur.calibrer(&hue);
	ColorTools::HSB_TO_RVB(hue, &ptrDevPixels[s]);

	s += NB_THREAD;
	}
    }

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

