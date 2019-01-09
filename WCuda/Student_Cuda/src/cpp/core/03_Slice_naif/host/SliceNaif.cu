#include "SliceNaif.h"

#include <iostream>
#include <assert.h>

#include "Device.h"

using std::cout;
using std::endl;

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Imported	 	*|
 \*-------------------------------------*/

extern __global__ void reductionIntraThread(float* tabGM,int nbSlice);
extern __global__ void ecrasementGM(float* tabGM, int moitier);

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Constructeur			*|
 \*-------------------------------------*/

SliceNaif::SliceNaif(const Grid& grid, int nbSlice, float* ptrPiHat) :
	ptrPiHat(ptrPiHat), nbSlice(nbSlice)
    {
    this->nTabGM = grid.threadCounts();
    this->sizeTabGM = nTabGM * sizeof(float); // [octet]

    // MM
	{
	// TODO SliceNaif
	Device::malloc(&tabGM, sizeTabGM);
	}

    // Grid
	{
	this->dg = grid.dg;
	this->db = grid.db;
	}
    }

SliceNaif::~SliceNaif(void)
    {
    //MM (device free)
	{
	// TODO SliceNaif
	Device::free(tabGM);
	}
    }

/*--------------------------------------*\
 |*		Methode			*|
 \*-------------------------------------*/

void SliceNaif::run()
    {
//    // TODO SliceNaif
//    reductionIntraThread<<<dg,db>>>(tabGM, nbSlice);
//
//    float* tabHost = new float[nTabGM];
//    Device::memcpyDToH(tabHost, tabGM, sizeTabGM);
//
//    float sum = 0.0;
//#pragma omp parallel for reduction(+:sum)
//    for (int i = 0; i < nTabGM; i++)
//	{
//	sum += tabHost[i];
//	}
//
//    *ptrPiHat = sum;

    reductionIntraThread<<<dg,db>>>(tabGM, nbSlice);// assynchrone
    int moitier = nTabGM / 2;
    while (moitier >= 1)
	{
	ecrasementGM<<<dg,db>>>(tabGM, moitier); //barriere synchronisation implicite
	moitier = moitier / 2;
	}
    Device::memcpyDToH(ptrPiHat, tabGM, sizeof(float));
    }

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
