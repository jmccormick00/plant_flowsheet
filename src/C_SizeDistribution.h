//======================================================================
// SizeDistribution.h 
// Author: James McCormick
// Description:
//	A class that represents a size distribution.
//	Provides functions to query Avg grain size, cumalative weights, and
//	fractional weights.
//======================================================================

#ifndef _SIZEDISTRIBUTION_
#define _SIZEDISTRIBUTION_


class C_SizeDistribution
{
	friend class C_FlowData;   // Allow the flowdata class to access the private data and methods

private:

	// PRIVATE DATA MEMBERS===================================================

	// Structure for storing a size fraction
	struct S_SizeFraction
	{
		float fRetained;		// in millimeters
		float fPassing;			// in millimeters
		float fCumWt;			// decimal
		float fFractionalWt;	// decimal
		float fAvgSize;			// Used for Avg grain size calcs - Calculated value
	};

	// Number of size fractions
	unsigned short d_sNumFractions;


	// The array that holds the size fractions 
	// [0] is the top size
	S_SizeFraction *d_sfFractions;

	// PRIVATE METHODS=========================================================


public:

	// PUBLIC DATA MEMBERS=====================================================
	
	// PUBLIC METHODS==========================================================

	// Constructor/Destructor
	C_SizeDistribution();
	~C_SizeDistribution();

	float TopSize() const { return d_sfFractions[0].fPassing; }

	// Load a size distribution from a txt file
	bool LoadSizeDist(const char* fileName);

	// Unload the size distribution from memory
	void UnloadSizeDist();

	// Get the number of size fractions
	unsigned short GetNumSizeFractions() const { return d_sNumFractions; }

	// Finds the start and end indices in the array for a certain size fraction
	void GetRange(const float& pass, const float& retained, short& start, short& end) const;

	// Gets the cumalative wt from the top size down to the
	// size retained
	float GetCumalativeWt(const float& retained) const;

	float GetTopSize() const { return d_sfFractions[0].fPassing; }

	// Gets the fractional weight between two size fractions
	float GetFractionalWt(const float& pass, const float& retained) const;

	// Gets the average grain size for a size fraction in mm
	float GetAVGSize(const float& pass, const float& retained) const;

	// Prints the SizeDistribution to the screen
	void PrintSizeDist() const;
};


//-----------------------------------------------------------------------
// Constructor - Public C_SizeDistribution
// Description 
//	Zeros everything.
// 
// Arguments:	None.
// Returns:		None.
//-----------------------------------------------------------------------
inline C_SizeDistribution::C_SizeDistribution()
{
	d_sfFractions = 0;
	d_sNumFractions = 0;
}


//-----------------------------------------------------------------------
// UnloadSizeDist - Public C_SizeDistribution
// Description 
//	Unloads a size distribution from memory.
//
// Arguments:	None.
// Returns:		None.
//-----------------------------------------------------------------------
inline void C_SizeDistribution::UnloadSizeDist()
{
	if(!d_sfFractions) return;

	delete[] d_sfFractions;
	d_sfFractions = 0;
	d_sNumFractions = 0;
}


//-----------------------------------------------------------------------
// Destructor - Public C_SizeDistribution
// Description 
//	Clears the memory used by the size table.
//
// Arguments:	None.
// Returns:		None.
//-----------------------------------------------------------------------
inline C_SizeDistribution::~C_SizeDistribution()
{
	this->UnloadSizeDist();
}


#endif // _SIZEDISTRIBUTION_