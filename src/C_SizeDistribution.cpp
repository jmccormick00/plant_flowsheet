//======================================================================
// SizeDistribution.cpp
// Author: James McCormick
// Description:
//	A class that represents a size distribution.
//======================================================================

#include "C_SizeDistribution.h"
#include <fstream>
#include <sstream>

// For Printing function - can be removed later
#include <iostream>
using namespace std;

//-----------------------------------------------------------------------
// LoadSizeDist - Public C_SizeDistribution
// Description 
//	Loads in a size distribution from a text file.
// 
// Arguments:	fileName - The name of the file to open.
// Returns:		bool - true if successfull, false otherwise.
//-----------------------------------------------------------------------
bool C_SizeDistribution::LoadSizeDist(const char* fileName)
{
	this->UnloadSizeDist();

	// Try to open the file
	ifstream file(fileName);

	// Check to see if the file opened
	if(!file)
		return false;

	// Create a buffer to store a line from the file
	string buffer;

	// Get the number of lines to read in
	getline(file, buffer);
	istringstream line1(buffer);

	line1 >> d_sNumFractions;

	// Allocate the memory
	d_sfFractions = new S_SizeFraction[d_sNumFractions];

	int i = 0;
	while(getline(file, buffer))
	{
		// Create a string stream from the line read in
		istringstream line(buffer);
		
		// Read the line into the fraction block
		line >> d_sfFractions[i].fPassing;
		line >> d_sfFractions[i].fRetained;
		line >> d_sfFractions[i].fFractionalWt;
		line >> d_sfFractions[i].fCumWt;

		d_sfFractions[i].fAvgSize = (d_sfFractions[i].fPassing + d_sfFractions[i].fRetained) / 2.0f;

		i++;
	}

	// Close the file
	file.close();

	return true;
}


//-----------------------------------------------------------------------
// GetRange - Private C_SizeDistribution
// Description 
//	Finds the start and end indices into the array for a certain 
//	size fraction.
//
// Arguments:	pass - the starting point to find.
//				retained - the end point to find.
// Returns:		start - the starting point in the array (By Ref)
//				end - the ending point in the array (By Ref)
//-----------------------------------------------------------------------
void C_SizeDistribution::GetRange(const float& pass, const float& retained, short& start, short& end) const
{
	// Test to see if pass is the top size
	if(d_sfFractions[0].fPassing < pass)
		start = 0;

	for(int i = 0; i < d_sNumFractions; i++)
	{
		if(d_sfFractions[i].fPassing == pass)
			start = i;
		if(d_sfFractions[i].fRetained == retained)
		{
			end = i;
			continue;
		}
	}
}


//-----------------------------------------------------------------------
// GetCumalativeWt - Public C_SizeDistribution
// Description 
//	Gets the cumalative wt from the top size down to the
//	size retained.
//
// Arguments:	retained - the size that is retained
// Returns:		float - the cumalative wt
//-----------------------------------------------------------------------
float C_SizeDistribution::GetCumalativeWt(const float& retained) const
{
	int i = 0;
	while(d_sfFractions[i].fRetained != retained) i++;

	return d_sfFractions[i].fCumWt;
}


//-----------------------------------------------------------------------
// GetFractionalWt - Public C_SizeDistribution
// Description 
//	Gets the fractional weight between two size fractions.
//
// Arguments:	pass - the top size
//				retained - the bottom size that is retained.
// Returns:		float - the fractional wt
//-----------------------------------------------------------------------
float C_SizeDistribution::GetFractionalWt(const float& pass, const float& retained)  const
{
	float fractWt = 0.0f;
	short start, stop;

	this->GetRange(pass, retained, start, stop);

	for(int i = start; i <= stop; i++)
		fractWt += d_sfFractions[i].fFractionalWt;

	return fractWt;
}

//-----------------------------------------------------------------------
// GetAVGSize - Public C_SizeDistribution
// Description 
//	Gets the average grain size between two size fractions.
//
// Arguments:	pass - the top size
//				retained - the bottom size that is retained.
// Returns:		float - the avg grain size in mm
//-----------------------------------------------------------------------	
float C_SizeDistribution::GetAVGSize(const float& pass, const float& retained) const
{
	float avgSum = 0.0f;
	float fractSum = 0.0f;

	short start, stop;

	this->GetRange(pass, retained, start, stop);

	for(int i = start; i <= stop; i++)
	{
		fractSum += d_sfFractions[i].fFractionalWt;
		avgSum += (d_sfFractions[i].fFractionalWt /d_sfFractions[i].fAvgSize);
	}

	return (fractSum/avgSum);
}



//-----------------------------------------------------------------------
// PrintSizeDist - Public C_SizeDistribution
// Description 
//	Prints the size distributions to the console
//
// Arguments:	None.
// Returns:		None.
//-----------------------------------------------------------------------
void C_SizeDistribution::PrintSizeDist() const
{
	if(d_sNumFractions == 0) return;

	cout << "Passing | Retained | Fract Wt | Cum Wt\n";
	for(int i = 0; i < d_sNumFractions; i++)
	{
		cout << d_sfFractions[i].fPassing << "\t" << d_sfFractions[i].fRetained 
			<< "\t" << d_sfFractions[i].fFractionalWt << "\t" << d_sfFractions[i].fCumWt << "\t" << d_sfFractions[i].fAvgSize << endl;  
	}
}
