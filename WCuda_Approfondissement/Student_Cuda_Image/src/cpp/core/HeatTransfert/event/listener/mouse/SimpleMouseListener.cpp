#include "SimpleMouseListener.h"

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

SimpleMouseListener::SimpleMouseListener(HeatTransfert* heatTransfert)
    {
    this->heatTransfert=heatTransfert;
    }

SimpleMouseListener::~SimpleMouseListener()
    {
    //rien
    }

/*-----------------------*\
|*	Methode  	*|
 \*----------------------*/

void SimpleMouseListener::onMouseMoved(const MouseEvent& event)
    {
    cout<<"[MouseMoved] : ";
    printXY(event);
    heatTransfert->createHeater(event.getX(), event.getY());
    }

void SimpleMouseListener::onMousePressed(const MouseEvent& event)
    {
    cout<<"[MousePressed]  :  button="<<event.getButtonType()<< " : ";
    printXY(event);
    }

void SimpleMouseListener::onMouseReleased(const MouseEvent& event)
    {
    cout<<"[MouseReleased] :  button="<<event.getButtonType()<< " : ";
    printXY(event);
    }

void SimpleMouseListener::onMouseWheel(const MouseWheelEvent& event)
    {
    cout<<"[MouseWheel]    :  direction="<<event.getDirection()<< " : ";
    printXY(event);
    heatTransfert->updateSizePencil(event.getDirection());
    }

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/**
 * (x,y) : coordonn�e image, et non frame!
 */
void SimpleMouseListener::printXY(const MouseEvent& event)
    {
    cout<<" at pixel (x,y) = ("<<event.getX()<<","<<event.getY()<<")"<<endl;
    }

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

