#include "SimpleKeyListener.h"

#include <iostream>

using std::cout;
using std::endl;

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

/*-----------------------*\
|*	Constructor  	*|
 \*----------------------*/

SimpleKeyListener::SimpleKeyListener(HeatTransfert* heatTransfert)
    {
    this->heatTransfert = heatTransfert;
    }

SimpleKeyListener::~SimpleKeyListener()
    {
    //rien
    }

/*-----------------------*\
|*	Methode  	*|
 \*----------------------*/

void SimpleKeyListener::onKeyPressed(const KeyEvent &event)
    {
    if (event.isSpecial())
	{
	cout << "Special Key Pressed " << event.getSpecialKey() << endl;
	}
    else
	{
	cout << "KeyPressed = " << event.getKey() << endl;
	}

    }

void SimpleKeyListener::onKeyReleased(const KeyEvent& event)
    {
    if (event.isSpecial())
	{
	cout << "Special Key Released " << event.getSpecialKey() << endl;
	}
    else
	{
	cout << "KeyReleased = " << event.getKey() << endl;

	switch (event.getKey())
	    {
	    case 'r':
		heatTransfert->setRubber();
		break;
	    case 'h':
		heatTransfert->setHeater();
		break;
	    case 'c':
		heatTransfert->setCooler();
		break;
	    case 'p':
		heatTransfert->togglePersistant();
		break;
	    default:
		break;
	    }

	}
    }

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
