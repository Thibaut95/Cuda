#include "Slice.h"

#include <iostream>
#include <assert.h>
#include "MathTools.h"

#include "Device.h"

using std::cout;
using std::endl;

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Imported	 	*|
 \*-------------------------------------*/

__global__ void sliceDevice(int n, float* ptrResultGM);

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Constructeur			*|
 \*-------------------------------------*/

Slice::Slice(const Grid& grid, int nbSlice, float* ptrPiHat) :
	ptrPiHat(ptrPiHat), nbSlice(nbSlice)
    {
    this->sizeGM = sizeof(float); // [octet]

    // MM
	{
	// TODO SliceNaif
	ptrResultGM=NULL;
	Device::malloc(&ptrResultGM,sizeGM);
	Device::memclear(ptrResultGM,sizeGM);
	}

    // Grid
	{
	this->dg = grid.dg;
	this->db = grid.db;
	}
    }

Slice::~Slice(void)
    {
    //MM (device free)
	{
	// TODO SliceNaif
	Device::free(ptrResultGM);
	}
    }

/*--------------------------------------*\
 |*		Methode			*|
 \*-------------------------------------*/

void Slice::run()
    {
    sliceDevice<<<dg,db, 1024*sizeof(float)>>>(nbSlice, ptrResultGM);// assynchrone
    Device::memcpyDToH(ptrPiHat, ptrResultGM, sizeof(float));
    }

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
