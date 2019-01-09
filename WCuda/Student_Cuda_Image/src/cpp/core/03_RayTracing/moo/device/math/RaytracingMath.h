#pragma once

#include <math.h>
#include "MathTools.h"

#include "Sphere.h"
#define FLOAT_MAX 1e+6

#include "ColorTools_GPU.h"
using namespace gpu;

class RaytracingMath
    {
    public:

	/*--------------------------------------*\
	|*		Constructor		*|
	 \*-------------------------------------*/

	__device__ RaytracingMath(Sphere* ptrDevTabSpheres, int nbSpheres)
	    {
	    this->ptrDevTabSpheres = ptrDevTabSpheres;
	    this->nbSpheres = nbSpheres;
	    }

	// constructeur copie automatique car pas pointeur dans RaytracingMath

	__device__
		        virtual ~RaytracingMath()
	    {
	    // rien
	    }

	/*--------------------------------------*\
	|*		Methodes		*|
	 \*-------------------------------------*/

    public:

	__device__
	void color(uchar4* ptrColorij, int i, int j, float t)
	    {
	    float2 xySol;
	    xySol.x = i;
	    xySol.y = j;

	    color(xySol, t, ptrColorij); // update colorij

	    ptrColorij->w = 255; // opacity facultatif

	    // debug
		{
//	    ptrColorij->x = 120;
//	    ptrColorij->y = 120;
//	    ptrColorij->z = 120;
		}
	    }

    private:

	__device__
	void color(const float2& xySol, float t, uchar4* ptrColorXY)
	    {
	    // TODO Raytracing GPU math
	    // process a color for the pixel (x,y)
	    // use methode of classe Sphere
	    float distanceMin = FLOAT_MAX;
	    float brightness;
	    float hue;

	    for(int i=0;i<=nbSpheres;i++)
		{
		float h2 = ptrDevTabSpheres[i].hCarre(xySol);
		if(ptrDevTabSpheres[i].isEnDessous(h2))
		    {
		    float dz = ptrDevTabSpheres[i].dz(h2);
		    float distance = ptrDevTabSpheres[i].distance(dz);
		    if(distance<distanceMin)
			{
			distanceMin=distance;
			brightness = ptrDevTabSpheres[i].brightness(dz);
			hue = ptrDevTabSpheres[i].hue(t);
			}
		    }
		}
	    if(distanceMin==FLOAT_MAX)
		{
		    ptrColorXY->x = 0;
		    ptrColorXY->y = 0;
		    ptrColorXY->z = 0;
		}
	    else
		{
		    ColorTools::HSB_TO_RVB(hue,1,brightness, ptrColorXY);
		}


	    }

	/*--------------------------------------*\
	|*		Attributs		*|
	 \*-------------------------------------*/

    private:

	// Input
	Sphere* ptrDevTabSpheres;
	int nbSpheres;

    };

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
