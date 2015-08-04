//======================================================================
// FlowData.cpp
// Author: James McCormick
// Description:
//	A data structure for handling the flowdata on the flowsheet.
//	Represents the flow to and from a flowsheet block.
//======================================================================

#include "C_FlowData.h"

// For memcpy,memset
#include <string.h>

// Used for printing to the consol...can be removed later
#include <iostream>
using namespace std;


//int RoundToNearest(const int& num, const int& roundTo)
//{
//	int rem = num % roundTo;
//	if(rem > (roundTo / 2))
//		return (num + roundTo - rem);
//	else
//		return (num - rem);
//}

void C_FlowData::RoundWater()
{
	int rem = ((int)d_FluidRate) % d_fspFSParams->d_iWaterRoundTo;
	if(rem > (d_fspFSParams->d_iWaterRoundTo / 2))
		d_FluidRate = ((int)d_FluidRate + d_fspFSParams->d_iWaterRoundTo - rem);
	else
		d_FluidRate = (int)d_FluidRate - rem;
}

//-----------------------------------------------------------------------
// Copy Constructor - Public C_FlowData
// Description 
//	Copies the flowdata passed into the current one.
// 
// Arguments:	fd - the flowdata to copy
// Returns:		None.
//-----------------------------------------------------------------------
C_FlowData::C_FlowData(const C_FlowData& fd)
{
	d_fpSizeFractions = 0;
	SetNumFractions(fd.d_usNumFractions);
	d_fspFSParams = fd.d_fspFSParams;

	// Copy the array from fd into the current array
	memcpy(d_fpSizeFractions, fd.d_fpSizeFractions, d_usNumFractions * sizeof(float));
	d_FluidRate = fd.d_FluidRate;
	d_SolidRate = fd.d_SolidRate;
	d_PerSolids = fd.d_PerSolids;
}


//-----------------------------------------------------------------------
// PrintFlowData - Public C_FlowData
// Description 
//	Prints the flowdata to the console.
// 
// Arguments:	None.
// Returns:		None.
//-----------------------------------------------------------------------
void C_FlowData::PrintFlowData() const
{
	cout << "-------------------\n";
	cout << "SolidsRate: " << d_SolidRate << endl;
	cout << "FluidsRate: " << d_FluidRate << endl;
	cout << "% SOL: " << d_PerSolids << endl;

	for(int i = 0; i < d_usNumFractions; i++)
		cout << '[' << i << "]\t" << d_fpSizeFractions[i] << endl;

	cout << "-------------------\n";
}


//-----------------------------------------------------------------------
// operator= - Public C_FlowData
// Description 
//	Copies the contents of fd into the current one.
// 
// Arguments:	fd - the flowdata to be assigned to the current one
// Returns:		a pointer to this flowdata
//-----------------------------------------------------------------------
C_FlowData& C_FlowData::operator=(const C_FlowData& fd)
{
	if(d_fspFSParams != fd.d_fspFSParams)
	{
		d_fspFSParams = fd.d_fspFSParams;
		if(d_usNumFractions != fd.d_usNumFractions)
		{
			Clean();
			SetNumFractions(fd.d_usNumFractions);
		}
	}

	// Copy the array from fd into the current array
	memcpy(d_fpSizeFractions, fd.d_fpSizeFractions, d_usNumFractions * sizeof(float));
	d_SolidRate = fd.d_SolidRate;
	d_FluidRate = fd.d_FluidRate;
	d_PerSolids = fd.d_PerSolids;
	return *this;
}


//-----------------------------------------------------------------------
// operator+= - Public C_FlowData
// Description 
//	Adds fd to the current variable and stores the result in the current.
// 
// Arguments:	fd - the flowdata to be added to the current one.
// Returns:		a pointer to this flowdata
//-----------------------------------------------------------------------
C_FlowData& C_FlowData::operator+=(const C_FlowData& fd)
{
	if(d_fspFSParams->d_bUpdateSolids)
	{
		d_SolidRate = 0.0f;
		for(int i = 0; i < d_usNumFractions; i++)
		{
			d_fpSizeFractions[i] += fd.d_fpSizeFractions[i];
			d_SolidRate += d_fpSizeFractions[i];
		}
	}

	if(d_fspFSParams->d_bUpdateWater)
	{
		d_FluidRate += fd.d_FluidRate;
		UpdatePerSolids();
	}

	return *this;
}


//-----------------------------------------------------------------------
// DistrubuteSolids - Public C_FlowData
// Description 
//	Distributes the tph into size fractions.
//
// Arguments:	pass - the size fraction that is passing
//				retained - the size fraction that is retained
//				solidRate - The total amount of solids in the flow 
// Returns:		none.
//-----------------------------------------------------------------------
void C_FlowData::DistributeSolids(const float& pass, const float& retained, const float& solidRate)
{
	short start, stop;

	d_fspFSParams->d_sdSizeDistribution.GetRange(pass, retained, start, stop);

	// Zero the flowdata
	this->Zero();

	float total = 0.0f;

	for(int i = start; i <= stop; i++)
	{
		d_fpSizeFractions[i] = d_fspFSParams->d_sdSizeDistribution.d_sfFractions[i].fFractionalWt * solidRate / 100.0f;
		total += d_fpSizeFractions[i];
	}

	d_SolidRate = total;
}



void C_FlowData::CopySolids(const FlowDataPtr fd)
{
	if(d_fspFSParams != fd->d_fspFSParams)
	{
		d_fspFSParams = fd->d_fspFSParams;
		if(d_usNumFractions != fd->d_usNumFractions)
		{
			Clean();
			SetNumFractions(fd->d_usNumFractions);
		}
	}

	// Copy the array from fd into the current array
	memcpy(d_fpSizeFractions, fd->d_fpSizeFractions, d_usNumFractions * sizeof(float));
	d_SolidRate = fd->d_SolidRate;
	
}
	

void C_FlowData::CopyWater(const FlowDataPtr fd)
{
	d_FluidRate = fd->d_FluidRate;
	d_PerSolids = fd->d_PerSolids;
}


void C_FlowData::ZeroSolids()
{
	d_SolidRate = 0.0f;
	// Set the array to zero
	memset(d_fpSizeFractions, 0, d_usNumFractions * sizeof(float));

}

void C_FlowData::ZeroWater()
{
	d_FluidRate = 0.0f;
	d_PerSolids = 0.0f;
}
