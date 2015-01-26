#include "stdafx.h"
#include "Hill_climb.h"

/***************************************************
* public functions START
***************************************************/

/// <summary>
/// Default Constructor.
/// </summary>
Hill_climb::Hill_climb()
{
}

/// <summary>
/// Constructor.
/// </summary>
/// <param name="inPuzzle">SAT_puzzle object to be solved.</param>
Hill_climb::Hill_climb(SAT_puzzle inPuzzle)
{
	baseFormula = inPuzzle;

	baseFormula.check_trues();
	bool t = baseFormula.satisfied();
	int k = 0;
}

/// <summary>
/// Runs the hill climb algorithm
/// </summary>
/// <returns>an integer representing success, failure, or timing out.</returns>
int Hill_climb::run_HillClimb()
{
	Stopwatch ^ timeKeeper = gcnew Stopwatch();
	timeKeeper->Start();

	Random ^ randomizer = gcnew Random();
	for (int i = 0; i < baseFormula.getNumMembers(); i++)
	{
		if (randomizer->Next(2) == 1)
		{
			baseFormula.setTrueAtIndex(i);
		}
		else
		{
			baseFormula.setFalseAtIndex(i);
		}
	}

	int index = 1;

	while (index >= 0)
	{
		//check for an index to improve the current solution fitness
		index = bestStep();
		int blah2 = 0;

		//If an index is found, which improves the current solution fitness when the variable
		// at that location is flipped, then that value is flipped.
		//If more than one such index is found, the best among them is flipped.
		if (index >= 0)
		{
			baseFormula.negateAtIndex(index);
			int num_trues = baseFormula.check_trues();
			int blah = 0;
			if (baseFormula.satisfied())
			{
				return -1;
			}
		}
		else
		{
			if (baseFormula.satisfied())
			{
				return -1;
			}
			else { return baseFormula.check_trues(); }
		}
	}

	timeKeeper->Stop();
	if (baseFormula.satisfied())
	{
		return -1;
	}
	else { return baseFormula.check_trues(); }
}

/// <summary>
/// checks to see if the formula has been solved or not.
/// </summary>
/// <returns>returns true if solved, false if not</returns>
bool Hill_climb::formulaSolved()
{
	return baseFormula.satisfied();
}

/// <summary>
/// checks how many clauses in the formula have been satisfied
/// </summary>
/// <returns>an integer equal to the number of satisfied clauses</returns>
int Hill_climb::checkPuzzle()
{
	return baseFormula.check_trues();
}

/***************************************************
* public functions END
***************************************************/

/***************************************************
* private functions START
***************************************************/

/// <summary>
/// Searches for the best index to flip to improve solution fitness.
/// </summary>
/// <returns>the index of the best variable to flip</returns>
int Hill_climb::bestStep()
{
	int index = -1;
	int currentSatisfied = baseFormula.check_trues();

	vector<int> satisfiedClauses;
	satisfiedClauses.resize(baseFormula.getNumMembers());
	for (unsigned int i = -0; i < satisfiedClauses.capacity(); i++)
	{
		satisfiedClauses[i] = 0;
	}

	//determine which variable satisfies the greatest number of clauses when it is flipped
	for (unsigned int i = 0; i < satisfiedClauses.capacity(); i++)
	{
		baseFormula.negateAtIndex(i);
		satisfiedClauses[i] = baseFormula.check_trues();
		baseFormula.negateAtIndex(i);

		if (satisfiedClauses[i] > currentSatisfied)
		{
			index = i;
			currentSatisfied = satisfiedClauses[i];
		}
	}
	return index;
}

/***************************************************
* private functions END
***************************************************/

