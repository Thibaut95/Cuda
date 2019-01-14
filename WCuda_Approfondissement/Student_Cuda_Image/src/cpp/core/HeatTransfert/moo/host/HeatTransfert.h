#pragma once

#include "cudaTools.h"
#include "MathTools.h"

#include "Animable_I_GPU.h"
#include "Variateur_GPU.h"
#include "Calibreur_GPU.h"

using namespace gpu;


/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

class HeatTransfert: public Animable_I<uchar4>
    {
	/*--------------------------------------*\
	|*		Constructor		*|
	 \*-------------------------------------*/

    public:

	HeatTransfert(const Grid& grid,uint w, uint h, const DomaineMath& domaineMath, int nMin, int nMax);
	virtual ~HeatTransfert(void);

	/*--------------------------------------*\
	 |*		Methodes		*|
	 \*-------------------------------------*/

    public:

	/*-------------------------*\
	|*   Override Animable_I   *|
	 \*------------------------*/

	/**
	 * Call periodicly by the api
	 */
	virtual void process(uchar4* ptrDevPixels, uint w, uint h, const DomaineMath& domaineMath);

	/**
	 * Call periodicly by the api
	 */
	virtual void animationStep();

	/*--------------------------------------*\
	 |*		Attributs		*|
	 \*-------------------------------------*/

    private:

	// Inputs
	int nMin;
	int nMax;
	float* ptrDevTabFloat;
	size_t sizeTabFloat;
	float* ptrTabFloat;

	float* ptrImageHeater;
	float* ptrImageInit;
	float* ptrDevImageHeater;
	float* ptrDevImageInit;
	float* ptrDevImageA;
	float* ptrDevImageB;

	const int NB_ITERATION_AVEUGLE = 50;

	// Tools
	Variateur<int> variateurT;

    };


/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
