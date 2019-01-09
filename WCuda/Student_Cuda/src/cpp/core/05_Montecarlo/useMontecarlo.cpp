#include <iostream>
#include "Grid.h"
#include "Device.h"

using std::cout;
using std::endl;

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Imported	 	*|
 \*-------------------------------------*/

#include "Montecarlo.h"

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

bool useMontecarlo(void);

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

bool useMontecarlo()
    {
    int n = 100000000;

    // Partie interessante GPGPU
	{
	// Grid cuda
	int mp = Device::getMPCount();
	int coreMP = Device::getCoreCountMP();

	// Entrelacement
	dim3 dg = dim3(mp, 1, 1);
	dim3 db = dim3(1024, 1, 1);
	Grid grid(dg, db);

	Montecarlo montecarlo(n, grid);

	montecarlo.run();
	cout << "Montecarlo : " << montecarlo.getPi() << endl;
	}
    return true;
    }

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
