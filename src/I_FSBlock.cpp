//======================================================================
// FSBlock.cpp
// Author: James McCormick
// Description:
//	The class for the flowsheet blocks.  Provides basic properties, 
//	methods, and interface that every block must have.
//======================================================================


#include "I_FSBlock.h"


void I_FSBlock::OnUpdate()
{
	if(d_fspFSParams->d_bUpdateSolids)
		UpdateSolids();
	
	if(d_fspFSParams->d_bUpdateWater)
	{
		UpdateWater();
		d_Ports.UpdatePercentSolids();
	}
}
