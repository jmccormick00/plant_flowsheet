//======================================================================
// Flowsheet.cpp
// Author: James McCormick
// Description:
//	A class that manages all of the blocks.  Creates, deletes, and
//	stores the blocks in memory.  Stores the flowsheet structure.
//======================================================================

#include "C_Flowsheet.h"


//-----------------------------------------------------------------------
// RemoveFromSources - Private C_Flowsheet
// Description 
//	Loop through each block in the current block's destination list 
//	and remove it from their source list.
// 
// Arguments:	DestList - the destination list for the removed block
//				removedID - the ID for the block to be removed
// Returns:		None.
//-----------------------------------------------------------------------
void C_Flowsheet::RemoveFromSources(BlockIDList& DestList, const BlockID& removedID)
{
	BlockIDListIterator itr = DestList.begin();
	BlockIDListIterator end = DestList.end();

	// Loop through the destination list
	while(itr != end)
	{
		FeedSourceList& feedList = d_SourceMap[(*itr)];
		FeedSourceListIterator feedItr = feedList.begin();
		FeedSourceListIterator feedEnd = feedList.end();

		while(feedItr != feedEnd)
		{
			if(feedItr->blkPointer->GetBlockID() == removedID)
				feedItr = feedList.erase(feedItr);
			else
				feedItr++;
		}
		itr++;
	}
}


//-----------------------------------------------------------------------
// RemoveFromDest - Private C_Flowsheet
// Description 
//	Loop through each block in the source list and have it remove the ID
//	from their destination list.
// 
// Arguments:	SourceList - the source list for a the removed block
//				removedID - the ID of the block to be removed
// Returns:		None.
//-----------------------------------------------------------------------
void C_Flowsheet::RemoveFromDest(FeedSourceList& SourceList, const BlockID& removedID)
{
	FeedSourceListIterator feedItr = SourceList.begin();
	FeedSourceListIterator feedEnd = SourceList.end();

	// Loop through the feed sources
	while(feedItr != feedEnd)
	{
		d_DestMap[feedItr->blkPointer->GetBlockID()].remove(removedID);
		feedItr++;
	}
}


//-----------------------------------------------------------------------
// DeleteSouceLink - Private C_Flowsheet
// Description 
//	Removes a specific entry in a blocks source list.
// 
// Arguments:	from - The ID of the block that supplies the feed.
//				to - The ID of the block that receives the feed.
//				port - The port number on the from block that the 
//					   feed comes from.
// Returns:		None.
//-----------------------------------------------------------------------
void C_Flowsheet::DeleteSourceLink(const BlockID& from, const BlockID& to, const PortNo& port)
{
	FeedSourceList& feedList = d_SourceMap[to];
	FeedSourceListIterator feedItr = feedList.begin();
	FeedSourceListIterator feedEnd = feedList.end();

		// Loop through the feed sources, find the entry, and delete it
	while(feedItr != feedEnd)
	{
		if( (feedItr->blkPointer->GetBlockID() == from) && (feedItr->usPort == port) )
			feedItr = feedList.erase(feedItr);
		else
			feedItr++;
	}
}


//-----------------------------------------------------------------------
// SumSources - Private C_Flowsheet
// Description 
//	Sums the sources data for a block.
// 
// Arguments:	feedList - The source list for a block
//				dest - A pointer to the FlowData to store the sum
// Returns:		none
//-----------------------------------------------------------------------
void C_Flowsheet::SumSources(FeedSourceList& feedList, C_FlowData* const fd)
{
	FeedSourceListIterator feedItr = feedList.begin();
	FeedSourceListIterator feedItrEnd = feedList.end();

	if(d_fspFSParams->d_bUpdateSolids)
		fd->ZeroSolids();
	if(d_fspFSParams->d_bUpdateWater)
		fd->ZeroWater();

	while(feedItr != feedItrEnd)
	{
		(*fd) += (*feedItr->blkPointer->GetFlowData(feedItr->usPort));
		feedItr++;
	}
}


//-----------------------------------------------------------------------
// LoadSizeDistribution - Public C_Flowsheet
// Description 
//	Loads a size distribution from a text file
// 
// Arguments:	name of the file to load
// Returns:		true if successful, false otherwise
//-----------------------------------------------------------------------
bool C_Flowsheet::LoadSizeDistribution(const char* fileName)
{	
	if(d_fspFSParams->d_sdSizeDistribution.LoadSizeDist(fileName))
	{
		BlockMapIterator blockItrEnd = d_BlockMap.end();
		BlockMapIterator blockItr = d_BlockMap.begin();
		while(blockItr != blockItrEnd)
		{
			blockItr->second->OnNewSizeDistribution();
			blockItr++;
		}
		return true;
	}
	else
		return false;
}

//-----------------------------------------------------------------------
// Check - private C_Flowsheet
// Description 
//	Checks to see if any value in deltaPorts is over the delta.  If just one
//	value is over, d_bDone is set to false and Check returns.
// 
// Arguments:	None.
// Returns:		None.
//-----------------------------------------------------------------------
void C_Flowsheet::Check(C_SmartPointer<C_BlockPorts> deltaPorts)
{
	C_FlowData* fd;
	for(unsigned short i = 0; i < deltaPorts->GetNumPorts(); i++)
	{
		fd = deltaPorts->GetFlowData(i);
		for(unsigned short j = 0; j < d_fspFSParams->d_sdSizeDistribution.GetNumSizeFractions(); j++)
		{
			if(((*fd)[j] < -d_fDelta) || ((*fd)[j] > d_fDelta))
			{
				d_bDone = false;
				return;
			}
		}
		if((fd->d_FluidRate < -d_fDelta) || (fd->d_FluidRate > d_fDelta))
		{
			d_bDone = false;
			return;
		}
	}
	return;
}


//-----------------------------------------------------------------------
// SolveFlowSheet - Public C_Flowsheet
// Description 
//	Updates all of the blocks until each block converges.
// 
// Arguments:	None.
// Returns:		None.
//-----------------------------------------------------------------------
bool C_Flowsheet::SolveFlowSheet()
{
	BlockMapIterator blockItrEnd = d_BlockMap.end();
	BlockMapIterator blockItr = d_BlockMap.begin();

	// Update the feed blocks first
	while(blockItr != blockItrEnd)
	{
		if(blockItr->second->GetProcessID() == PROCID_FEED)
			blockItr->second->OnUpdate();
		blockItr++;
	}

	// Set the number of iterations back to zero
	d_uiNumIterations = 0;

	do
	{
		// Assume that it is done.
		d_bDone = true;
		blockItr = d_BlockMap.begin();

		while(blockItr != blockItrEnd)
		{
			if(blockItr->second->GetProcessID() != PROCID_FEED)
			{
				// Store the previous values for this block
				C_BlockPorts before(blockItr->second->GetPorts());

				// Sum the sources then update
				SumSources(d_SourceMap[blockItr->first], blockItr->second->GetFlowData(0));
				blockItr->second->OnUpdate();
	
				// If it has not been proven that the flowsheet isnt done, then run a check
				// If just one block isn't finished, then set it to false.
				if(d_bDone)
					Check( (blockItr->second->GetPorts()).Difference(before) );
			}
			blockItr++;
		}

		d_uiNumIterations++;
	}while((!d_bDone) && (d_uiNumIterations <= d_uiMaxNumberIter));

	return d_bDone;
}
