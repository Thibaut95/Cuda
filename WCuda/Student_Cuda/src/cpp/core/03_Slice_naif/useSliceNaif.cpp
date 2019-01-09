#include <iostream>
#include "Grid.h"
#include "Device.h"
#include "SliceNaif.h"
#include "LimitsTools.h"
#include "MathTools.h"

using std::cout;
using std::endl;

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Imported	 	*|
 \*-------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

bool isSliceNaifOK();
bool isSliceNaifOK(const Grid& grid);

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

/**
 * On passse la grille à useAddVecteur pour pouvoir facilement la faire varier de l'extérieur pour trouver l'optimum, ou faire des tests avec des grilles différentes
 * Contrainte : grid :  puissance de 2 pour etre compatible avec la version naivePlus
 */
bool isSliceNaifOK(const Grid& grid)
    {
    int nbSlice = LimitsTools::MAX_INT / 2;
    float piHat;

    // TODO use SliceNaif
    SliceNaif sliceNaif(grid, nbSlice, &piHat);
    sliceNaif.run();


    bool isOk = MathTools::isEquals((double) piHat, PI, 1e-4);

    return isOk;
    }

bool isSliceNaifOK()
    {
    //int mp = Device::getMPCount();
    int coreMP = Device::getCoreCountMP();

    dim3 dg = dim3(64, 1, 1);  		// disons, a optimiser selon le gpu, peut drastiqument ameliorer ou baisser les performances
    dim3 db = dim3(coreMP * 4, 1, 1);   		// disons, a optimiser selon le gpu, peut drastiqument ameliorer ou baisser les performances
    Grid grid(dg, db); // puissance de 2 pour etre compatible avec la version naivePlus

    return isSliceNaifOK(grid);
    }

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

