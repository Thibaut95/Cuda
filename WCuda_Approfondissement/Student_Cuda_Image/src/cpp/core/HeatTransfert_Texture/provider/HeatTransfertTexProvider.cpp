#include "../../HeatTransfert_Texture/provider/HeatTransfertTexProvider.h"

#include <assert.h>
#include "MathTools.h"
#include "Grid.h"
#include "../../HeatTransfert/event/ImageCustomEvent.h"
#include "../moo/host/HeatTransfertTex.h"

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Override		*|
 \*-------------------------------------*/

/**
 * Override
 */
Animable_I<uchar4>* HeatTransfertTexProvider::createAnimable()
    {
    // Animation;
    int nMin = 20;
    int nMax = 120;

    // Dimension
    int w = 800;
    int h = 800;

    // grid cuda
    int mp = Device::getMPCount();
    int coreMP = Device::getCoreCountMP();

    dim3 dg = dim3(mp, 2, 1);  		// disons, a optimiser selon le gpu, peut drastiqument ameliorer ou baisser les performances
    dim3 db = dim3(coreMP, 2, 1);   	// disons, a optimiser selon le gpu, peut drastiqument ameliorer ou baisser les performances
    Grid grid(dg, db);

    return new HeatTransfertTex(grid, w, h, nMin, nMax);
    }

/**
 * Override
 */
Image_I* HeatTransfertTexProvider::createImageGL(void)
    {
    ColorRGB_01 colorTexte(0, 1, 0); // Green
    return new ImageCustomEvent(createAnimable(), colorTexte);
    }

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
