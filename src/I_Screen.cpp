//======================================================================
// C_Screen.cpp
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

#include "I_Screen.h"

void I_Screen::ScreenTheFeed(int start)
{
	// Get the feed
	C_FlowData* feed = d_Ports.GetFlowData(0);
	
	float pass, retained;
	short first, last;
	for(int i = 0; i <= d_NumDecks; i++)
	{
		if(i == 0)
			retained = 0.0f;
		else
			retained = pass;

		if(i == d_NumDecks)
			pass = d_fspFSParams->d_sdSizeDistribution.GetTopSize();
		else
			pass = d_DeckCutPoints[i];

		d_fspFSParams->d_sdSizeDistribution.GetRange(pass, retained, first, last);

		C_FlowData* port = d_Ports.GetFlowData(start + i);			
		port->Zero();

		for(int j = first; j <= last; j++)
		{
			(*port)[j] = (*feed)[j];
			(*port).d_SolidRate += (*feed)[j];
		}
	}
}
