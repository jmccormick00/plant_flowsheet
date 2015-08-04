//======================================================================
// FlowData.h 
// Author: James McCormick
// Description:
//	A data structure for handling the flowdata on the flowsheet.
//	Represents the flow to and from a flowsheet block.
//======================================================================

#ifndef _FLOWDATA_
#define _FLOWDATA_

#include "C_SmartPointer.h"
#include "C_FlowSheetParameters.h"
#include "Typedefs.h"

class C_FlowData;
typedef C_SmartPointer<C_FlowData> FlowDataPtr;

class C_FlowData : public C_SmartPointerObject
{
private:
	// PRIVATE DATA MEMBERS====================================================

	// The flowsheet parameters structure pointer
	FSParamsPtr d_fspFSParams;

	// Stores the solid rate per size fraction
	float* d_fpSizeFractions;		

	// The number of elements in the sizeFraction array
	unsigned short d_usNumFractions;

	// PRIVATE METHODS=========================================================

	// Sets the number of fractions and allocates the memory
	bool SetNumFractions(const unsigned short numFract);

	// Removes any memory allocated
	void Clean() 
	{ 
		if(d_fpSizeFractions != 0)
		{
			delete [] d_fpSizeFractions;
			d_fpSizeFractions = 0;
			d_usNumFractions = 0;
		}
	} 

public:
	// PUBLIC DATA MEMBERS=====================================================

	// Stores the total solid rate
	SolidsRate d_SolidRate;	
	
	// Stores the fluid rate
	FluidRate d_FluidRate;

	// Percent Solids
	PercentSolids d_PerSolids;
	
	// PUBLIC METHODS==========================================================

	// Constructor/Destructor
	C_FlowData();
	C_FlowData(const C_FlowData& fd);
	~C_FlowData() { Clean(); }

	// Zeros the flowdata
	void Zero();
	void ZeroSolids();
	void ZeroWater();

	// Initializes the flowdata and zero's it
	void Init(FSParamsPtr params) 
	{
		Clean();
		d_fspFSParams = params; 
		SetNumFractions(d_fspFSParams->d_sdSizeDistribution.GetNumSizeFractions());
		Zero();
	}

	// Resets the flowdata - Clears the memory, allocates new memory and then zero's it
	void Reset()
	{
		Clean();
		SetNumFractions(d_fspFSParams->d_sdSizeDistribution.GetNumSizeFractions());
		Zero();
	}

	void RoundWater();

	void CopySolids(const FlowDataPtr fd);
	void CopyWater(const FlowDataPtr fd);

	unsigned short GetNumFractions() const { return d_usNumFractions; }

	// Distributes the solids into the size fractions
	void DistributeSolids(const float& pass, const float& retained, const float& solidRate);

	// Copys the fd variable into the current variable
	C_FlowData& operator=(const C_FlowData& fd);

	// Adds fd to the current variable and stores the result in the current
	C_FlowData& operator+=(const C_FlowData& fd);

	// Accessors
	float& operator[] (int i) { return d_fpSizeFractions[i]; }
	float operator[] (int i) const { return d_fpSizeFractions[i]; }

	// Fluid calculations
	void CalculateFluidsBasedOnSurfaceMoisture(const float& sm);
	void CalculateFluidsBasedOnPerSolids(const float& ps);

	// Prints the flowdata to the console
	void PrintFlowData() const;

	// Updates the percent solids
	void UpdatePerSolids();

	friend class C_BlockPorts;
};

//-----------------------------------------------------------------------
// Constructor - Public C_FlowData
// Description 
//	Sets everything to zero.
// 
// Arguments:	None.
// Returns:		None.
//-----------------------------------------------------------------------
inline C_FlowData::C_FlowData()
{
	d_fpSizeFractions = 0;
	d_usNumFractions = 0;
	d_PerSolids = 0.0f;
}

//-----------------------------------------------------------------------
// Set NumFractions- Private C_FlowData
// Description 
//	Sets the number of size fractions and allocates memory of the array.
// 
// Arguments:	numFract - the number of fractions
// Returns:		None.
//-----------------------------------------------------------------------
inline bool C_FlowData::SetNumFractions(const unsigned short numFract)
{ 
	if(d_fpSizeFractions) return true;

	d_usNumFractions = numFract; 
	d_fpSizeFractions = new float[numFract];

	if(!d_fpSizeFractions)  // Memory was not allocated
	{
		d_usNumFractions = 0;
		return false;
	}

	return true;
}


//-----------------------------------------------------------------------
// CalculateFluidsBasedOnSurfaceMoisture - Public C_FlowData
// Description 
//	Calculates the fluids based on surface moisture.
// 
// Arguments:	sm - surface moisture. as a percent
// Returns:		None.
//-----------------------------------------------------------------------
inline void C_FlowData::CalculateFluidsBasedOnSurfaceMoisture(const float& sm)
{
	float ps = 1 - sm;

	this->CalculateFluidsBasedOnPerSolids(ps);
}

//-----------------------------------------------------------------------
// CalculateFluidsBasedOnPerSolids - Public C_FlowData
// Description 
//	Calculates the fluids based on percent solids.
// 
// Arguments:	ps - percent solids. as a percent.
// Returns:		None.
//-----------------------------------------------------------------------
inline void C_FlowData::CalculateFluidsBasedOnPerSolids(const float& ps)
{
	if(d_fspFSParams->d_bMetric)
		d_FluidRate = ((d_SolidRate / ps) - d_SolidRate);
	else
		d_FluidRate = ((d_SolidRate / ps) - d_SolidRate) * 4;

	RoundWater();

	d_PerSolids = ps;
}


//-----------------------------------------------------------------------
// UpdatePerSolids - Private C_FlowData
// Description 
//	Updates the percent solids
// 
// Arguments:	None.
// Returns:		None.
//-----------------------------------------------------------------------
inline void C_FlowData::UpdatePerSolids()
{
	if(d_fspFSParams->d_bMetric)
		d_PerSolids = d_SolidRate / (d_SolidRate + d_FluidRate);
	else
		d_PerSolids = d_SolidRate / (d_SolidRate + (d_FluidRate / 4));
}


//-----------------------------------------------------------------------
// Zero - Public C_FlowData
// Description 
//	Zeros the data.
// 
// Arguments:	None.
// Returns:		None.
//-----------------------------------------------------------------------
inline void C_FlowData::Zero()
{
	ZeroSolids();
	ZeroWater();
}



#endif // _FLOWDATA_