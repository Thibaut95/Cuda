#include <iostream>
#include "Grid.h"
#include "Device.h"
#include "VectorTools.h"
#include "AddVector.h"

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

bool isAddVecteurOk();
bool isAddVecteurOk(const Grid& grid);

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
 */
bool isAddVecteurOk(const Grid& grid)
    {
    int n = 9;

    float* ptrV1 = VectorTools::createV1(n);
    float* ptrV2 = VectorTools::createV2(n);
    float* ptrW = new float[n];

    // Partie interessante GPGPU
	{
	// TODO use AddVector
	AddVector addVector(grid, ptrV1, ptrV2, ptrW, n);
	addVector.run();
	}

    // check
	{
	VectorTools::print(ptrV1, n);
	VectorTools::print(ptrV2, n);
	cout << "--------------------------------------------------------------------" << endl;
	VectorTools::print(ptrW, n);
	}

    bool isOk = VectorTools::isAddVector_Ok(ptrV1, ptrV2, ptrW, n);

    delete[] ptrV1;
    delete[] ptrV2;
    delete[] ptrW;

    return isOk;
    }

bool isAddVecteurOk()
    {
    int mp = Device::getMPCount();
    int coreMP = Device::getCoreCountMP();

//    dim3 dg = dim3(mp, 2, 1);  		// disons, a optimiser selon le gpu, peut drastiqument ameliorer ou baisser les performances
//    dim3 db = dim3(coreMP, 2, 1);   	// disons, a optimiser selon le gpu, peut drastiqument ameliorer ou baisser les performances
//    Grid grid(dg, db);

    dim3 dg = dim3(10, 10, 1);  		// disons, a optimiser selon le gpu, peut drastiqument ameliorer ou baisser les performances
    dim3 db = dim3(3, 1, 1);   	// disons, a optimiser selon le gpu, peut drastiqument ameliorer ou baisser les performances
    Grid grid(dg, db);

    return isAddVecteurOk(grid);
    }

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

