//======================================================================
// Flowsheet.h 
// Author: James McCormick
// Description:
//	A class that manages all of the blocks.  Creates, deletes, and
//	stores the blocks in memory.  Stores the flowsheet structure.
//======================================================================

#ifndef _FLOWSHEET_
#define _FLOWSHEET_

#include "C_BlockFactory.h"
#include <map>
#include <list>

class C_Flowsheet
{
private:

	// Stores the information on the block that feeds another block.
	struct S_FeedSource
	{
		BlockPtr blkPointer;
		PortNo usPort;
		S_FeedSource(BlockPtr pointer, PortNo port)
		{
			blkPointer = pointer;
			usPort = port;
		}
	};

	// Used to store a list of blocks
	typedef std::list<BlockID> BlockIDList;
	typedef std::list<BlockID>::iterator BlockIDListIterator;

	// Used to store a list of S_FeedSources
	typedef std::list<S_FeedSource> FeedSourceList;
	typedef std::list<S_FeedSource>::iterator FeedSourceListIterator;

	// Maps a BlockID to a FeedSourceList.  This tells the block with BlockID what feeds it.
	typedef std::map< BlockID, FeedSourceList > SourceMap;

	// Maps the BlockID to a block in memory.  This stores the blocks.
	typedef std::map< BlockID, C_SmartPointer<I_FSBlock> > BlockMap;
	typedef std::map< BlockID, C_SmartPointer<I_FSBlock> >::iterator BlockMapIterator;
	
	// Maps a BlockID to a list of BlockID's.  This tells the block with BlockID what it feeds.
	typedef std::map< BlockID, BlockIDList > DestMap;
	typedef std::map< BlockID, BlockIDList >::iterator DestMapIterator;
	
	// PRIVATE DATA MEMBERS====================================================

	// Used for determining if the flowsheet has converged
	bool d_bDone;

	// The maximum delta considered for convergence
	float d_fDelta;	

	// The number of iterations the SolveFlowsheet() function took
	unsigned d_uiNumIterations;

	// The maximum number of iterations
	unsigned d_uiMaxNumberIter;

	// The array for all of the blocks in the flowsheet
	BlockMap d_BlockMap;

	// Source map for each block
	SourceMap d_SourceMap;

	// Destination Map for each block
	DestMap d_DestMap;

	// The factory for creating the blocks
	C_SmartPointer<C_BlockFactory> d_BlockFactory;

	// The flowsheet Parameters
	FSParamsPtr d_fspFSParams;

	// PRIVATE METHODS=========================================================

	// For suming the sources before calling the blocks update function
	void SumSources(FeedSourceList& feedList, C_FlowData* const fd);

	// Checks to see if the block has stopped changing
	void Check(C_SmartPointer<C_BlockPorts> deltPorts);

	// For removing a block from the flowsheet
	void RemoveFromSources(BlockIDList& DestList, const BlockID& removedID);
	void RemoveFromDest(FeedSourceList& SourceList, const BlockID& removedID);
	
	// For removing a link
	void DeleteSourceLink(const BlockID& from, const BlockID& to, const PortNo& port);

public:

	// PUBLIC DATA MEMBERS=====================================================
	

	// PUBLIC METHODS==========================================================

	// Constructor/Destructor
	C_Flowsheet();
	~C_Flowsheet();

	// Wipes any FSBlocks in memory and sets everything to zero
	void Reset();

	// Solves the flowsheet - returns true if it converged, false if it hit the max number of iterations
	bool SolveFlowSheet();

	// Sets the metric bit
	void SetMetric(bool b) { d_fspFSParams->d_bMetric = b; }

	void SetUpdateSolids(bool b) { d_fspFSParams->d_bUpdateSolids = b; }
	void SetUpdateWater(bool b) { d_fspFSParams->d_bUpdateWater = b; }
	void SetRoundToWater(int r) { d_fspFSParams->d_iWaterRoundTo = r; }

	// Sets the delta for balancing the flowsheet
	void SetDelta(float d) { d_fDelta = d; }

	void SetMaxIterations(unsigned i) { d_uiMaxNumberIter = i; }

	// Creates a new block
	BlockID CreateBlock(const unsigned short& procID);

	// Removes a block from the flowsheet
	void RemoveBlock(const BlockID& id);

	// Removes the link
	void BreakLink(BlockID from, BlockID to, PortNo port);

	// Makes the first block a source for the second block
	void MakeLink(BlockID from, PortNo fromPort, BlockID to);

	// Loads the size distribution from a text file and tells the 
	// blocks that a size distribution has been updated
	bool LoadSizeDistribution(const char* fileName);

	// Prints the Size distribution to the console
	void PrintSizeDistribution() { d_fspFSParams->d_sdSizeDistribution.PrintSizeDist(); }

	// Pushes parameters to a block
	void PushParameters(BlockParamsPtr fsbParams) { d_BlockMap[fsbParams->d_BlockID]->OnParameters(fsbParams); }
};


//-----------------------------------------------------------------------
// Constructor - Public C_Flowsheet
// Description 
//	Performs Initialization.
// 
// Arguments:	None.
// Returns:		None.
//-----------------------------------------------------------------------
inline C_Flowsheet::C_Flowsheet()
{
	d_bDone = false;
	d_fspFSParams = new S_FlowSheetParams;
	d_uiNumIterations = 0;
	d_BlockFactory = new C_BlockFactory(d_fspFSParams, 100);
}


//-----------------------------------------------------------------------
// Desctructor - Public C_Flowsheet
// Description 
//	Deletes the memory used by the blocks and size distribution.
// 
// Arguments:	None.
// Returns:		None.
//-----------------------------------------------------------------------
inline C_Flowsheet::~C_Flowsheet()
{
	Reset();
	d_fspFSParams->d_sdSizeDistribution.UnloadSizeDist();
}



//-----------------------------------------------------------------------
// CreateBlock - Public C_Flowsheet
// Description 
//	Creates a block.
// 
// Arguments:	procID - the id for the proccess operation 
//						that the block represents.
// Returns:		true if successfull false otherwise
//-----------------------------------------------------------------------
inline BlockID C_Flowsheet::CreateBlock(const unsigned short &procID)
{
	C_SmartPointer<I_FSBlock> temp = d_BlockFactory->CreateBlock(procID);

	if(temp == NULL)
		return 0;

	d_BlockMap[temp->GetBlockID()] = temp;
	
	return temp->GetBlockID();
}


//-----------------------------------------------------------------------
// Reset - Public C_BlockManager
// Description 
//	Wipes any FSBlocks in memory and sets everything to zero
// 
// Arguments:	None.
// Returns:		None.
//-----------------------------------------------------------------------
inline void C_Flowsheet::Reset()
{
	d_BlockMap.clear();
	d_SourceMap.clear();
	d_DestMap.clear();
	d_BlockFactory->Reset();
}

//-----------------------------------------------------------------------
// BreakLink - Public C_BlockManager
// Description 
//	Breaks a link between two blocks
// 
// Arguments:	from - the id of the block feeding.
//				to - the id of the block receiving the feed
//				port - the number for the port on the from block to use.
// Returns:		None.
//-----------------------------------------------------------------------
inline void C_Flowsheet::BreakLink(BlockID from, BlockID to, PortNo port)
{
	d_DestMap[from].remove(to);
	DeleteSourceLink(from, to, port);
}

//-----------------------------------------------------------------------
// MakeLink - Public C_Flowsheet
// Description 
//	Creates a link between two blocks.
// 
// Arguments:	from - The block where the link starts.
//				to - The block where the link ends.
// Returns:		None.
//-----------------------------------------------------------------------
inline void C_Flowsheet::MakeLink(BlockID from, PortNo fromPort, BlockID to)
{
	d_DestMap[from].push_back(to);
	d_SourceMap[to].push_back(S_FeedSource(d_BlockMap[from], fromPort));
}


//-----------------------------------------------------------------------
// RemoveBlock - Public C_BlockManager
// Description 
//	Removes a block while maintaining the flowsheet structure.
// 
// Arguments:	id - The id of the block to remove.
// Returns:		None.
//-----------------------------------------------------------------------
inline void C_Flowsheet::RemoveBlock(const BlockID &id)
{
	RemoveFromSources(d_DestMap[id], id);
	RemoveFromDest(d_SourceMap[id], id);
	d_DestMap.erase(id);
	d_SourceMap.erase(id);
	d_BlockMap.erase(id);
}

#endif // _FLOWSHEET_