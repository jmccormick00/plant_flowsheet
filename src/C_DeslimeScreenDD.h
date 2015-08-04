//======================================================================
// C_DeslimeScreen.h 
// Author: James McCormick
// Description:
//	A block that represents a double deslime screen
//
//======================================================================

#ifndef _DESLIMESCREENDD_
#define _DESLIMESCREENDD_

#include "I_Screen.h"


class C_DeslimeScreenDDParams : public I_FSBlockParameters
{
private:
	C_DeslimeScreenDDParams();
	C_DeslimeScreenDDParams(C_DeslimeScreenDDParams&);

public:
	C_DeslimeScreenDDParams(BlockID id, ProcessID p, float deckSMTop, float deckSMBottom, float cutPointTop, float cutPointBottom, float washWater) : I_FSBlockParameters(id, p), 
	 d_fWashWater(washWater)
	{
		d_fDeckSM[0] = deckSMBottom;
		d_fDeckSM[1] = deckSMTop;
		d_fCutPoint[0] = cutPointBottom;
		d_fCutPoint[1] = cutPointTop;
	}

	float d_fWashWater;
	float d_fDeckSM[2];
	float d_fCutPoint[2];
};



class C_DeslimeScreenDD : public I_Screen
{
protected:

	// PROTECTED DATA MEMBERS==================================================


	// PROTECTED METHODS=======================================================

	virtual void UpdateWater();
	virtual void UpdateSolids();

public:

	// PUBLIC DATA MEMBERS=====================================================

	// PUBLIC METHODS==========================================================

	// Constructor
	C_DeslimeScreenDD(FSParamsPtr fsp, const BlockID& ID) : I_Screen(fsp, ID, 2) 
	{
		d_ProccessID = PROCID_DESLIME_DOUBLEDECK;
		d_Ports.Init(fsp, 4); 
	}

	// The size distribution has been updated
	virtual void OnNewSizeDistribution();

	// Called by the flowsheet to have the block update itself.
	// The flowsheet will update the flowdata for the block before calling update.
	//virtual void OnUpdate();

	// Is called to pass the parameters to the block
	virtual void OnParameters(BlockParamsPtr);
};

#endif // _DESLIMESCREENDD_