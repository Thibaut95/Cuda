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

class Montecarlo
    {
    public:

	Montecarlo(int n, const Grid& grid);
	~Montecarlo();
	float getPi();
	int getN0();
	void run();

    private:

	// Inputs
	int n;
	Grid grid;

	// Outputs
	float pi;
	int n0;

	// Tools
	int* ptrResultGM;
	curandState* tabDevGeneratorGM;
	size_t sizeTabGenerator;
    };

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/
