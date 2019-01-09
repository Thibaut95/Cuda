#include <omp.h>
#include <OmpTools.h>

#include "00_pi_tools.h"

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Imported	 	*|
 \*-------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

bool isPiOMPEntrelacerCritical_Ok(int n);

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

static double piOMPEntrelacerCritical(int n);

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

bool isPiOMPEntrelacerCritical_Ok(int n)
    {
    return isAlgoPI_OK(piOMPEntrelacerCritical, n, "Pi OMP Entrelacer critical");
    }

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

double piOMPEntrelacerCritical(int n)
    {
    const int NBTHREAD = OmpTools::setAndGetNaturalGranularity();
    double sum = 0.0;

#pragma omp parallel
	{
	const int TID = OmpTools::getTid();
	int s = TID;
	double xi = 0.0;
	double sumThread = 0.0;

	while (s < n)
	    {
	    xi = (double) s / (double) n;
	    sumThread += fpi(xi);
	    s += NBTHREAD;
	    }
#pragma omp critical(critical1)
	    {
	    sum += sumThread;
	    }

	}

    return sum / n;
    }

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

