#include <omp.h>
#include "MathTools.h"
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

bool isPiOMPforPromotionTab_Ok(int n);

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

static double piOMPforPromotionTab(int n);
static void syntaxeSimplifier(double* tabSumThread, int n);
static void syntaxeFull(double* tabSumThread, int n);

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

bool isPiOMPforPromotionTab_Ok(int n)
    {
    return isAlgoPI_OK(piOMPforPromotionTab, n, "Pi OMP for promotion tab");
    }

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/**
 * De-synchronisation avec PromotionTab
 */
double piOMPforPromotionTab(int n)
    {
    const int NBTHREAD = OmpTools::setAndGetNaturalGranularity();
    double tab[NBTHREAD];

    for (int i = 0; i <= NBTHREAD; i++)
	{
	tab[i] = 0;
	}

#pragma omp parallel for
    for (int i = 0; i < n; i++)
	{
	double xi = i / (double) n;
	tab[OmpTools::getTid()] += fpi(xi);
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

