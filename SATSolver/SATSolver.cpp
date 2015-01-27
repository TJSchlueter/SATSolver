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

// SATSolver.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SAT_puzzle.h"
#include "Genetic.h"
#include "Hill_climb.h"
#include "WalkSAT.h"
#using <System.dll>
using namespace std;
using namespace System;
using namespace System::IO;
using namespace System::Diagnostics;

/*Struct used to store both the number of clauses satisfied and the
* amount of time that a solve attempt took to complete.*/
struct Solve_results
{
	int satisfied_clauses;
	int time_elapsed;
};

SAT_puzzle loadProblem();
Solve_results runSolver(SAT_puzzle myprob);

int _tmain(int argc, _TCHAR* argv[])
{
	int ret_int = 0;
	Console::WriteLine("Welcome to SAT Solver!");
	bool run_solve_algorithm = true, file_loaded = false, load_new = true;
	SAT_puzzle myprob;
	
	//Loops so long as the user wishes to solve more problems
	while (run_solve_algorithm)
	{
		
		//Only loads a new SAT problem file if the user elects to do so
		if (load_new)
		{
			load_new = false;
			try
			{
				myprob = loadProblem();
				Console::WriteLine("Problem loaded.");
				file_loaded = true;
			}
			catch (String^ thrown_string)
			{
				file_loaded = false;
				Console::WriteLine("Program will now close");
			}
			catch (Exception^ thrown_error)
			{
				file_loaded = false;
				Console::WriteLine("Program will now close");
			}
		}

		//The program will only attempt to solve the problem if all information is successfully loaded.
		if (file_loaded)
		{
			Stopwatch ^ timeKeep = gcnew Stopwatch();

			Solve_results results = runSolver(myprob);
			if (results.satisfied_clauses == -1)
			{
				Console::WriteLine("SATISFIABLE");
				Console::WriteLine("milliseconds elapsed: " + results.time_elapsed.ToString());
				Console::WriteLine("clauses satisfied: " + myprob.getNumClauses().ToString());
			}
			else if (results.satisfied_clauses == -2)
			{
				Console::WriteLine("Solve attempt aborted.");
			}
			else
			{
				Console::WriteLine("UNSATISFIABLE");
				Console::WriteLine("milliseconds elapsed: " + results.time_elapsed.ToString());
				Console::WriteLine("clauses satisfied: " + results.satisfied_clauses.ToString());
			}
			ret_int = 0;
		}
		else
		{
			ret_int = 1;
		}

		String ^ response = " ";

		// Check to see if the user wishes to run a solving algorithm again
		while (response != "y" && response != "Y" && response != "n" && response != "N")
		{
			Console::WriteLine("Would you like to run the solver again? y/n:");
			response = Console::ReadLine();
			if (response == "y" || response == "Y")
				run_solve_algorithm = true;
			else if (response != "n" || response != "N")
				run_solve_algorithm = false;
			else
				Console::WriteLine(response + " is not a valid response.  Please enter y or n.");
		}
		response = " ";
		// Check to see if the user wants to load a different problem to attemp to solve.
		while (response != "y" && response != "Y" && response != "n" && response != "N" && run_solve_algorithm)
		{
			Console::WriteLine("Would you also like to use a different problem? y/n:");
			response = Console::ReadLine();
			if (response == "y" || response == "Y")
				load_new = true;
			else if (response != "n" || response != "N")
				load_new = false;
			else
				Console::WriteLine(response + " is not a valid response.  Please enter y or n.");
		}
	}
	Console::ReadLine();
	return ret_int;
}

/// <summary>
/// Loads a SAT problem from a user-selected file.
/// </summary>
SAT_puzzle loadProblem()
{
	Console::WriteLine("Please enter the filename of the SAT problem you wish to solve:");
	String^ filename = Console::ReadLine();
	SAT_puzzle myprob;

	bool open = false;
	while (!open)
	{
		//Attempt to load SAT problem information from the provided filename.
		//If information cannot be loaded, the program will exit.
		try
		{
			myprob = SAT_puzzle(filename);
			open = true;
		}
		catch (String^ thrown_string)
		{
			Console::WriteLine("File could not be opened.");
			Console::WriteLine("Enter a different filename or q to quit.");
			filename = Console::ReadLine();
			if (filename == "q" || filename == "Q")
			{
				throw thrown_string;
			}
		}
		catch (Exception^ thrown_error)
		{
			Console::WriteLine("A problem was encountered extracting contents from the file.");
			Console::WriteLine("Enter a different filename or q to quit.");
			filename = Console::ReadLine();
			if (filename == "q" || filename == "Q")
			{
				throw thrown_error;
			}
		}
	}

	return myprob;
}

/// <summary>
/// Has the user select the desired solving algorithm, then runs the algorithm on the
/// given problem.
/// </summary>
/// <param name="myprob">SAT problem to be solved.</param>
Solve_results runSolver(SAT_puzzle myprob)
{
	Stopwatch ^ timeKeep = gcnew Stopwatch();
	Solve_results Solver_results;
	bool complete = false;

	Console::WriteLine("Please select a SAT-solving method from the following:");
	Console::WriteLine("0: Genetic evolution");
	Console::WriteLine("1: Hill-climbing search");
	Console::WriteLine("2: WalkSAT search");
	Console::WriteLine("q: Quit the program");
	String ^ selection = Console::ReadLine();

	// Check user input to see which of the aforementioned options was chosen
	// If none of the above are chosen, the user is prompted to enter a valid selection
	while (!complete)
	{
		if (selection == "q" || selection == "Q")
		{
			Solver_results.satisfied_clauses = -2;
			Solver_results.time_elapsed = 0;
			complete = true;
		}
		else
		{
			int choice;
			try
			{
				choice = int::Parse(selection);
			}
			catch (Exception^ thrown_error)
			{
				choice = -1;
			}

			Genetic GeneSolver = Genetic();
			Hill_climb HCSolver;
			WalkSAT WalkSolver;
			timeKeep->Start();
			switch (choice)
			{
			case 0:
				Solver_results.satisfied_clauses = GeneSolver.runGenetic(myprob);
				complete = true;
				break;
			case 1:
				HCSolver = Hill_climb(myprob);
				Solver_results.satisfied_clauses = HCSolver.run_HillClimb();
				complete = true;
				break;
			case 2:
				WalkSolver = WalkSAT(myprob);
				Solver_results.satisfied_clauses = WalkSolver.run_WalkSAT();
				complete = true;
				break;
			default:
				Console::WriteLine("That is not a valid selection.");
				Console::WriteLine("Please enter a valid selection.");
				selection = Console::ReadLine();
				break;
			}
			timeKeep->Stop();
			Solver_results.time_elapsed = timeKeep->ElapsedMilliseconds;
		}
	}
	
	return Solver_results;
}

