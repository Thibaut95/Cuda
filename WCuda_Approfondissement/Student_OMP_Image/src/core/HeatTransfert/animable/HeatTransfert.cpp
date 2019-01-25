#include "../../../HeatTransfert/animable/HeatTransfert.h"

#include <iostream>
#include <assert.h>

#include "ColorTools_CPU.h"
#include "IndiceTools_CPU.h"
#include "Rect2D.h"

using std::cout;
using std::endl;

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Imported	 	*|
 \*-------------------------------------*/

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*-------------------------*\
 |*	Constructeur	    *|
 \*-------------------------*/

HeatTransfert::HeatTransfert(uint w, uint h, int nMin, int nMax) :
	Animable_I<uchar4>(w, h, "HeatTransfert_Cuda_RGBA_uchar4"), variateurT(Interval<int>(nMin, nMax), 1)
    {
    // Inputs/animation
    this->nMin = nMin;
    this->nMax = nMax;

    // Tools
    sizePencil = 5;
    valuePencil = 1.0;

    iteration_aveugle = 0;

    heaterPersistant = true;
    ecrasementFlag = true;
    isImageAInput = true;

    Interval<float> start(0, 1);
    Interval<float> end(0.7, 0);

    calibreur = new Calibreur<float>(start, end);

    this->t = nMin;

    this->ptrImageB = new float[w * h];

    this->ptrImageA = new float[w * h];
    for (int i = 0; i < w * h; i++)
	{
	ptrImageA[i] = 0.0;
	}

    //Initial heaters
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

    ecrasementFor(ptrImageHeater, ptrImageA);
    }

HeatTransfert::~HeatTransfert()
    {
    delete ptrImageHeater;
    delete ptrImageA;
    delete ptrImageB;
    delete calibreur;
    }

/*-------------------------*\
 |*	Methode		    *|
 \*-------------------------*/

void HeatTransfert::resetHeaters()
    {
    for (int i = 0; i < w * h; i++)
	{
	ptrImageHeater[i] = 0.0;
	}
    }

void HeatTransfert::setRubber()
    {
    valuePencil = 0.0;
    }

void HeatTransfert::setHeater()
    {
    valuePencil = 1.0;
    }

void HeatTransfert::setCooler()
    {
    valuePencil = 0.2;
    }

void HeatTransfert::togglePersistant()
    {
    heaterPersistant = !heaterPersistant;
    }

void HeatTransfert::updateSizePencil(int direction)
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
	{
	sizePencil = MAX_SIZE_PENCIL;
	}
    else if (sizePencil < 1)
	{
	sizePencil = 1;
	}
    }

void HeatTransfert::createHeater(int x, int y)
{
    for (int i = x - sizePencil; i < x + sizePencil; i++)
    {
    for (int j = y - sizePencil; j < y + sizePencil; j++)
	{
	if (i > 1 && j > 1 && i < w && j < h)
	    ptrImageHeater[j * w + i] = valuePencil;
	}
    }
    ecrasementFlag = true;
}

void HeatTransfert::processEntrelacementOMP(uchar4* ptrDevPixels, unsigned int w, unsigned int h, const DomaineMath& domaineMath)
    {
    float* ptrImageInput;
    float* ptrImageOutput;

    if (isImageAInput)
    {
    ptrImageInput = ptrImageA;
    ptrImageOutput = ptrImageB;
    }
    else
    {
    ptrImageInput = ptrImageB;
    ptrImageOutput = ptrImageA;
    }

    diffusionEntr(ptrImageInput, ptrImageOutput);
    if (heaterPersistant || ecrasementFlag)
    {
    ecrasementEntr(ptrImageHeater, ptrImageOutput);
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
    toScreenImageHSBEntr(ptrDevPixels, ptrImageOutput);
    }
    iteration_aveugle++;
    }

/**
 * Override
 * Call periodicly by the API
 */
void HeatTransfert::processForAutoOMP(uchar4* ptrDevPixels, uint w, uint h, const DomaineMath& domaineMath)
    {
    float* ptrImageInput;
    float* ptrImageOutput;

    if (isImageAInput)
	{
	ptrImageInput = ptrImageA;
	ptrImageOutput = ptrImageB;
	}
    else
	{
	ptrImageInput = ptrImageB;
	ptrImageOutput = ptrImageA;
	}

	diffusionFor(ptrImageInput, ptrImageOutput);
	if (heaterPersistant || ecrasementFlag)
	{
	ecrasementFor(ptrImageHeater, ptrImageOutput);

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
	toScreenImageHSBFor(ptrDevPixels, ptrImageOutput);
	}
    iteration_aveugle++;
    }

/**
 * Override
 * Call periodicly by the API
 */
void HeatTransfert::animationStep()
{
this->t = variateurT.varierAndGet();
}

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

void HeatTransfert::diffusionFor(float* ptrInput, float* ptrOutput)
    {
    const int WH = (w-2) * (h-2);

#pragma omp parallel for
    for(int s=0;s<WH;s++)
	{
	int sReal = s+w+1;
	ptrOutput[sReal]=ptrInput[sReal]+0.25*(ptrInput[sReal+1]+ptrInput[sReal-1]+ptrInput[sReal+w]+ptrInput[sReal-w]-4*ptrInput[sReal]);
	}

    }

void HeatTransfert::ecrasementFor(float* ptrImageHeater, float* ptrOutput)
    {
    const int WH = w * h;


#pragma omp parallel for
    for(int s=0;s<WH;s++)
	{
	if(ptrImageHeater[s]>0.0)
	    {
	    ptrOutput[s]=ptrImageHeater[s];
	    }

	}
    }

void HeatTransfert::toScreenImageHSBFor(uchar4* ptrDevPixels, float* ptrTabFloatPixels)
    {
    const int WH = w * h;


#pragma omp parallel for
    for(int s=0;s<WH;s++)
	{
	int i;
	int j;
	IndiceTools::toIJ(s, w, &i, &j);

	float hue=ptrTabFloatPixels[s];
	calibreur->calibrer(&hue);
	ColorTools::HSB_TO_RVB(hue, &ptrDevPixels[s]);
	}
    }

void HeatTransfert::diffusionEntr(float* ptrInput, float* ptrOutput)
    {
#pragma omp parallel
    {
    const int TID = OmpTools::getTid();
    const int NB_THREAD = OmpTools::getNbThread();
    const int WH = (w-2) * (h-2);

    int s = TID;

    while (s < WH)
    {
    int sReal = s+w+1;
    ptrOutput[sReal]=ptrInput[sReal]+0.25*(ptrInput[sReal+1]+ptrInput[sReal-1]+ptrInput[sReal+w]+ptrInput[sReal-w]-4*ptrInput[sReal]);

    s += NB_THREAD;
    }
    }
    }

void HeatTransfert::ecrasementEntr(float* ptrImageHeater, float* ptrOutput)
    {
#pragma omp parallel
    {
    const int TID = OmpTools::getTid();
    const int NB_THREAD = OmpTools::getNbThread();
    const int WH = w * h;

    int i;
    int j;

    int s = TID;

    while (s < WH)
    {
    if(ptrImageHeater[s]>0.0)
	{
	ptrOutput[s]=ptrImageHeater[s];
	}

    s += NB_THREAD;
    }
    }
    }

void HeatTransfert::toScreenImageHSBEntr(uchar4* ptrDevPixels, float* ptrTabFloatPixels)
    {
#pragma omp parallel
    {
    const int TID = OmpTools::getTid();
    const int NB_THREAD = OmpTools::getNbThread();
    const int WH = w * h;

    int i;
    int j;

    int s = TID;

    while (s < WH)
    {
    IndiceTools::toIJ(s, w, &i, &j);

    float hue=ptrTabFloatPixels[s];
    calibreur->calibrer(&hue);
    ColorTools::HSB_TO_RVB(hue, &ptrDevPixels[s]);

    s += NB_THREAD;
    }
    }
    }
