#pragma once
#include "Indice2D.h"

/*----------------------------------------------------------------------*\
 |*			Implementation 					*|
 \*---------------------------------------------------------------------*/

class ReductionAddTools
    {
    public:

	/**
	 * Hypothese:
	 *
	 * 	(H1) 	On suppose que T est un type simple sur lequel atomicAdd existe
	 *
	 * Exemple :
	 * 		ReductionAddTools.add<int>(tabSm,ptrDevResultatGM);
	 *
	 * Contraintes :
	 *
	 * 	(C1) 	|tabSM| puissance de 2, comme 32,64,128,256,512,1024
	 * 	(C2)	|ThreadByBlock|=|tabSM|
	 * 	(C3)	Reduction intra-thread laisser a l'utilsiateur
	 *
	 * Warning :
	 *
	 * 	(W1)	ptrDevResultatGM n'est pas un tableau
	 * 	(W2)	Oubliez pas le MM pour  ptrDevResultatGM
	 * 	(W3)	Oubliez pas l'initialisation de ptrDevResultatGM ( a 0 pour de l'addiction)
	 */
	template <typename T>
	static __device__ void reductionADD(T* tabSM, T* ptrDevResultatGM)
	    {
	    // TODO reductionIntraBlock puis reductionInterblock

	    // __synctrhreads();// pour touts les threads d'un meme block, necessaires? ou?

	    reductionIntraBlock(tabSM);

	    __syncthreads();

	    reductionInterblock(tabSM, ptrDevResultatGM);
	    }

    private:

	/*--------------------------------------*\
	|*	reductionIntraBlock		*|
	 \*-------------------------------------*/

	/**
	 * Sur place, le resultat est dans tabSM[0]
	 */
	template <typename T>
	static __device__ void reductionIntraBlock(T* tabSM)
	    {
	    // TODO
	    // Ecrasement sucessifs dans une boucle (utiliser methode ecrasement ci-dessus)
	    // __synctrhreads();// pour touts les threads d'un meme block, necessaires? ou?
	    int midle = blockDim.x / 2;
	    int tidLocal = threadIdx.x;


	    while (midle >= 64)
		{
		if (tidLocal < midle)
		    {
		    tabSM[tidLocal] += tabSM[tidLocal + midle];
		    }

		__syncthreads();

		midle>>=1;
		}


	    if(tidLocal<32)
		{
		volatile T* ptrData=tabSM;

		ptrData[tidLocal]+=ptrData[tidLocal+32];
		ptrData[tidLocal]+=ptrData[tidLocal+16];
		ptrData[tidLocal]+=ptrData[tidLocal+8];
		ptrData[tidLocal]+=ptrData[tidLocal+4];
		ptrData[tidLocal]+=ptrData[tidLocal+2];
		ptrData[tidLocal]+=ptrData[tidLocal+1];
		}
	    }

	/*--------------------------------------*\
	|*	reductionInterblock		*|
	 \*-------------------------------------*/

	template <typename T>
	static __device__ void reductionInterblock(T* tabSM, T* ptrDevResultatGM)
	    {
	    // TODO
	    // utiliser atomicAdd(pointeurDestination, valeurSource);
	    // warning : atomicAdd existe pas forcement pour tous les types simples!!
	    if (threadIdx.x == 0)
		{
		atomicAdd(ptrDevResultatGM, tabSM[0]);
		}

	    }
    };
	/*----------------------------------------------------------------------*\
|*			End	 					*|
	 \*---------------------------------------------------------------------*/
