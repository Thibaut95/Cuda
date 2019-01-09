#include <iostream>
#include <stdlib.h>

using std::cout;
using std::endl;

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Imported	 	*|
 \*-------------------------------------*/

extern bool helloCuda(void);
extern bool isAddScalarGPU_proceduraleOk(void);
extern bool isAddScalarGPU_ObjetOk(void);
extern bool isAddVecteurOk(void);
extern bool isSliceNaifOK();
extern bool useMontecarlo(void);
extern bool useMontecarloMutliGPU(void);
extern bool isSliceOK();

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

int mainCore();

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Public			*|
 \*-------------------------------------*/

int mainCore()
    {
    bool isOk = true;
    isOk &= helloCuda();
    isOk &= isAddScalarGPU_proceduraleOk();
    isOk &= isAddScalarGPU_ObjetOk();
    isOk &= isAddVecteurOk();
    isOk &= isSliceNaifOK();
    isOk &= useMontecarlo();
    isOk &= useMontecarloMutliGPU();
    isOk &= isSliceOK();

    cout << "\nisOK = " << isOk << endl;
    cout << "\nEnd : mainCore" << endl;

    return isOk ? EXIT_SUCCESS : EXIT_FAILURE;
    }

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

