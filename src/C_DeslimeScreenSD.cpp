//======================================================================
// C_DeslimeScreen.cpp 
// Author: James McCormick
// Description:
//	A block that represents a deslime screen
//
//======================================================================

#include "C_DeslimeScreenSD.h"

// The size distribution has been updated
void C_DeslimeScreenSD::OnNewSizeDistribution()
{
	d_Ports.Reset();
}

// Is called to pass the parameters to the block
void C_DeslimeScreenSD::OnParameters(BlockParamsPtr p)
{
	// If the process id of the parameters does not match the id for this
	// block, then exit
	if(p->d_ProcessID != d_ProccessID)
		return;

	C_DeslimeScreenSDParams* castParams = static_cast<C_DeslimeScreenSDParams*>((I_FSBlockParameters*)p);
	
	d_DeckCutPoints[0] = castParams->d_fCutPoint;
	d_SMPerDeck[0] = castParams->d_fDeckSM;
	d_AddWater = castParams->d_fWashWater;
}


void C_DeslimeScreenSD::UpdateWater()
{
	(d_Ports.GetFlowData(2))->CalculateFluidsBasedOnSurfaceMoisture(d_SMPerDeck[0]);
	(d_Ports.GetFlowData(1))->d_FluidRate = (d_AddWater + (d_Ports.GetFlowData(0))->d_FluidRate) - (d_Ports.GetFlowData(2))->d_FluidRate;
	(d_Ports.GetFlowData(1))->RoundWater();
}


void C_DeslimeScreenSD::UpdateSolids()
{
	ScreenTheFeed(1);
}