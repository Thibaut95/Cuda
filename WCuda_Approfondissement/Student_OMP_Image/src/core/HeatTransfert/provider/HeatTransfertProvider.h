#pragma once

#include "Provider_I_CPU.h"

using namespace cpu;

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/



class HeatTransfertProvider: public Provider_I<uchar4>
    {
    public:

	virtual ~HeatTransfertProvider()
	    {
	    // Rien
	    }

	/*--------------------------------------*\
	 |*		Override		*|
	 \*-------------------------------------*/

	virtual Animable_I<uchar4>* createAnimable(void);

	virtual Image_I* createImageGL(void);

    };

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

