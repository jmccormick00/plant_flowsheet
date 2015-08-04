//======================================================================
// main.cpp 
// Author: James McCormick
// Discription:
//	A driver program for testing the flowsheet class.
//
//======================================================================


#include "C_FeedBlock.h"
#include "C_PrintBlock.h"
#include "C_DeslimeScreenSD.h"
#include "C_DeslimeScreenDD.h"
#include "C_SumpPump.h"

#include "C_Flowsheet.h"


#include <iostream>

int main()
{
	C_Flowsheet flowSheet;

	// Set some basic options in the flowsheet
	flowSheet.SetMetric(false);
	flowSheet.SetDelta(0.01f);
	flowSheet.SetMaxIterations(100);
	flowSheet.SetRoundToWater(2);

	// Load the size distribution
	if(!flowSheet.LoadSizeDistribution("Test.txt"))
	{
		std::cout << "Failed to load size distribution!!\n";
		return 0;
	}

	flowSheet.PrintSizeDistribution();
	std::cout << "------------------------------------------------\n";

	//flowSheet.CreateBlock(PROCID_FEED);					// id 100
	//flowSheet.CreateBlock(PROCID_PRINTBLOCK);			// id 101
	//flowSheet.CreateBlock(PROCID_FEED);					// id 102
	//flowSheet.CreateBlock(PROCID_DESLIME_SINGLEDECK);	// id 103
	//flowSheet.CreateBlock(PROCID_PRINTBLOCK);			// id 104
	//flowSheet.CreateBlock(PROCID_PRINTBLOCK);			// id 105
	//flowSheet.CreateBlock(PROCID_SUMPPUMP);				// id 106
	//flowSheet.CreateBlock(PROCID_PRINTBLOCK);			// id 107

	//// Put both feeds into the print block
	//flowSheet.MakeLink(100, 0, 101);
	//flowSheet.MakeLink(102, 0, 101);

	//// Make both feedBlocks feed the deslime screen
	//flowSheet.MakeLink(102, 0, 103);
	//flowSheet.MakeLink(100, 0, 103);

	//flowSheet.MakeLink(103, 1, 104);
	//flowSheet.MakeLink(103, 2, 105);

	//flowSheet.MakeLink(103, 1, 106);
	//flowSheet.MakeLink(106, 0, 107); 

	//flowSheet.PushParameters( new C_FeedBlockParams(100, PROCID_FEED, 700.0f, 0.04f) );
	//flowSheet.PushParameters( new C_FeedBlockParams(102, PROCID_FEED, 1000.0f, 0.04f) );
	//flowSheet.PushParameters( new C_DeslimeScreenSDParams(103, PROCID_DESLIME_SINGLEDECK, 0.03f, 6.35, 300.0f) );
	//flowSheet.PushParameters( new C_SumpPumpParams(106, PROCID_SUMPPUMP, 300.0f) ); 

	//flowSheet.SetUpdateSolids(true);
	//flowSheet.SetUpdateWater(true);
	//flowSheet.SolveFlowSheet();

	// Load the size distribution
	/*if(!flowSheet.LoadSizeDistribution("Test1.txt"))
	{
		std::cout << "Failed to load size distribution!!\n";
		return 0;
	}

	flowSheet.SetUpdateSolids(true);
	flowSheet.SetUpdateWater(false);
	flowSheet.SolveFlowSheet();*/


	flowSheet.CreateBlock(PROCID_FEED);					// id 100
	flowSheet.CreateBlock(PROCID_PRINTBLOCK);			// id 101
	flowSheet.CreateBlock(PROCID_DESLIME_DOUBLEDECK);	// id 102
	flowSheet.CreateBlock(PROCID_PRINTBLOCK);			// id 103
	flowSheet.CreateBlock(PROCID_PRINTBLOCK);			// id 104
	flowSheet.CreateBlock(PROCID_PRINTBLOCK);			// id 105
	flowSheet.CreateBlock(PROCID_SUMPPUMP);				// id 106
	flowSheet.CreateBlock(PROCID_PRINTBLOCK);			// id 107
	flowSheet.CreateBlock(PROCID_SUMPPUMP);				// id 108
	flowSheet.CreateBlock(PROCID_PRINTBLOCK);			// id 109

	// What the plant feed block feeds
	flowSheet.MakeLink(100, 0, 101);
	flowSheet.MakeLink(100, 0, 102);

	// The deslime screen feeds
	flowSheet.MakeLink(102, 1, 105);
	flowSheet.MakeLink(102, 1, 106);
	flowSheet.MakeLink(102, 2, 104);
	flowSheet.MakeLink(102, 2, 108);
	flowSheet.MakeLink(102, 3, 103);

	// The RC pump feeds a print block
	flowSheet.MakeLink(106, 0, 107);

	// The HMC pump feeds a print block
	flowSheet.MakeLink(108, 0, 109);

	flowSheet.PushParameters( new C_FeedBlockParams(100, PROCID_FEED, 700.0f, 0.7f) );
	flowSheet.PushParameters( new C_DeslimeScreenDDParams(102, PROCID_DESLIME_DOUBLEDECK, 0.10f, 0.14f, 25.4f, 6.35, 300.0f) );
	flowSheet.PushParameters( new C_SumpPumpParams(106, PROCID_SUMPPUMP, 300.0f) );
	flowSheet.PushParameters( new C_SumpPumpParams(108, PROCID_SUMPPUMP, 300.0f) );

	flowSheet.SetUpdateSolids(true);
	flowSheet.SetUpdateWater(false);
	flowSheet.SolveFlowSheet();

	flowSheet.SetUpdateSolids(false);
	flowSheet.SetUpdateWater(true);
	flowSheet.SolveFlowSheet();

	return 0;
}
