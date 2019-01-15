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
__global__ void diffusionTex(float* ptrDevOutput, uint w, uint h);
__host__ void initTextureAB(float* ptrDevImageAB, int w, int h);

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

__global__ void diffusionTex(float* ptrDevOutput, uint w, uint h)
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

	if(i>1 && j >1 && j<w && i<h)
	    {
	    ptrDevOutput[s]=tex2D(textureAB,j,i)+0.25*(tex2D(textureAB,j+1,i)+tex2D(textureAB,j,i+1)]+tex2D(textureAB,j-1,i)+tex2D(textureAB,j,i-1)-4*tex2D(textureAB,j,i));
	    }

	s += NB_THREAD;
	}
    }

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

