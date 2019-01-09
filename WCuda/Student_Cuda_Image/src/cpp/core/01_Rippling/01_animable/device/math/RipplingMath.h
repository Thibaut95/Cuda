#pragma once

#include <math.h>
#include "MathTools.h"

#include "ColorTools_GPU.h"
using namespace gpu;

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

class RipplingMath
    {
	/*--------------------------------------*\
	|*		Constructeur		*|
	 \*-------------------------------------*/

    public:

	__device__ RipplingMath(uint w, uint h)
	    {
	    this->dim2 = w / 2.f;
	    }

	// constructeur copie: pas besoin car pas attribut ptr

	__device__  virtual ~RipplingMath(void)
	    {
	    // rien
	    }

	/*--------------------------------------*\
	|*		Methode			*|
	 \*-------------------------------------*/

    public:

	__device__
	void colorIJ(uchar4* ptrColorIJ, int i, int j, float t)
	    {
	    uchar levelGris;

	    f(j, i, t, &levelGris); // update levelgray

	    ptrColorIJ->x = levelGris;
	    ptrColorIJ->y = levelGris;
	    ptrColorIJ->z = levelGris;

	    ptrColorIJ->w = 255; //opaque
	    }

    private:

	__device__
	void f(int i, int j, float t, uchar* ptrlevelGris)
	    {
	    float result;
	    dij(i, j, &result); // warning : dij return void. Ne peut pas etre "imbriquer dans une fonction"

	    result = result / 10;
	    // TODO Rippling CPU : cf formules math rippling.pdf
	    *ptrlevelGris = (cosf(result - t / 7) / (result + 1)) * 127 + 128;
	    }

	__device__
	void dij(int i, int j, float* ptrResult)
	    {
	    //TODO Rippling CPU cf fonction math pdf
	    i -= dim2;
	    j -= dim2;
	    *ptrResult = sqrtf(i * i + j * j);
	    }

	/*--------------------------------------*\
	|*		Attribut			*|
	 \*-------------------------------------*/

    private:

	// Tools
	float dim2;

    };

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
