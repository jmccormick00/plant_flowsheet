//======================================================================
// C_DeslimeScreen.h 
// Author: James McCormick
// Description:
//	A block that represents a deslime screen
//
//======================================================================

#ifndef _DESLIMESCREENSD_
#define _DESLIMESCREENSD_

#include "I_Screen.h"


class C_DeslimeScreenSDParams : public I_FSBlockParameters
{
private:
	C_DeslimeScreenSDParams();
	C_DeslimeScreenSDParams(C_DeslimeScreenSDParams&);

public:
	C_DeslimeScreenSDParams(BlockID id, ProcessID p, float deckSM, float cutPoint, float washWater) : I_FSBlockParameters(id, p), 
		d_fCutPoint(cutPoint), d_fDeckSM(deckSM), d_fWashWater(washWater)
	{}

	float d_fWashWater;
	float d_fDeckSM;
	float d_fCutPoint;
};



class C_DeslimeScreenSD : public I_Screen
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
	C_DeslimeScreenSD(FSParamsPtr fsp, const BlockID& ID) : I_Screen(fsp, ID, 1) 
	{
		d_ProccessID = PROCID_DESLIME_SINGLEDECK;
		d_Ports.Init(fsp, 3); 
	}

	// The size distribution has been updated
	virtual void OnNewSizeDistribution();

	// Called by the flowsheet to have the block update itself.
	// The flowsheet will update the flowdata for the block before calling update.
	//virtual void OnUpdate();

	// Is called to pass the parameters to the block
	virtual void OnParameters(BlockParamsPtr);
};

#endif // _DESLIMESCREENSD_