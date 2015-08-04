//======================================================================
// C_Screen.h 
// Author: James McCormick
// Description:
//	Interface for a screen block.
//	The first port is the feed.
//
//	* For Drain and rinse screens *
//	The second is for drain water.
//	The third is for the bleed, if there is any.
//	The fourth is for the rinse water.
//	The fifth, sixth,... would be the deck discharge
//	
//	* For Deslime Screen *
//	The second is for drain water.
//	The third, fourth and so on is the deck discharge.
//
// * The d_DeckCutPoint[] starts at 0 with the bottom deck
// and then goes up
//======================================================================

#ifndef _SCREEN_
#define _SCREEN_

#include "I_FSBlock.h"

class I_Screen : public I_FSBlock
{
protected:

	// PROTECTED DATA MEMBERS==================================================
	float *d_DeckCutPoints;
	float *d_SMPerDeck;
	short d_NumDecks;

	// PROTECTED METHODS=======================================================
	void ScreenTheFeed(int start);

public:

	// PUBLIC DATA MEMBERS=====================================================

	// PUBLIC METHODS==========================================================

	// Constructor
	I_Screen(FSParamsPtr fsp, const BlockID& ID, short numDecks) : I_FSBlock(fsp, ID), d_NumDecks(numDecks)
	{
		d_SMPerDeck = new float[numDecks];
		d_DeckCutPoints = new float[numDecks];
	}

	~I_Screen()
	{
		delete[] d_SMPerDeck;
		delete[] d_DeckCutPoints;
	}

	// The size distribution has been updated
	virtual void OnNewSizeDistribution() = 0;

	// Called by the flowsheet to have the block update itself.
	// The flowsheet will update the flowdata for the block before calling update.
	//virtual void OnUpdate() = 0;

	// Is called to pass the parameters to the block
	virtual void OnParameters(BlockParamsPtr) = 0;
};


#endif // _SCREEN_
