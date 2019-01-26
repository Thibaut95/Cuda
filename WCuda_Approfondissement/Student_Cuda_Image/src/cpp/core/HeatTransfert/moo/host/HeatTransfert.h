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

	HeatTransfert(const Grid& grid, uint w, uint h, int nMin, int nMax);
	virtual ~HeatTransfert(void);

	/*--------------------------------------*\
	 |*		Methodes		*|
	 \*-------------------------------------*/

    public:

	void setRubber();
	void setHeater();
	void setCooler();
	void togglePersistant();
	void updateSizePencil(int direction);
	void createHeater(int x, int y);

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

	void resetHeaters();

	// Inputs
	int nMin;
	int nMax;

	// Tools
	Variateur<int> variateurT;
	Calibreur<float>* calibreur;

	int sizePencil;
	float valuePencil;

	size_t sizeTabFloat;
	float* ptrImageHeater;
	float* ptrImageInit;
	float* ptrDevImageHeater;
	float* ptrDevImageA;
	float* ptrDevImageB;

	int iteration_aveugle;

	bool heaterPersistant;
	bool ecrasementFlag;
	bool isImageAInput;

	// Constante
	const bool TEXTUREMODE=true;
	const int MAX_SIZE_PENCIL = 100;
	const int NB_ITERATION_AVEUGLE = 50;

    };

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
