/* Author: Jacob Schlueter
* Creation Date: January 23, 1015
* Creation date C# version: Fall 2014
* Project name: SAT Solver
* Purpose: develop 3 different algorithms from a specified list to
*  solve SAT formulas read into the program from files.
*  It is expected that there will be a maximum permissible run time
*  for each of the developed algorithms, but those that finish within
*  the time limit will print to file the details of the solution to
*  the formula, which may be a simple remark that no solution exists.
*  Those that do not finish will print to file a statement that it is
*  not known whether or not a solution exists.
*/

#pragma once
#include <string>
#include <vector>
#include "SAT_puzzle.h"
using namespace std;
using namespace System;
using namespace System::IO;

/// <summary>
/// Class containing the necessary data structures and methods to
/// solve a SAT formula through use of the DPLL algorithm.
/// </summary>
class Genetic
{

public: 
	/// <summary>
	/// Constructor
	/// </summary>
	Genetic();

	/// <summary>
	/// Driving method that performs most of the Genetic algorithm functionality/
	/// </summary>
	/// <param name="in_puzzle">SAT formula to be solved</param>
	int runGenetic(SAT_puzzle inPuzzle);

private:
	/// <summary>
	/// Create a child SAT_puzzle object
	/// </summary>
	/// <param name="dad">Parent node 1</param>
	/// <param name="mom">Parent node 2</param>
	/// <returns>child node, combining parent node values randomly</returns>
	SAT_puzzle makeChild(SAT_puzzle dad, SAT_puzzle mom);

	/// <summary>
	/// Alters 10% of the values of the target object.
	/// </summary>
	/// <param name="target"></param>
	void mutate(SAT_puzzle target);


	SAT_puzzle baseFormula;

	int MAXTIME = 10000;
	vector<bool> flaggedMembers;
	vector<SAT_puzzle> population, children;
};

