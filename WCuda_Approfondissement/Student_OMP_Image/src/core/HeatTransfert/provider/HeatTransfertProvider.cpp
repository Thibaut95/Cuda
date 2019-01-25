#include "../../HeatTransfert/provider/HeatTransfertProvider.h"

#include <assert.h>
#include "MathTools.h"

#include "../../HeatTransfert/animable/HeatTransfert.h"
#include "../../HeatTransfert/event/ImageCustomEvent.h"

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Override		*|
 \*-------------------------------------*/

/**
 * Override
 */
Animable_I<uchar4>* HeatTransfertProvider::createAnimable()
    {
    // Animation;
    int nMin = 20;
    int nMax = 120;

    // Dimension
    int w = 800;
    int h = 800;
    auto ht=new HeatTransfert(w, h, nMin, nMax);
    ht->setParallelPatern(ParallelPatern::OMP_ENTRELACEMENT);
    return ht;
    }

/**
 * Override
 */
Image_I* HeatTransfertProvider::createImageGL(void)
    {
    ColorRGB_01 colorTexte(0, 1, 0); // Green
    return new ImageCustomEvent(createAnimable(), colorTexte);
    }

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
