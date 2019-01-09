#include <iostream>

#include "Raytracing.h"
#include "Device.h"
#include "SphereCreator.h"
#include <assert.h>

using std::cout;
using std::endl;

/*----------------------------------------------------------------------*\
 |*			Declaration 					*|
 \*---------------------------------------------------------------------*/


/*--------------------------------------*\
 |*		Imported	 	*|
 \*-------------------------------------*/

__global__ void kernelRaytacingGM(uchar4* ptrDevPixels, uint w, uint h, float t, Sphere* ptrTabSpheresGM, int nbSpheres);
__global__ void kernelRaytacingSM(uchar4* ptrDevPixels, uint w, uint h, float t, Sphere* ptrTabSpheresGM, int nbSpheres);
__global__ void kernelRaytacingCM(uchar4* ptrDevPixels, uint w, uint h, float t, int nbSpheres);

__host__ void uploadToCM(Sphere* ptrTabSpheres, int nbSpheres);

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

/*--------------------------------------*\
 |*		Constructeur		*|
 \*-------------------------------------*/

Raytracing::Raytracing(const Grid& grid, uint w, uint h, float dt, int nbSpheres,const MemoryType& memoryType) :
	Animable_I<uchar4>(grid, w, h, "Raytracing_Cuda_RGBA_uchar4"), memoryType(memoryType)
    {
    // Inputs
    this->dt = dt;
    this->nbSpheres = nbSpheres;

    // Tools
    this->sizeSpheres = nbSpheres * sizeof(Sphere);
    this->t = 0;
    Device::malloc(&ptrDevTabSpheres, sizeSpheres);

    // spheres
	{
	SphereCreator sphereCreator(nbSpheres, w, h);
	uploadToDevice(sphereCreator.getTabSphere());
	} // SphereCreator depiler, donc detruit, tabSphere cote host detruit!

    cout << "[Raytracing] : memoryType : " << memoryType;
    }

Raytracing::~Raytracing()
    {
    // TODO Raytracing GPU MemoryManagement free
    switch (memoryType)
	{
	case GM:
	    {
	    Device::free(ptrDevTabSpheres);
	    // ??
	    break;
	    }
	case CM:
	    {
	    assert(false);// to delete once implement
	    // ??
	    break;
	    }
	case SM:
	    {
	    Device::free(ptrDevTabSpheres);
	    // ??
	    break;
	    }
	}
    }

/*--------------------------------------*\
 |*		Methode			*|
 \*-------------------------------------*/

/**
 * Override
 */
void Raytracing::process(uchar4* ptrDevPixels, uint w, uint h, const DomaineMath& domaineMath)
    {
    // TODO Raytracing GPU lauch kernel

    switch (memoryType)
	{
	case GM:
	    {
	    kernelRaytacingGM<<<dg,db>>>(ptrDevPixels,w,h,t,ptrDevTabSpheres,nbSpheres);
	    // Call the kernel kernelRaytacingGM (line 25)
	    break;
	    }
	case CM:
	    {
	    kernelRaytacingCM<<<dg,db>>>(ptrDevPixels,w,h,t,nbSpheres);
	    // Call the kernel kernelRaytacingCM (line 26)
	    break;
	    }
	case SM:
	    {

	    kernelRaytacingSM<<<dg,db,sizeSpheres>>>(ptrDevPixels,w,h,t,ptrDevTabSpheres,nbSpheres);
	    // Call the kernel kernelRaytacingSM (line 27)
	    break;
	    }
	}
    }

/**
 * Override
 */
void Raytracing::animationStep()
    {
    t += dt;
    }

/*--------------------------------------*\
 |*		Private			*|
 \*-------------------------------------*/

void Raytracing::uploadToDevice(Sphere* ptrTabSpheres)
    {
    // TODO Raytracing GPU MemoryManagement
    switch (memoryType)
	{
	case GM:
	    {
	    Device::memcpyHToD(ptrDevTabSpheres, ptrTabSpheres, sizeSpheres);
	    break;
	    }
	case CM:
	    {
	    uploadToCM(ptrTabSpheres, nbSpheres);
	    break;
	    }
	case SM:
	    {
	    Device::memcpyHToD(ptrDevTabSpheres, ptrTabSpheres, sizeSpheres);
	    break;
	    }
	}
    }

/*----------------------------------------------------------------------*\
 |*			End	 					*|
 \*---------------------------------------------------------------------*/

