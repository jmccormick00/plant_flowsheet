//======================================================================
// C_BlockFactory.h 
// Author: James McCormick
// Description:
//	A class that creates the blocks.
//======================================================================

#ifndef _BLOCKFACTORY_
#define _BLOCKFACTORY_

#include "I_FSBlock.h"


class C_BlockFactory : public C_SmartPointerObject
{
private:

	// Stores the current ID to hand out
	BlockID d_uiCurrID;

	// Start ID
	BlockID d_uiStartID;

	// A pointer to the flowsheet Parameters
	FSParamsPtr d_fspFSParams;

public:
	// Constructor/Destructor
	 C_BlockFactory(FSParamsPtr FSParams, BlockID startID);
	 ~C_BlockFactory() {}

	 BlockPtr CreateBlock(const ProcessID &procID);

	 void Reset() { d_uiCurrID = d_uiStartID; }

};

//-----------------------------------------------------------------------
// Constructor - Public C_BlockFactory
// Description 
//	Performs Initialization.
// 
// Arguments:	None.
// Returns:		None.
//-----------------------------------------------------------------------
inline C_BlockFactory::C_BlockFactory(FSParamsPtr FSParams, BlockID startID) : d_uiStartID(startID), d_uiCurrID(startID), 
d_fspFSParams(FSParams)
{}

#endif // _BLOCKFACTORY_