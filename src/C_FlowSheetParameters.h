//======================================================================
// C_FlowsheetParameters.h 
// Author: James McCormick
// Description:
//	A class for storing the flowsheet properties that all blocks and
//	flowsheet will need to access.
//======================================================================

#ifndef _FLOWSHEETPARAMS_
#define _FLOWSHEETPARAMS_

#include "C_SizeDistribution.h"
#include "C_SmartPointer.h"

class S_FlowSheetParams : public C_SmartPointerObject
{
public:
	int d_iWaterRoundTo;					// The value to round the water to
	bool d_bUpdateWater;					// Tell the blocks to update the water
	bool d_bUpdateSolids;					// Tell the blocks to update the solids
	bool d_bMetric;							// States if metric units are to be used
	C_SizeDistribution d_sdSizeDistribution;	// The flowsheet size distribution
};

typedef C_SmartPointer<S_FlowSheetParams> FSParamsPtr;

#endif // _FLOWSHEETPARAMS_