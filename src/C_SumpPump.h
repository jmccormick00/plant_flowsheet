//======================================================================
// C_SumpPump.h 
// Author: James McCormick
// Description:
//	A block that represents the sump and pump
//	The Sump pump block will have only one port, the feed.  The block
//	that gets fed from the pump will refer to the feed.
//======================================================================

#ifndef _SUMPPUMP_
#define _SUMPPUMP_

#include "I_FSBlock.h"


class C_SumpPumpParams : public I_FSBlockParameters
{
private:
	C_SumpPumpParams();
	C_SumpPumpParams(C_SumpPumpParams&);

public:
	C_SumpPumpParams(BlockID id, ProcessID p, float addWater) : I_FSBlockParameters(id, p), 
		d_fAddWater(addWater)
	{}

	float d_fAddWater;

};


class C_SumpPump : public I_FSBlock
{
protected:

	// PROTECTED DATA MEMBERS==================================================


	// PROTECTED METHODS=======================================================

	virtual void UpdateWater()
	{
		d_Ports.GetFlowData(0)->d_FluidRate += d_AddWater;
	}

	virtual void UpdateSolids() {}

public:

	// PUBLIC DATA MEMBERS=====================================================

	// PUBLIC METHODS==========================================================

	// Constructor
	C_SumpPump(FSParamsPtr fsp, const BlockID& ID) : I_FSBlock(fsp, ID) 
	{
		d_ProccessID = PROCID_SUMPPUMP;
		d_Ports.Init(fsp, 1); 
	}

	// The size distribution has been updated
	virtual void OnNewSizeDistribution()
	{
		d_Ports.Reset();
	}

	// Called by the flowsheet to have the block update itself.
	// The flowsheet will update the flowdata for the block before calling update.
	//virtual void OnUpdate();

	// Is called to pass the parameters to the block
	virtual void OnParameters(BlockParamsPtr p)
	{
		// If the process id of the parameters does not match the id for this
		// block, then exit
		if(p->d_ProcessID != d_ProccessID)
			return;

		C_SumpPumpParams* castParams = static_cast<C_SumpPumpParams*>((I_FSBlockParameters*)p);

		d_AddWater = castParams->d_fAddWater;
	}
};

#endif // _SUMPPUMP_