//======================================================================
// C_BlockFactory.cpp
// Author: James McCormick
// Description:
//	A class that creates the blocks.
//======================================================================

#include "C_FeedBlock.h"
#include "C_PrintBlock.h"
#include "C_DeslimeScreenSD.h"
#include "C_BlockFactory.h"
#include "C_SumpPump.h"
#include "C_DeslimeScreenDD.h"

//-----------------------------------------------------------------------
// CreateBlock - Public C_BlockFactory
// Description 
//	Creates the block.
// 
// Arguments:	procID - The id of the type of process the block represents.
// Returns:		A pointer to the block, null if unsucessful.
//-----------------------------------------------------------------------
BlockPtr C_BlockFactory::CreateBlock(const ProcessID &procID)
{
	switch(procID)
	{
		case PROCID_FEED:
		{
			return new C_FeedBlock(d_fspFSParams, d_uiCurrID++);
		}
		case PROCID_PRINTBLOCK:
		{
			return new C_PrintBlock(d_fspFSParams, d_uiCurrID++);
		}
		case PROCID_DESLIME_SINGLEDECK:
		{
			return new C_DeslimeScreenSD(d_fspFSParams, d_uiCurrID++);
		}
		case PROCID_SUMPPUMP:
		{
			return new C_SumpPump(d_fspFSParams, d_uiCurrID++);
		}
		case PROCID_DESLIME_DOUBLEDECK:
		{
			return new C_DeslimeScreenDD(d_fspFSParams, d_uiCurrID++);
		}
		default:
		{
			return NULL;
		}
	}
}