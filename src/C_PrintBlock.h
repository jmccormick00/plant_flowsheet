//======================================================================
// C_PrintBlock.h 
// Author: James McCormick
// Description:
//	A print block.  Prints the flow information that is fed to it.
//======================================================================

#ifndef _PRINTBLOCK_
#define _PRINTBLOCK_

#include "I_FSBlock.h"
#include <iostream>

class C_PrintBlock : public I_FSBlock
{
protected:
	
	// PROTECTED DATA MEMBERS==================================================

	// PROTECTED METHODS=======================================================
	C_PrintBlock();
	C_PrintBlock(const C_PrintBlock&);

	void UpdateSolids() {}
	void UpdateWater() {}

public:

	C_PrintBlock(FSParamsPtr fsp, const BlockID& ID);

	// Gets the flowdata from a specific port
	virtual C_FlowData* GetFlowData(const PortNo& pn) { return d_Ports.GetFlowData(pn); }

	// The size distribution has been updated
	void OnNewSizeDistribution();

	// Called by the flowsheet to have the block update itself.
	// The flowsheet will update the flowdata for the block before calling update.
	void OnUpdate() 
	{ 
		std::cout << "\n********************\nBlockID: " << d_BlockID << std::endl; 
		(d_Ports.GetFlowData(0))->PrintFlowData(); 
	}

	// Is called to pass the parameters to the block
	void OnParameters(BlockParamsPtr p) {}
	
};


//-----------------------------------------------------------------------
// Constructor - Public C_PrintBlock
// Description 
//	Initializes the print block.
// 
// Arguments:	blockID - The id for this block.
//				*params - A pointer to the flowsheet parameters.
// Returns:		None.
//-----------------------------------------------------------------------
inline C_PrintBlock::C_PrintBlock(FSParamsPtr fsp, const BlockID& ID) : I_FSBlock(fsp, ID)
{
	d_ProccessID = PROCID_PRINTBLOCK;
	d_Ports.Init(fsp, 1);
}


//-----------------------------------------------------------------------
// OnNewSizeDistribution - Public C_PrintBlock
// Description 
//	Updates the FlowData for this block based on the size distribution.
// 
// Arguments:	None.
// Returns:		None.
//-----------------------------------------------------------------------
inline void C_PrintBlock::OnNewSizeDistribution()
{
	d_Ports.Reset();
}

#endif // _PRINTBLOCK_
