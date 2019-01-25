#include <iostream>
#include <stdlib.h>

#include "HeatTransfert/provider/HeatTransfertProvider.h"

#include "Animateur_GPU.h"
#include "Settings_GPU.h"
using namespace gpu;

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

int mainAnimable(Settings& settings);

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

static void heatTransfert();


// Tools
template<typename T>
static void animer(Provider_I<T>* ptrProvider, int nbIteration);

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

int mainAnimable(Settings& settings)
    {
    cout << "\n[Animable] mode" << endl;

    // Please, un a la fois!
    heatTransfert();


    cout << "\n[Animable] end" << endl;

    return EXIT_SUCCESS;
    }

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

void heatTransfert()
    {
    const int NB_ITERATION = 50000;

    HeatTransfertProvider provider;
    animer<uchar4>(&provider, NB_ITERATION);
    }


/*-----------------------------------*\
 |*		Tools	        	*|
 \*-----------------------------------*/

template<typename T>
void animer(Provider_I<T>* ptrProvider, int nbIteration)
    {
    Animateur<T> animateur(ptrProvider, nbIteration);
    animateur.run();
    }

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

