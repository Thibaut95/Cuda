#include <omp.h>
#include "OmpTools.h"
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

bool isPiOMPEntrelacerPromotionTab_Ok(int n);

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

static double piOMPEntrelacerPromotionTab(int n);

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

bool isPiOMPEntrelacerPromotionTab_Ok(int n)
    {
    return isAlgoPI_OK(piOMPEntrelacerPromotionTab, n, "Pi OMP Entrelacer promotionTab");
    }

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/**
 * pattern cuda : excellent!
 */
double piOMPEntrelacerPromotionTab(int n)
    {
    const int NBTHREAD = OmpTools::setAndGetNaturalGranularity();
    double tab[NBTHREAD];

#pragma omp parallel
	{
	const int TID = OmpTools::getTid();
	int s = TID;
	double xi = 0.0;
	double sum = 0.0;

	while (s < n)
	    {
	    xi = (double) s / (double) n;
	    sum += fpi(xi);
	    s += NBTHREAD;
	    }

	tab[TID] = sum;
	}

    double sum_tot = 0.0;
    for (int i = 0; i <= NBTHREAD; i++)
	{
	sum_tot += tab[i];
	}
    return sum_tot / n;
    }

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

