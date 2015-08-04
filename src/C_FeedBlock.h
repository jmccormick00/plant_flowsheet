//======================================================================
// C_FeedBlock.h 
// Author: James McCormick
// Description:
//	A feed block object.  Initializes the flow data for the flowsheet.
//======================================================================

#ifndef _FEEDBLOCK_
#define _FEEDBLOCK_

#include "I_FSBlock.h"

class C_FeedBlockParams : public I_FSBlockParameters
{
private:
	C_FeedBlockParams();
	C_FeedBlockParams(C_FeedBlockParams&);

public:
	C_FeedBlockParams(BlockID id, ProcessID p, float feedRate, float surfaceMoisture) : I_FSBlockParameters(id, p),
		d_feedRate(feedRate), d_surfaceMoisture(surfaceMoisture)
	{}

	float d_feedRate;
	float d_surfaceMoisture;
};


class C_FeedBlock : public I_FSBlock
{
protected:

	// The total amount for the feed
	float d_fFeedSolidRate;	

	// Stores the incoming surface moisture.
	float d_fFeedSurfaceMoisture;

	// Has the feed been defined
	bool d_bUpdatedFeed;

	// Update the feed flowdata before updating
	//virtual void UpdateFeedData();

	virtual void UpdateSolids();
	virtual void UpdateWater();

public:
	// Constructor / Destructor
	C_FeedBlock(FSParamsPtr fsp, const BlockID& ID);
	~C_FeedBlock() { }

	// Distribute the solids if needed
	/*virtual void OnUpdate(const C_FlowData& fd) 
	{ 
		if(!d_bUpdatedFeed)
			this->UpdateFeedData(); 
	}
	*/
	// Take the parameters and set them
	virtual void OnParameters(BlockParamsPtr p); 

	// The size distribution has been updated
	virtual void OnNewSizeDistribution();
};

//-----------------------------------------------------------------------
// Constructor - Public C_FeedBlock
// Description 
//	Initializes the feed block.
// 
// Arguments:	blockID - The id for this block.
//				*params - A pointer to the flowsheet parameters.
// Returns:		None.
//-----------------------------------------------------------------------
inline C_FeedBlock::C_FeedBlock(FSParamsPtr fsp, const BlockID& ID) : I_FSBlock(fsp, ID)
{
	d_ProccessID = PROCID_FEED;
	d_Ports.Init(fsp, 1); 
	d_bUpdatedFeed = false;
}

//-----------------------------------------------------------------------
// UpdateSolids - Public C_FeedBlock
// Description 
//	Calculates the feed data.
// 
// Arguments:	None.
// Returns:		None.
//-----------------------------------------------------------------------
inline void C_FeedBlock::UpdateSolids()
{
	if(!d_bUpdatedFeed)
	{
		float temp = d_fspFSParams->d_sdSizeDistribution.GetTopSize(); 
		(d_Ports.GetFlowData())->DistributeSolids(temp, 0, d_fFeedSolidRate);
		d_bUpdatedFeed = true;
	}
}

inline void C_FeedBlock::UpdateWater()
{
	(d_Ports.GetFlowData())->CalculateFluidsBasedOnSurfaceMoisture(d_fFeedSurfaceMoisture);
}


//-----------------------------------------------------------------------
// OnParameters - Public C_FeedBlock
// Description 
//	Takes the parameters passed to it and updates the class members.
// 
// Arguments:	p - the parameters.
// Returns:		None.
//-----------------------------------------------------------------------
inline void C_FeedBlock::OnParameters(BlockParamsPtr p)
{
	// If the process id of the parameters does not match the id for this
	// block, then exit
	if(p->d_ProcessID != d_ProccessID)
		return;

	C_FeedBlockParams* castParams = static_cast<C_FeedBlockParams*>((I_FSBlockParameters*)p);

	d_fFeedSolidRate = castParams->d_feedRate;
	d_fFeedSurfaceMoisture = castParams->d_surfaceMoisture;
	d_bUpdatedFeed = false;
}


//-----------------------------------------------------------------------
// OnNewSizeDistribution - Public C_FeedBlock
// Description 
//	Updates the FlowData for this block based on the size distribution.
// 
// Arguments:	None.
// Returns:		None.
//-----------------------------------------------------------------------
inline void C_FeedBlock::OnNewSizeDistribution()
{
	d_bUpdatedFeed = false;
	d_Ports.Reset();
}


#endif // _FEEDBLOCK_