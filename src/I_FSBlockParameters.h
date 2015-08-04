//======================================================================
// I_FSBlockParameters.h
// Author: James McCormick
// Description:
//	The base class for any flowsheet parameters list.  This provides the
//	interface for the parameters class.
//======================================================================

#ifndef _FSBLOCKPARAMETERS_
#define _FSBLOCKPARAMETERS_

#include "Typedefs.h"
#include "ProcessIDs.h"
#include "C_SmartPointer.h"

class I_FSBlockParameters : public C_SmartPointerObject
{
protected:
	I_FSBlockParameters();
	I_FSBlockParameters(const I_FSBlockParameters&);

public:
	I_FSBlockParameters(BlockID id, ProcessID p) { d_BlockID = id; d_ProcessID = p; }
	virtual ~I_FSBlockParameters() {}

	// The id of the block the parameters are associated with
	BlockID d_BlockID;

	// The type of process the parameters are for
	ProcessID d_ProcessID;
};

typedef C_SmartPointer<I_FSBlockParameters> BlockParamsPtr;

#endif // _FSBLOCKPARAMETERS_
