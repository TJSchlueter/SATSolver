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

#include "stdafx.h"
#include "SAT_puzzle.h"


/***************************************************
* construct/destruct functions START
***************************************************/

/// <summary>
/// Default constructor; currently does nothing.
/// </summary>
SAT_puzzle::SAT_puzzle()
{
}

/// <summary>
/// Constructor based upon a filename.
/// </summary>
/// <param name="filename">Filename of a SAT formula.
/// </param>
SAT_puzzle::SAT_puzzle(String^ filename)
{
	//If the provided filename does not work, the program will ask the user to supply a different filename or
	// to enter 'q' to quit.
	//If the user quits in this way, the program will display a message and exit.
	bool Not_opened = true, quit = false;
	StreamReader^ in_file;

	try
	{
		Not_opened = false;
		in_file = gcnew StreamReader(filename);
	}
	//If opening fails, then request entry of a new file or a command to quit.
	catch (FileNotFoundException^ open_error)
	{
		Not_opened = true;
		Console::WriteLine("File " + filename + "  could not be opened.");
		throw filename;
	}
	
	if (Not_opened == false)
	{
		String^ line_in = in_file->ReadLine();
		int number_line = 0;
		bool descriptorLine_found = false;

		//loop processes each line from the file and converts it into data useable by the puzzle
		while (line_in != nullptr)
		{
			//records how many lines of numbers have been reached
			array<String^>^ token = line_in->Split(' ');
			
			//Seeks the indicator for the line that describes how many members and clauses there are to
			// satisfy in the SAT problem.
			//The expected files represent cnf formulas in a specific format used in the 2002 SAT Competition.
			//Information on this format can be found at: http://www.satcompetition.org/2004/format-solvers2004.html
			//All other formula-related lines should have no indicator in front of them, while comment lines should
			// be denoted by a lower case 'c'.
			//In this version, a flag has been added to determine if the descriptor line has been found and parsed yet.
			// This is to prevent error lines or non-cnf files from causing problems for the program.
			if (token[0]->Equals("p"))
			{
				//Attempts to parse the descriptor line for the forumula, extracting the number of members and clauses
				try
				{
					num_members = int::Parse(token[2]);
				}
				catch (Exception^ parse_error)
				{
					Console::Write("Parsing error with: ");
					Console::WriteLine(token[2]);
					throw parse_error;
				}
				try
				{
					num_clauses = int::Parse(token[3]);
				}
				catch (Exception^ parse_error)
				{
					Console::Write("Parsing error with: ");
					Console::WriteLine(token[3]);
					throw parse_error;
				}

				//If the numbewr of members and clauses are successfully extracted, problem's member vectors are
				// re-sized to accommodate a sufficient number of variables.
				member_list.resize(num_members);
				clause_list.resize(num_clauses);
				puzzle_ints.resize(num_clauses);
				for (int i = 0; i < num_clauses; i++)
				{
					puzzle_ints[i].resize(num_members + 1);
				}

				//The flag indicating the descriptor line was found and parsed is set to true, allowing subsequent
				// lines to be processed
				descriptorLine_found = true;
			}
			// else if is used here to avoid testing more if statements than necessary
			//There is no else following this, because lines that do not meet either the above or below
			// if statements should not be processed.
			/// <note> Consider adding some sort of error functionality to specifically warn of garbage lines: TJS 1/22/2015
			else if (!token[0]->Equals("c") && descriptorLine_found)
			{
				int offset = 0;
				for (int i = 0; i < (token->Length - offset); i++)
				{
					//Attempts to parse a line into a series of integers which are used to indicate which
					// of the members needs to either be true (positive) or false (negative).
					try
					{
						int z = number_line;
						while (token[i + offset]->Equals(""))
						{
							offset++;
						}
						puzzle_ints[number_line][i] = int::Parse(token[i + offset]);
					}
					catch (IndexOutOfRangeException^ index_error)
					{
						Console::WriteLine(token[i] + " " + i.ToString());
						Console::WriteLine("Index OB.");
						throw index_error;
					}
					catch (Exception^ parse_error)
					{
						Console::Write("Parsing error with: ");
						Console::WriteLine(token[i]);
						throw parse_error;
					}
				}
				number_line++;
			}

			line_in = in_file->ReadLine();
		}

		//To create a base-state a solution is sought from, all member and clause values are set to false
		for (int i = 0; i < num_members; i++)
		{
			member_list[i] = false;
		}
		for (int i = 0; i< num_clauses; i++)
		{
			clause_list[i] = false;
		}
		
	}
}

/// <summary>
/// Constructor, based upon a previously existing SAT_puzzle object.
/// </summary>
/// <param name="base_puzzle"> SAT_puzzle object to be copied.
/// </param>
SAT_puzzle::SAT_puzzle(SAT_puzzle &base_puzzle)
{
	base_puzzle.getNumMembers();
	num_members = base_puzzle.getNumMembers();
	num_clauses = base_puzzle.getNumClauses();

	//copy member_list from the previous object
	vector<bool> member_list_temp = base_puzzle.getMemberList();
	member_list.resize(num_members);
	for (int i = 0; i < num_members; i++)
	{
		bool temp = member_list_temp[i];
		member_list[i] = temp;
	}

	//copy clause_list from the previous object
	vector<bool> clause_list_temp = base_puzzle.getClauseList();
	clause_list.resize(num_clauses);
	for (int i = 0; i < num_clauses; i++)
	{
		bool temp = clause_list_temp[i];
		clause_list[i] = temp;
	}

	//copy puzzle_ints from the previous object
	vector<vector<int>> puzzle_ints_temp = base_puzzle.getPuzzleInts();
	puzzle_ints.resize(num_clauses);
	for (int i = 0; i < num_clauses; i++)
	{
		puzzle_ints[i].resize(num_members);
	}
	for (int i = 0; i < num_clauses; i++)
	{
		for (int j = 0; j < num_members; j++)
		{
			int temp = puzzle_ints_temp[i][j];
			puzzle_ints[i][j] = temp;
		}
	}

	int k = check_trues();
}

/// <summary>
/// Destructor.
/// </summary>
SAT_puzzle::~SAT_puzzle()
{
}

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
int SAT_puzzle::check_trues()
{
	true_clause_count = 0;
	for (int i = 0; i < num_clauses; i++)
	{
		clause_list[i] = false;
	}
	//Iterates through each clause to check the values of the clauses' members
	for (int i = 0; i < num_clauses; i++)
	{
		//Iterates through the members of a clause, checking to see if any of the members satisfy the
		// requirements to make the clause true.
		for (int j = 0; j < num_members; j++)
		{
			//Zero indicates the termination of a clause
			if (puzzle_ints[i][j] != 0)
			{
				int index = puzzle_ints[i][j];
				if (index > 0)
				{
					clause_list[i] = clause_list[i] | member_list[index - 1];
				}
				else
				{
					clause_list[i] = clause_list[i] | !member_list[(-index) - 1];
				}
			}
			else
			{
				j = num_members;
			}
		}
		if (clause_list[i])
		{
			true_clause_count++;
		}
	}
	return true_clause_count;
}

/// <summary>
/// Determines whether or not the formula is satisfied and returns true or false accordingly.
/// </summary>
/// <returns>true if the formula is satisfied or false if it is not.</returns>
bool SAT_puzzle::satisfied()
{
	bool isSatisfied = true;
	for (int i = 0; i < num_clauses; i++)
	{
		isSatisfied = isSatisfied & clause_list[i];
	}
	return isSatisfied;
}

////////////////////////////////////////////////////////////
//Accessors

/// <summary>
/// Gets the list of bool variables used in the formula.
/// </summary>
/// <returns></returns>
vector<bool> SAT_puzzle::getMemberList()
{
	return member_list;
}

/// <summary>
/// Gets the list of clauses used in the formula.
/// </summary>
/// <returns></returns>
vector<bool> SAT_puzzle::getClauseList()
{
	return clause_list;
}

/// <summary>
/// Gets the array of integers representing member indices within each clause
/// </summary>
/// <returns></returns>
vector<vector<int>> SAT_puzzle::getPuzzleInts()
{
	return puzzle_ints;
}

/// <summary>
/// Get the number of member variables in the formula.
/// </summary>
/// <returns></returns>
int SAT_puzzle::getNumMembers()
{
	return num_members;
}

/// <summary>
/// Get the number of clauses in the formula.
/// </summary>
/// <returns></returns>
int SAT_puzzle::getNumClauses()
{
	return num_clauses;
}

////////////////////////////////////////////////////////////
//Mutators

/// <summary>
/// Sets a bool variable of member_list equal to true.
/// </summary>
/// <param name="index"> The index to be set to true.
/// </param>
void SAT_puzzle::setTrueAtIndex(int index)
{
	member_list[index] = true;
}

/// <summary>
/// Sets a bool variable of member_list equal to false.
/// </summary>
/// <param name="index"> The index to be set to false.
/// </param>
void SAT_puzzle::setFalseAtIndex(int index)
{
	member_list[index] = false;
}

/// <summary>
/// Flips the bool value at the given index.
/// </summary>
/// <param name="index">The index to be flipped.</param>
void SAT_puzzle::negateAtIndex(int index)
{
	member_list[index] = !member_list[index];
}

/// <summary>
/// Sets the member at location index equal to inputValue
/// </summary>
/// <param name="index">the index to be changed</param>
/// <param name="inputValue">the value the index is to be set to</param>
void SAT_puzzle::setMemberAtIndex(int index, bool inputValue)
{
	member_list[index] = inputValue;
}

/***************************************************
* member functions END
***************************************************/