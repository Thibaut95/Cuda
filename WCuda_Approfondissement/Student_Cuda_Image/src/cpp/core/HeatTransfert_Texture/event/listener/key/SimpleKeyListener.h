#pragma once

#include "KeyListener_I.h"
#include "HeatTransfert.h"

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

class SimpleKeyListener: public KeyListener_I
    {
    public:

	/*--------------------------------------*\
	|*		Constructor		*|
	 \*-------------------------------------*/

	SimpleKeyListener(HeatTransfert* heatTransfert);
	virtual ~SimpleKeyListener();

	/*--------------------------------------*\
	 |*		Methodes		*|
	 \*-------------------------------------*/

    public:

	/**
	 * Override
	 */
	virtual void onKeyPressed(const KeyEvent& event);

	/**
	 * Override
	 */
	virtual void onKeyReleased(const KeyEvent& event);

	/*--------------------------------------*\
	 |*		Attributs		*|
	 \*-------------------------------------*/

    private:
	HeatTransfert* heatTransfert;
    };

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
