#include <iostream>
#include <assert.h>

#include "Device.h"

#include "IndiceTools_GPU.h"
#include "Rect2D.h"
#include "HeatTransfertTex.h"

using std::cout;
using std::endl;

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Imported	 	*|
 \*-------------------------------------*/
extern __host__ void initTexture(float* ptrDevHeater, int w, int h);
extern __global__ void toScreenImageHSB(uchar4* ptrDevPixels, float* ptrTabFloatPixels, uint w, uint h, Calibreur<float> calibreur);
extern __global__ void diffusion(float* ptrDevInput, float* ptrDevOutput, uint w, uint h);
extern __global__ void ecrasementTex(float* ptrDevOutput, uint w, uint h);

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*-------------------------*\
 |*	Constructeur	    *|
 \*-------------------------*/

HeatTransfertTex::HeatTransfertTex(const Grid& grid, uint w, uint h, int nMin, int nMax) :
	Animable_I<uchar4>(grid, w, h, "HeatTransfertTex_Texture_Cuda_RGBA_uchar4"), variateurT(Interval<int>(nMin, nMax), 1)
    {
    // Inputs/animation
    this->nMin = nMin;
    this->nMax = nMax;

    // Tools
    sizePencil = 5;
    iteration_aveugle = 0;
    valuePencil = 1.0;

    heaterPersistant = true;
    ecrasementFlag = true;
    isImageAInput = true;

    Interval<float> start(0, 1);
    Interval<float> end(0.7, 0);

    calibreur = new Calibreur<float>(start, end);

    this->t = nMin;

    this->ptrImageInit = new float[w * h];
    for (int i = 0; i < w * h; i++)
	{
	ptrImageInit[i] = 0.0;
	}

    Rect2D tabRectCooler[8] =
		{
		Rect2D(179, 179, 16, 16), Rect2D(605, 179, 16, 16), Rect2D(179, 605, 16, 16), Rect2D(605, 605, 16, 16), Rect2D(400, 295, 5, 5), Rect2D(295, 400,
			5, 5), Rect2D(505, 400, 5, 5), Rect2D(400, 505, 5, 5)
		};
    Rect2D tabRectHeater[1] =
	{
	Rect2D(300, 300, 200, 200)
	};

    this->ptrImageHeater = new float[w * h];

    for (int i = 0; i < w * h; i++)
	{
	ptrImageHeater[i] = 0.0;
	}

    for (int n = 0; n < 8; n++)
	{
	for (int i = tabRectCooler[n].y; i < tabRectCooler[n].y + tabRectCooler[n].height; i++)
	    {
	    for (int j = tabRectCooler[n].x; j < tabRectCooler[n].x + tabRectCooler[n].height; j++)
		{
		ptrImageHeater[j * w + i] = 0.2;
		}
	    }
	}
    for (int n = 0; n < 1; n++)
	{
	for (int i = tabRectHeater[n].y; i < tabRectHeater[n].y + tabRectHeater[n].height; i++)
	    {
	    for (int j = tabRectHeater[n].x; j < tabRectHeater[n].x + tabRectHeater[n].height; j++)
		{
		ptrImageHeater[j * w + i] = 1.0;
		}
	    }
	}

    this->sizeTabFloat = w * h * sizeof(float);

    Device::malloc(&ptrDevImageA, sizeTabFloat);
    Device::malloc(&ptrDevImageB, sizeTabFloat);

    Device::malloc(&ptrDevImageInit, sizeTabFloat);
    Device::memcpyHToD(ptrDevImageA, ptrImageInit, sizeTabFloat);
    Device::malloc(&ptrDevImageHeater, sizeTabFloat);
    Device::memcpyHToD(ptrDevImageHeater, ptrImageHeater, sizeTabFloat);

    initTexture(ptrDevImageHeater, w, h);

    ecrasementTex<<<dg,db>>>(ptrDevImageA, w, h);
}

HeatTransfertTex::~HeatTransfertTex()
{
Device::free(ptrDevImageA);
Device::free(ptrDevImageB);
Device::free(ptrDevImageInit);
Device::free(ptrDevImageHeater);
delete calibreur;
}

/*-------------------------*\
 |*	Methode		    *|
 \*-------------------------*/

void HeatTransfertTex::resetHeaters()
{
for (int i = 0; i < w * h; i++)
    {
    ptrImageHeater[i] = 0.0;
    }
}

void HeatTransfertTex::setRubber()
{
valuePencil = 0.0;
}

void HeatTransfertTex::setHeater()
{
valuePencil = 1.0;
}

void HeatTransfertTex::setCooler()
{
valuePencil = 0.2;
}

void HeatTransfertTex::togglePersistant()
{
heaterPersistant = !heaterPersistant;
Device::memcpyHToD(ptrDevImageHeater, ptrImageHeater, sizeTabFloat);
}

void HeatTransfertTex::updateSizePencil(int direction)
{
if (direction == 1)
    {
    sizePencil--;
    }
else
    {
    sizePencil++;
    }
if (sizePencil > MAX_SIZE_PENCIL)
    sizePencil = MAX_SIZE_PENCIL;
if (sizePencil < 1)
    sizePencil = 1;
cout << "Size pencil : " << sizePencil << endl;
}

void HeatTransfertTex::createHeater(int x, int y)
{
for (int i = x - sizePencil; i < x + sizePencil; i++)
    {
    for (int j = y - sizePencil; j < y + sizePencil; j++)
	{
	if (i > 1 && j > 1 && i < w && j < h)
	    ptrImageHeater[j * w + i] = valuePencil;
	}
    }
Device::memcpyHToD(ptrDevImageHeater, ptrImageHeater, sizeTabFloat);
ecrasementFlag = true;
}

/**
 * Override
 * Call periodicly by the API
 */
void HeatTransfertTex::process(uchar4* ptrDevPixels, uint w, uint h, const DomaineMath& domaineMath)
{
if (isImageAInput)
    {
    diffusion<<<dg,db>>>(ptrDevImageA, ptrDevImageB, w, h);
    if (heaterPersistant || ecrasementFlag)
	{
    ecrasementTex<<<dg,db>>>(ptrDevImageB, w, h);
    }
}
else
{
diffusion<<<dg,db>>>(ptrDevImageB, ptrDevImageA, w, h);
if (heaterPersistant || ecrasementFlag)
    {
ecrasementTex<<<dg,db>>>(ptrDevImageA, w, h);
}
}
isImageAInput = !isImageAInput;
if (ecrasementFlag && !heaterPersistant)
{
ecrasementFlag = false;
resetHeaters();
}

if (iteration_aveugle >= NB_ITERATION_AVEUGLE)
{
iteration_aveugle = 0;
toScreenImageHSB<<<dg,db>>>(ptrDevPixels, ptrDevImageA, w, h, *calibreur);
}

iteration_aveugle++;
}

/**
 * Override
 * Call periodicly by the API
 */
void HeatTransfertTex::animationStep()
{
this->t = variateurT.varierAndGet();
}

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

