//======================================================================
// I_GravitySep.h 
// Author: James McCormick
// Description:
//	Interface for a block that performs a gravity seperation
//======================================================================

#ifndef _GRAVITYSEP_
#define _GRAVITYSEP_

#include "I_FSBlock.h"

class I_GravitySep : public I_FSBlock
{
protected:

	// PROTECTED DATA MEMBERS==================================================
	float d_fYield;
	float d_fEp;

	// PROTECTED METHODS=======================================================
	void GravitySeperation();

public:

	// PUBLIC DATA MEMBERS=====================================================

	// PUBLIC METHODS==========================================================

	// Constructor
	I_GravitySep(FSParamsPtr fsp, const BlockID& ID) : I_FSBlock(fsp, ID), d_fEp(0.0f), d_fYield(0.0f) 
	{}

	~I_GravitySep()
	{}

	// The size distribution has been updated
	virtual void OnNewSizeDistribution() = 0;

	// Called by the flowsheet to have the block update itself.
	// The flowsheet will update the flowdata for the block before calling update.
	//virtual void OnUpdate() = 0;

	// Is called to pass the parameters to the block
	virtual void OnParameters(BlockParamsPtr) = 0;
};


#endif // _GRAVITYSEP_
