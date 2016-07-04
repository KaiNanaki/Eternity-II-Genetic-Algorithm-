//////////////////////////////////////////////////////////////////////////
// Name:		Kris Stevenson
// ID:			07032044
// Project:		Eternity II Genetic Algorithm (EII GA)
// Supervisor:	Dave Harrison
// Date:		07/04/2011
// Updated:		26/04/2016
//////////////////////////////////////////////////////////////////////////


// A Genetic Algorithm designed to attempt to solve the "Eternity II" puzzle, a large NP-Complete
// combinatorial problem. (Last altered date at university, 07th April 2011. Additions made April 2016)


// About the Eternity II puzzle
// Eternity II (EII) is an NP-Complete signed edge matching puzzle released in 2007. It involves 
// the placement of 256 unique square pieces onto a 16 x 16 grid; each puzzle square is spilt 
// twice diagonally into 4 coloured triangles which each contain half of an individually coloured shape.
// These must match up with an identical triangle/shape on the adjacent piece’s edge. The border and 
// corner pieces contain triangles that are grey with no shape in them; these edges must be 
// adjacent to the board edge.
// To date no complete solution has been found. This is not surprising considering there are at most 
// 256! × 4 to the 256th power possible solutions (or 1.15×10 to the 661th power possible 
// combinations). As such an exhaustive search method was decided against and a Genetic Algorithm 
// was used in hopes that its heuristic properties would allow the puzzle to be solved in a much 
// shorter space of time.

// UPDATED (April 2016)
// I have updated the EII GA in 2016 by added further commenting, making minor improvements to the code
// and general tidying of the program. 
// Improvements include: 
// - Formatted data displayed in window to make it more readable.
// - Adding the ability to display the fittest chromosome on screen in text format.
// - Added window resizing so the on screen chromosome can be more easily seen.
// - Adding the option to pause the GA so this chromosome/GA data can been checked if the data is
// streaming down the window too quickly.
// - Redesigning the fitness subprogram to be more efficient and to check only hard constraints.
// - Altering how the fittest chromosome/generation text files are saved to make them more readable.


#include <iostream>					// Standard I/O lib	
#include "stdlib.h"					// Standard Lib

// Windows includes
#define _WIN32_WINNT 0x0501			// Windows version header. Program is running on XP or higher.
#include <windows.h>				// Used for resizing/moving console window in Windows.
#include "conio.h"					// MS DOS Lib, used to detect input without halting/blocking
									// the program. (Illustrative uses only, not required for 
									// normal program use.)

// Class includes
#include "DataStructures.h"			// EII GA classes/subprograms
#include "FileLoader.h"				// File Loader subprograms

using namespace std;				// Standard namespace



void main()
{
	// Get window handle and set window position/size
	HWND consoleHandle = GetConsoleWindow() ;
	MoveWindow(consoleHandle, 0, 0, 800, 570, TRUE) ;


	srand((unsigned)time(NULL)) ;			// Seed for rand()
	cout.precision(4) ;						// Set precision to 4 significant digits
	cout << showpoint ;						// Show significant digits after decimal point (used 
											// to format whole numbers)

	int		saveCounter		= 0 ;			// Counter for saving
	float	storedFitness	= 0.0 ;			// Stores highest fitness, used in saving fittest chromosome
	
	// Create and initialize the EII GA population
	cout << "Kris Stevenson - 07032044\n"
		 << "Eternity II Genetic Algorithm (EII GA - 36 piece) 2010-2016\n" << endl ;

	Population	eiiPopulation ;				// Create the EII Puzzle population
	cout << "Population of " << eiiPopulation.getMaxPopulation() << " created" << endl ;

	eiiPopulation.initialisePopulation() ;	// initialize the population

	//loadSolution(&eiiPopulation) ;  // Testing only - used to test a solution

	// Randomise each chromosomes piece locations	
	createRandomPopulation(&eiiPopulation) ;
	
	// Determine Population fitness
	eiiPopulation.determinePopulationfitness() ;


	// Output highest and average fitness of the population
	cout << "Initial population highest fitness: " 
		<< eiiPopulation.getMaxPopulationFitness()	<< endl ;
	cout << "Initial population average fitness: " 
		<< eiiPopulation.getAveragePopulationFitness()	<< endl ;

	// Pause GA until user is ready to continue
	cout << "\nGA Ready..."	<< endl ;
	system("PAUSE") ;


	// Main loop
	// Loops until 100% fitness or a certain number of generations is reached
	while(eiiPopulation.getMaxPopulationFitness() < 100.0
		|| eiiPopulation.numberOfGenerations < 10000)
	{
		// Select parents and breeds them using the crossover subprogram
		eiiPopulation.selection() ;

		// Scan though each of the child chromosomes and fix duplicate or missing pieces
		eiiPopulation.scanAndFix() ;

		// Replace current population with their offspring
		eiiPopulation.replacePopulation() ;

		// Randomly apply mutation to the new offspring
		eiiPopulation.mutation() ;

		// Can be used to introduce a greater amount of random chromsomes into the population
		// to try and delay convergance
		//introduceRandomChromosomes(&eiiPopulation) ;

		// Determine each chromosome's fitness
		eiiPopulation.determinePopulationfitness() ;

		// Determine the maximum and average population fitness
		eiiPopulation.getMaxPopulationFitness() ;
		eiiPopulation.getAveragePopulationFitness() ;

		// Display current generation number along with maximum and average fitness values
		// for this generation
		cout << "\nGeneration: "	<< eiiPopulation.numberOfGenerations 
			 << "\nMax fitness: "	<< eiiPopulation.maxPopulationFitness 
			 << "\tAve fitness: "	<< eiiPopulation.avePopulationFitness << endl ;


		// Save the fittest chromosome every x generations
		if (saveCounter == 500)
		{
			// Save fittest chromosome if previously stored fitness is lower
			if (eiiPopulation.getMaxPopulationFitness() > storedFitness)
			{
				saveFittestChromosome(&eiiPopulation) ;
				cout << "\nFittest chromosome saved at generation " 
					 << eiiPopulation.numberOfGenerations << "." << endl ;
				storedFitness = eiiPopulation.maxPopulationFitness ;
			}
			// If not as fit as previous chromosome, do not save 
			else
			{
				cout << "\nMax fitness chromosome not as fit as last save."
					 << "\nNo new chromosome save created at generation "
					 << eiiPopulation.numberOfGenerations << "." << endl ;
			}
			saveCounter = 0 ;					// Reset save counter
			//system("PAUSE") ;
		}

		// Save the population's max fitness value for this generation
		saveFitnessData(&eiiPopulation) ;

		// Print fittest chromosome on screen
		printFittestChromosome(&eiiPopulation) ;
		
		eiiPopulation.numberOfGenerations++ ;	// Increment generation count
		saveCounter++ ;							// Increment save counter

		// Checks for keypress and pauses the GA
		// Not part of the ANSI C/C++ standard, may not compile on non windows units. Safe to 
		// comment out if needs be as this is only used to pause the GA for illustrative purposes.
		if (_kbhit())
		{
			cout << "Pausing Eternity II Genetic Algorithm (36 Piece)..." << endl ;
			system("PAUSE") ;
		}
	}// end while

	// End of GA, display final details and save fittest chromosome
	cout << "\n\nEII GA has found a solution! Generation: " << eiiPopulation.numberOfGenerations << "!"
		 << "\nMax fitness: " << eiiPopulation.maxPopulationFitness 
		 << "\tAve fitness: " << eiiPopulation.avePopulationFitness << endl ;


	// Save and then print fittest chromosome
	saveFittestChromosome(&eiiPopulation) ;
	printFittestChromosome(&eiiPopulation) ;

	cout << "\nCheck 'GASave - Fittest Chromosome.txt' for Fittest Chromosome Data." << endl ;
	cout << "Check 'GASave - Fitness Data.txt' for fitness data for each generation.\n" << endl ;

	system("PAUSE") ;

}//end main()