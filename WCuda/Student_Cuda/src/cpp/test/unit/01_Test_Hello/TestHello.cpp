#include "TestHello.h"

#include "Device.h"

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Imported	 	*|
 \*-------------------------------------*/

extern bool helloCuda(void);

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Constructor		*|
 \*-------------------------------------*/

TestHello::TestHello()
    {
    TEST_ADD(TestHello::hello);
    }

/*--------------------------------------*\
 |*		Methodes		*|
 \*-------------------------------------*/

void TestHello::hello(void)
    {
    TEST_ASSERT(helloCuda() == true);
    }



/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

