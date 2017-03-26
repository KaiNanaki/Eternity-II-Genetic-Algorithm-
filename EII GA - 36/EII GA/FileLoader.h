#ifndef _FILELOADER_
#define _FILELOADER_

#include <iostream>			// Used for input/output
#include <fstream>			// Used for file loading/saving
#include <time.h>			// Used for the seeding of random numbers
#include "DataStructures.h" // Include Chromosome data structures header 

// File Loader Subprograms
// The variables for the main population (which are each of the PuzzlePiece variables within each
// Chromosome) are set by a combination of loading in values from a text data file and from 
// random number generation. This was done to allow for the easy adjustment of the EII GA when 
// switching from the 36 piece puzzle over to the 256 piece puzzle, and to allow for the saving
// and loading of previous solutions that can be saved by the GA. 
// The data file (36Data.txt/256Data.txt) contains a list of each piece within the puzzle and 
// their respective values.



// loadSolution() initialises each chromosome using the values held in the data file. 
// It is essential that the data file holds correct data for the puzzle pieces as the 
// file loader subprogram does not do any form of error checking on the data loaded in.
void loadSolution( Population *_population ) ;

// createRandomSolution() is very similar to loadSolution(). 
// Once values are set however, the array positions and chromosome values are randomised to create
// a new solution. 
void createRandomSolution( Chromosome *_mainPopulation) ;

// createRandomPopulation() loops through the entire population and uses createRandomSolution()
// to randomise chromosome data values in order to create a diverse population the GA can use.
void createRandomPopulation( Population *_population ) ;

void introduceRandomChromosomes( Population *_population ) ;

// The subprograms SaveFittestChromosome() and SavePopulationFitness() can be used in conjunction
//with seperate data files to save particular data values within the GA.

// saveChromosome() saves the data of the chromosome passed to it. The file 
// "GASave - Fittest Chromosome.txt" is opened/created and the data copied into it.
void saveChromosome( Chromosome *_chromosome ) ;

// saveFittestChromosome() subprograms almost same way as saveChromosome. However the entire 
// population is looped through to find the first chromosome with the highest fitness. This is
// then saved into "GASave - Fittest Chromosome.txt".
void saveFittestChromosome( Population *_population ) ;

void printFittestChromosome( Population *_population ) ;
void saveFitnessData( Population *_population ) ;

#endif