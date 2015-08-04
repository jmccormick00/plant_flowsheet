//======================================================================
// C_DeslimeScreenDD.cpp 
// Author: James McCormick
// Description:
//	A block that represents a deslime screen
//
//======================================================================

#include "C_DeslimeScreenDD.h"

// The size distribution has been updated
void C_DeslimeScreenDD::OnNewSizeDistribution()
{
	d_Ports.Reset();
}

// Is called to pass the parameters to the block
void C_DeslimeScreenDD::OnParameters(BlockParamsPtr p)
{
	// If the process id of the parameters does not match the id for this
	// block, then exit
	if(p->d_ProcessID != d_ProccessID)
		return;

	C_DeslimeScreenDDParams* castParams = static_cast<C_DeslimeScreenDDParams*>((I_FSBlockParameters*)p);
	
	d_DeckCutPoints[0] = castParams->d_fCutPoint[0];
	d_DeckCutPoints[1] = castParams->d_fCutPoint[1];
	d_SMPerDeck[0] = castParams->d_fDeckSM[0];
	d_SMPerDeck[1] = castParams->d_fDeckSM[1];
	d_AddWater = castParams->d_fWashWater;
}


void C_DeslimeScreenDD::UpdateWater()
{
	(d_Ports.GetFlowData(2))->CalculateFluidsBasedOnSurfaceMoisture(d_SMPerDeck[0]);
	(d_Ports.GetFlowData(3))->CalculateFluidsBasedOnSurfaceMoisture(d_SMPerDeck[1]);
	(d_Ports.GetFlowData(1))->d_FluidRate = (d_AddWater + (d_Ports.GetFlowData(0))->d_FluidRate) - (d_Ports.GetFlowData(2))->d_FluidRate - (d_Ports.GetFlowData(3))->d_FluidRate;
	(d_Ports.GetFlowData(1))->RoundWater();
}


void C_DeslimeScreenDD::UpdateSolids()
{
	ScreenTheFeed(1);
}