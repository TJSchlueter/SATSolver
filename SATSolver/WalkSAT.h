#pragma once
#include <string>
#include <vector>
#include "SAT_puzzle.h"
#using <System.dll>
using namespace std;
using namespace System;
using namespace System::IO;
using namespace System::Diagnostics;

/// <summary>
/// Class containing the necessary data structures and methods to
/// solve a SAT formula through use of the WalkSAT algorithm.
/// </summary>
class WalkSAT
{
public:

	/// <summary>
	/// Default constructor.
	/// </summary>
	WalkSAT();

	/// <summary>
	/// Constructor.
	/// </summary>
	/// <param name="inPuzzle">SAT_puzzle object to be solved.</param>
	WalkSAT(SAT_puzzle inPuzzle);

	/// <summary>
	/// Runs the hill climb algorithm
	/// </summary>
	/// <returns>an integer representing success, failure, or timing out.</returns>
	int run_WalkSAT();

	/// <summary>
	/// checks to see if the formula has been solved or not.
	/// </summary>
	/// <returns>returns true if solved, false if not</returns>
	bool formulaSolved();

	/// <summary>
	/// checks how many clauses in the formula have been satisfied
	/// </summary>
	/// <returns>an integer equal to the number of satisfied clauses</returns>
	int checkPuzzle();

private:

	/// <summary>
	/// Searches for the best index to flip to improve solution fitness.
	/// </summary>
	/// <returns>the index of the best variable to flip</returns>
	int bestStep();

	int randStep();

	SAT_puzzle baseFormula;
	int MAXTIME = 10000;
};

