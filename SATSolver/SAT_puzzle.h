/* Author: Jacob Schlueter
* Creation Date: January 20, 2015
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
*  This is a C++ version of a project I originally coded in C# in the fall of 2014.
*/


#pragma once
#include <string>
#include <vector>
using namespace std;
using namespace System;
using namespace System::IO;

class SAT_puzzle
{
public:
	
	/***************************************************
	* construct/destruct functions START
	***************************************************/

	/// <summary>
	/// Default constructor; currently does nothing.
	/// </summary>
	SAT_puzzle();

	/// <summary>
	/// Constructor based upon a filename.
	/// </summary>
	/// <param name="filename">Filename of a SAT formula.</param>
	SAT_puzzle(String^ filename);

	/// <summary>
	/// Constructor, based upon a previously existing SAT_puzzle object.
	/// </summary>
	/// <param name="base_puzzle"> SAT_puzzle object to be copied.
	/// </param>
	SAT_puzzle(SAT_puzzle &base_puzzle);

	/// <summary>
	/// Destructor.
	/// </summary>
	~SAT_puzzle();

	/***************************************************
	* construct/destruct functions END
	***************************************************/

	/***************************************************
	* member functions START
	***************************************************/

	/// <summary>
	/// checks to see how many of the puzzle's clauses are satisfied as true.
	/// </summary>
	/// <returns>the number of satisfied clauses</returns>
	int check_trues();

	/// <summary>
	/// Determines whether or not the formula is satisfied and returns true or false accordingly.
	/// </summary>
	/// <returns>true if the formula is satisfied or false if it is not.</returns>
	bool satisfied();

	////////////////////////////////////////////////////////////
	//Accessors

	/// <summary>
	/// Gets the list of bool variables used in the formula.
	/// </summary>
	/// <returns></returns>
	vector<bool> getMemberList();

	/// <summary>
	/// Gets the list of clauses used in the formula.
	/// </summary>
	/// <returns></returns>
	vector<bool> getClauseList();

	/// <summary>
	/// Gets the array of integers representing member indices within each clause
	/// </summary>
	/// <returns></returns>
	vector<vector<int>> getPuzzleInts();

	/// <summary>
	/// Get the number of member variables in the formula.
	/// </summary>
	/// <returns></returns>
	int getNumMembers();

	/// <summary>
	/// Get the number of clauses in the formula.
	/// </summary>
	/// <returns></returns>
	int getNumClauses();

	////////////////////////////////////////////////////////////
	//Mutators

	/// <summary>
	/// Sets a bool variable of member_list equal to true.
	/// </summary>
	/// <param name="index"> The index to be set to true.
	/// </param>
	void setTrueAtIndex(int index);

	/// <summary>
	/// Sets a bool variable of member_list equal to false.
	/// </summary>
	/// <param name="index"> The index to be set to false.
	/// </param>
	void setFalseAtIndex(int index);

	/// <summary>
	/// Flips the bool value at the given index.
	/// </summary>
	/// <param name="index">The index to be flipped.</param>
	void negateAtIndex(int index);

	/// <summary>
	/// Sets the member at location index equal to inputValue
	/// </summary>
	/// <param name="index">the index to be changed</param>
	/// <param name="inputValue">the value the index is to be set to</param>
	void setMemberAtIndex(int index, bool inputValue);

	/***************************************************
	* member functions END
	***************************************************/

private:
	/***************************************************
	* member variables START
	***************************************************/

	vector<bool> member_list; //variables used in the formulas
	vector<bool> clause_list; //clauses results of the formula
	//2D int array describing which member indicies are used in a clause and whether
	// those members are negated or not.  Each row terminates in a 0 to signal the end
	// of the clause.
	vector<vector<int>> puzzle_ints;

	/// <summary>
	/// true_clause_count: the number of clauses that have resolved as true
	/// num_members: the total number of member variables used in the formula
	/// num_clauses: the total number of clauses contained in the formula
	/// </summary>
	int true_clause_count, num_members, num_clauses;

	/***************************************************
	* member variables END
	***************************************************/

};

