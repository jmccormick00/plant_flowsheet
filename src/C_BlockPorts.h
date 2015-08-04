//======================================================================
// C_BlockPorts.h 
// Author: James McCormick
// Description:
//	Handles the ports for the blocks.  Performs the initializations,
//	memory management, etc.
//======================================================================

#ifndef _BLOCKPORTS_
#define _BLOCKPORTS_

#include "C_FlowData.h"

class C_BlockPorts : public C_SmartPointerObject
{
protected:
	// PROTECTED DATA MEMBERS==================================================

	// The flowdata for the ports
	C_FlowData* d_Ports;

	// The number of ports
	unsigned short d_usNumPorts;

	// PROTECTED METHODS=======================================================

	// Clean the memory allocated
	void Clean();

public:
	// PUBLIC DATA MEMBERS=====================================================

	// PUBLIC METHODS==========================================================
	
	// Constructor/Destructor
	C_BlockPorts() : d_Ports(0), d_usNumPorts(0) {}
	C_BlockPorts(C_BlockPorts&);
	~C_BlockPorts() { Clean(); }

	// Resets all of the flow data objects and zero's them
	void Reset();

	// Copys the bp variable into the current variable
	C_BlockPorts& operator=(const C_BlockPorts& bp);

	unsigned short GetNumPorts() const { return d_usNumPorts; }

	// Init the ports - Must be called before they can be used
	bool Init(FSParamsPtr fsParams, const unsigned short& NumPorts);
	
	C_FlowData* GetFlowData(const unsigned short& port = 0)
	{
		return &d_Ports[port];
	}

	void UpdatePercentSolids();

	C_SmartPointer<C_BlockPorts> Difference(const C_BlockPorts& b); 
};



inline void C_BlockPorts::UpdatePercentSolids()
{
	for(int i = 0; i < d_usNumPorts; i++)
		d_Ports[i].UpdatePerSolids();
}



//-----------------------------------------------------------------------
// Destructor - Public C_BlockPorts
// Description 
//	Cleans up any memory used and sets everything to null.
// 
// Arguments:	None.
// Returns:		None.
//-----------------------------------------------------------------------
inline void C_BlockPorts::Clean()
{
	// Delete the array of ports
	if(d_Ports != 0)
	{
		delete [] d_Ports;
		d_Ports = 0;
		d_usNumPorts = 0;
	}
}


//-----------------------------------------------------------------------
// Init - Public C_BlockPorts
// Description 
//	Allocates the memory, initializes the ports.
// 
// Arguments:	None.
// Returns:		None.
//-----------------------------------------------------------------------
inline bool C_BlockPorts::Init(FSParamsPtr fsParams, const unsigned short& numPorts)
{
	Clean();

	d_Ports = new C_FlowData[numPorts];

	if(d_Ports == 0)
		return false;
	
	d_usNumPorts = numPorts;

	for(int i = 0; i < numPorts; i++)
		d_Ports[i].Init(fsParams);

	return true;
}


//-----------------------------------------------------------------------
// Reset - Public C_BlockPorts
// Description 
//	Resets all of the flow data objects based on the currently loaded
//	size distribution.
// 
// Arguments:	None.
// Returns:		None.
//-----------------------------------------------------------------------
inline void C_BlockPorts::Reset()
{
	for(int i = 0; i < d_usNumPorts; i++)
		d_Ports[i].Reset();
}

//-----------------------------------------------------------------------
// Copy Constructor - Public C_BlockPorts
// Description 
//	Copies one set of ports into the current one
// 
// Arguments:	b - The other port data to subtract.
// Returns:		A reference to this C_BlockPorts
//-----------------------------------------------------------------------
inline C_BlockPorts::C_BlockPorts(C_BlockPorts& b)
{
	d_Ports = 0;
	Init(b.d_Ports[0].d_fspFSParams, b.d_usNumPorts);

	for(int i = 0; i < d_usNumPorts; i++)
		d_Ports[i] = b.d_Ports[i];
}


//-----------------------------------------------------------------------
// Difference - Public C_BlockPorts
// Description 
//	Finds the difference between two sets of port data
// 
// Arguments:	b - The other port data to subtract.
// Returns:		A new C_BlockPorts variable containing the difference of
//				this set of ports minus b's set of ports.
//-----------------------------------------------------------------------
inline C_SmartPointer<C_BlockPorts> C_BlockPorts::Difference(const C_BlockPorts& b)
{
	C_SmartPointer<C_BlockPorts> deltaPort;
	if(b.d_usNumPorts != d_usNumPorts)
		return deltaPort;

	deltaPort = new C_BlockPorts;

	// Init the new set of ports and zero all of the data
	deltaPort->Init(d_Ports->d_fspFSParams , d_usNumPorts);

	for(int i = 0; i < d_usNumPorts; i++)
	{
		deltaPort->d_Ports[i].d_FluidRate = d_Ports[i].d_FluidRate - b.d_Ports[i].d_FluidRate;
		deltaPort->d_Ports[i].d_SolidRate = d_Ports[i].d_SolidRate - b.d_Ports[i].d_SolidRate;
		for(int j = 0; j < d_Ports->d_usNumFractions; j++)
			deltaPort->d_Ports[i].d_fpSizeFractions[j] = d_Ports[i].d_fpSizeFractions[j] - b.d_Ports[i].d_fpSizeFractions[j];
	}
	return deltaPort;
}


#endif // _BLOCKPORTS_