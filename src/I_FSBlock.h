//======================================================================
// FSBlock.h
// Author: James McCormick
// Description:
//	The class for the flowsheet blocks.  Provides basic properties, 
//	methods, and interface that every block must have.
//======================================================================

#ifndef _FSBLOCK_
#define _FSBLOCK_

#include "Typedefs.h"
#include "C_SmartPointer.h"
#include "C_BlockPorts.h"
#include "I_FSBlockParameters.h"

class I_FSBlock : public C_SmartPointerObject
{
protected:
	
	// PROTECTED DATA MEMBERS==================================================

	// The block's unique ID - Generated when it is created
	BlockID d_BlockID;

	// Stores the process id to identify the type of block
	ProcessID d_ProccessID;

	// The ports on the block
	C_BlockPorts d_Ports;

	// A pointer to the flowsheet Parameters
	FSParamsPtr d_fspFSParams;

	// The amount of washwater coming in for sprays, washwater, or add water
	float d_AddWater;	


	// PROTECTED METHODS=======================================================

	virtual void UpdateWater() = 0;
	virtual void UpdateSolids() = 0;

	// Force the constructor that takes parameters
	I_FSBlock() { }
	I_FSBlock(const I_FSBlock &o) { }

public:

	// PUBLIC DATA MEMBERS=====================================================
	
	// PUBLIC METHODS==========================================================

	// Constructor/Destructor
	I_FSBlock(FSParamsPtr fsp, const BlockID& ID) : d_fspFSParams(fsp), d_BlockID(ID), d_AddWater(0)
	{}

	virtual ~I_FSBlock() {}

	float GetAddWater() const { return d_AddWater; }

	// Get the ID of the block
	BlockID GetBlockID() const { return d_BlockID; }

	// Gets the type for this block
	ProcessID GetProcessID() const { return d_ProccessID; }

	// Gets the flowdata from a specific port
	C_FlowData* GetFlowData(const PortNo& pn) { return d_Ports.GetFlowData(pn); }

	// Gets the Ports
	C_BlockPorts& GetPorts() { return d_Ports; }

	// The size distribution has been updated
	virtual void OnNewSizeDistribution() = 0;

	// Called by the flowsheet to have the block update itself.
	// The flowsheet will update the flowdata for the block before calling update.
	virtual void OnUpdate();

	// Is called to pass the parameters to the block
	virtual void OnParameters(BlockParamsPtr) = 0;
};

typedef C_SmartPointer<I_FSBlock> BlockPtr;

#endif // _FSBLOCK_