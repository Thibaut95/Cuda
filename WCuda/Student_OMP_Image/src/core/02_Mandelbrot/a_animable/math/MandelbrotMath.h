#pragma once

#include <math.h>
#include "MathTools.h"

#include "DomaineMath_CPU.h"
#include "ColorTools_CPU.h"
using namespace cpu;

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

class MandelbrotMath
    {
	/*--------------------------------------*\
	|*		Constructeur		*|
	 \*-------------------------------------*/

    public:

	MandelbrotMath()
	    {
	    //
	    }

	// constructeur copie: pas besoin car pas attribut ptr

	virtual ~MandelbrotMath(void)
	    {
	    // rien
	    }

	/*--------------------------------------*\
	|*		Methode			*|
	 \*-------------------------------------*/

    public:

	void colorIJ(uchar4* ptrColorIJ, int i, int j, int n, const DomaineMath& domaineMath)
	    {
	    double x;
	    double y;

	    // TODO Mandelbrot CPU
	    // Convertir (i,j) en (x,y)
	    // Calculer la suite en (x,y)
	    // Colorier
	    }

    private:

	int suite(double x, double y, int n)
	    {
	// TODO Mandelbrot CPU
	    // Calculer la suite en (x,y) jusqu'à n, à moins que critere arret soit atteint avant
	    // return le nombre d'element de la suite calculer
	    }

	/*--------------------------------------*\
	|*		Attribut		*|
	 \*-------------------------------------*/

    private:

	// rien

    };

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
