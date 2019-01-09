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
	 * used in reductionIntraBlock
	 */
	template <typename T>
	static __device__ void ecrasement(T* tabSM, int middle)
	    {
	    // TODO
	    // Ecrasement sucessifs dans une boucle
	    // __synctrhreads();// pour touts les threads d'un meme block, necessaires? ou?

	    // Rappel : |ThreadByBlock|=|tabSM|
	    // const int LOCAL_TID = Indice2D::get

	    }

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

	    //a 64 on ne divise plus et on a besoin de 32 thread pour finir de reduire le 64 premi?res cases
	    while (midle >= 64)
		{

		if (tidLocal < midle)
		    {
		    tabSM[tidLocal] += tabSM[tidLocal + midle];
		    }

		__syncthreads();

		//midle /= 2;
		midle>>=1;
		}

	    // midlde >=32. Dans le cas 32, la semantique est dans les 64 premières cases.

	    // Utilisation des 32 thread d'un warp pour finir la reduction des 64 premières cases
	    if(tidLocal<32)// semantique dans les 64 premieres cases
		{
		// no __syncthreads() necessary after each of the following lines as long as  we acces the data via a pointer decalred as volatile
		// because the 32 therad in each warp execute in a locked-step with each other
		volatile T* ptrData=tabSM;

		ptrData[tidLocal]+=ptrData[tidLocal+32];//  each thread of the warp execute this line in the same time. Aucun thread ne peut prendre de l'avance! A la fine de cette ligne, semantique dans les 32 premieres cases

		//if(tidLocal<16)		// pas necessaire car lecture(#) de ptrData[16] avant écriture(##) dans ptrData[16] ecriture ptrData[0]+=ptrData[16] (action #)  et ptrData[16]+=ptrData[32] (action ##)
		ptrData[tidLocal]+=ptrData[tidLocal+16];//  Apres cette ligne semantique dans les 16 premières cases. Seuls les 16 premiers threads sont utiles

		//if(tidLocal<8)
		ptrData[tidLocal]+=ptrData[tidLocal+8];//  Apres cette ligne semantique dans les 8 premières cases. Seuls les 8 premiers threads sont utiles

		//if(tidLocal<4)
		ptrData[tidLocal]+=ptrData[tidLocal+4];// ...

		//if(tidLocal<2)
		ptrData[tidLocal]+=ptrData[tidLocal+2];

		//if(tidLocal<1)
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
