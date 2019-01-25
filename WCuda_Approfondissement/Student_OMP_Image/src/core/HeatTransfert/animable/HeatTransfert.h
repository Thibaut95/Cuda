#pragma once

#include "MathTools.h"

#include "Animable_I_CPU.h"
#include "Variateur_CPU.h"
#include "Calibreur_CPU.h"

using namespace cpu;

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

class HeatTransfert: public Animable_I<uchar4>
    {
	/*--------------------------------------*\
	|*		Constructor		*|
	 \*-------------------------------------*/

    public:

	HeatTransfert(uint w, uint h, int nMin, int nMax);
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
	virtual void processEntrelacementOMP(uchar4* ptrTabPixels, unsigned int w, unsigned int h, const DomaineMath& domaineMath);
	virtual void processForAutoOMP(uchar4* ptrTabPixels, unsigned int w, unsigned int h, const DomaineMath& domaineMath);

	/**
	 * Call periodicly by the api
	 */
	virtual void animationStep();

	/*--------------------------------------*\
	 |*		Attributs		*|
	 \*-------------------------------------*/

    private:
	void diffusionFor(float* ptrInput, float* ptrOutput);
	void ecrasementFor(float* ptrImageHeater, float* ptrOutput);
	void toScreenImageHSBFor(uchar4* ptrDevPixels, float* ptrTabFloatPixels);

	void diffusionEntr(float* ptrInput, float* ptrOutput);
	void ecrasementEntr(float* ptrImageHeater, float* ptrOutput);
	void toScreenImageHSBEntr(uchar4* ptrDevPixels, float* ptrTabFloatPixels);

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
	float* ptrImageA;
	float* ptrImageB;

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
