#pragma once

#include "cudaTools.h"
#include "Grid.h"
#include <curand_kernel.h>

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

class MontecarloMultiGPU
    {
    public:

	MontecarloMultiGPU(int n, const Grid& grid);
	~MontecarloMultiGPU();
	float getPi();
	void run();

    private:

	// Inputs
	int n;
	Grid grid;

	// Outputs
	float pi;
    };

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
