#pragma once

#include <math.h>
#include "MathTools.h"

#include "DomaineMath_GPU.h"
#include "ColorTools_GPU.h"
#include "Calibreur_GPU.h"
using namespace gpu;

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

class HeatTransfertMath
    {
	/*--------------------------------------*\
	|*		Constructeur		*|
	 \*-------------------------------------*/

    public:

	__device__ HeatTransfertMath(int n):calibreur(Interval<float>(1, n), Interval<float>(0, 1))
	    {
	    //
	    }

	// constructeur copie: pas besoin car pas attribut ptr
	__device__
	   virtual ~HeatTransfertMath(void)
	    {
	    // rien
	    }

	/*--------------------------------------*\
	|*		Methode			*|
	 \*-------------------------------------*/

    public:
	__device__
	void colorIJ(uchar4* ptrColorIJ, int i, int j, int n, const DomaineMath& domaineMath)
	    {
	    double x;
	    double y;

	    // TODO Mandelbrot CPU
	    // Convertir (i,j) en (x,y)
	    // Calculer la suite en (x,y)
	    // Colorier
	    domaineMath.toXY(i, j, &x, &y);
	    int k = suite(x,y,n);
	    if(k > n)
		{
		ptrColorIJ->x = 0;
		ptrColorIJ->y = 0;
		ptrColorIJ->z = 0;
		}
	    else
		{
		float hue = k;
		calibreur.calibrer(&hue);
		ColorTools::HSB_TO_RVB(hue, ptrColorIJ);
		}

	    ptrColorIJ->w = 255; //opaque

	    }

    private:
	__device__
	int suite(double x, double y, int n)
	    {
	    // TODO Mandelbrot CPU
	    // Calculer la suite en (x,y) jusqu'à n, à moins que critere arret soit atteint avant
	    // return le nombre d'element de la suite calculer
	    int k = 0;
	    double a = 0;
	    double b = 0;
	    while (k <= n && (a * a + b * b) <= 4)
		{
		double aCopy = a;
		a = (a * a - b * b) + x;
		b = (double)2 * aCopy * b + y;
		k++;
		}
	    return k;

	    }

	/*--------------------------------------*\
	|*		Attribut		*|
	 \*-------------------------------------*/

    private:

	// rien
	Calibreur<float> calibreur;

    };

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
