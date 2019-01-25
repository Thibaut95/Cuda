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

texture<float,2,cudaReadModeElementType> textureHeater;

__global__ void ecrasementTex(float* ptrDevOutput, uint w, uint h);
__host__ void initTextureHeater(float* ptrDevHeater, int w, int h);

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

__host__ void initTextureHeater(float* ptrDevHeater, int w, int h)
    {
    textureHeater.addressMode[0]=cudaAddressModeClamp;
    textureHeater.addressMode[1]=cudaAddressModeClamp;
    textureHeater.filterMode=cudaFilterModePoint;
    textureHeater.normalized=false;

    size_t pitch = w * sizeof(float);
    cudaChannelFormatDesc channelDesc = cudaCreateChannelDesc<float>();
    HANDLE_ERROR(cudaBindTexture2D(NULL,textureHeater,ptrDevHeater,channelDesc,w,h,pitch));
    }

__global__ void ecrasementTex(float* ptrDevOutput, uint w, uint h)
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

	if(tex2D(textureHeater,j,i)>0.0)
	    {
	    ptrDevOutput[s]=tex2D(textureHeater,j,i);
	    }

	s += NB_THREAD;
	}
    }

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/


