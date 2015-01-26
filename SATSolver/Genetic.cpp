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

#include "stdafx.h"
#include "Genetic.h"

/// <summary>
/// Constructor
/// </summary>
Genetic::Genetic()
{
	population.resize(20);
	children.resize(40);
}

/// <summary>
/// Driving method that performs most of the Genetic algorithm functionality/
/// </summary>
/// <param name="in_puzzle">SAT formula to be solved</param>
int Genetic::runGenetic(SAT_puzzle inPuzzle)
{
	Random ^ rand = gcnew Random();
	vector<vector<int>> fitnessList;
	fitnessList.resize(20);
	for (int i = 0; i < 20; i++)
	{
		fitnessList[i].resize(2);
	}
	int bestFitness = 0;
	//generate starting population
	for (int i = 0; i < 20; i++)
	{
		population[i] = SAT_puzzle(inPuzzle);
		for (int j = 0; j < population[i].getNumMembers(); j++)
		{
			if (rand->Next(2) == 1)
			{
				population[i].setTrueAtIndex(j);
			}
			else
			{
				population[i].setFalseAtIndex(j);
			}
		}
		fitnessList[i][0] = population[i].check_trues();
		fitnessList[i][1] = i;
	}

	//run for 200 generations
	for (int generation = 0; generation < 200; generation++)
	{
		//check for a solution
		for (int i = 0; i < 20; i++)
		{
			if (population[i].check_trues() > bestFitness)
			{
				bestFitness = population[i].check_trues();
			}
			if (population[i].satisfied())
			{
				return -1;
			}
		}


		int fitness = 0;
		int leastFit = 0;
		for (int i = 0; i < 20; i++)
		{
			fitnessList[i][0] = -i;
			if (fitnessList[i][0] < fitnessList[leastFit][0])
				leastFit = i;
		}
		//make children from parents
		for (int i = 0; i < 40; i++)
		{
			int d = rand->Next(20);
			int m = rand->Next(20);
			while (m == d)
			{
				m = rand->Next(20);
			}
			children[i] = makeChild(population[d], population[m]);
			fitness = children[i].check_trues();

			//check to see if the child will join the new population by comparing 
			// its fitness to that of others
			for (int j = 0; j < 20; j++)
			{
				if (fitness > fitnessList[j][0])
				{
					fitnessList[leastFit][1] = i;
					fitnessList[leastFit][0] = fitness;
				}
			}
		}

		for (int i = 0; i < 20; i++)
		{
			population[i] = children[fitnessList[i][1]];
		}
		mutate(population[rand->Next(20)]);
		mutate(population[rand->Next(20)]);
	}
	//check for a solution
	for (int i = 0; i < 20; i++)
	{
		if (population[i].check_trues() > bestFitness)
		{
			bestFitness = population[i].check_trues();
		}
		if (population[i].satisfied())
		{
			return -1;
		}
	}
	return bestFitness;
}

/// <summary>
/// Create a child SAT_puzzle object
/// </summary>
/// <param name="dad">Parent node 1</param>
/// <param name="mom">Parent node 2</param>
/// <returns>child node, combining parent node values randomly</returns>
SAT_puzzle Genetic::makeChild(SAT_puzzle dad, SAT_puzzle mom)
{
	Random ^ rand = gcnew Random();
	vector<bool> dad_members = dad.getMemberList();
	vector<bool> mom_members = mom.getMemberList();
	SAT_puzzle kid = SAT_puzzle(mom);

	for (int i = 0; i < kid.getNumMembers(); i++)
	{
		if (rand->Next(2) == 1)
		{
			kid.setMemberAtIndex(i, mom_members[i]);
		}
		else
		{
			kid.setMemberAtIndex(i, dad_members[i]);
		}
	}

	return kid;
}

/// <summary>
/// Alters 10% of the values of the target object.
/// </summary>
/// <param name="target"></param>
void Genetic::mutate(SAT_puzzle target)
{
	Random ^ rand = gcnew Random();
	int len = target.getNumMembers() / 10;
	for (int i = 0; i < len; i++)
	{
		target.negateAtIndex(rand->Next(target.getNumMembers()));
	}
}