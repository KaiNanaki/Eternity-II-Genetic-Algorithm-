#ifndef _DATASTRUCTURES_
#define _DATASTRUCTURES_

#include <iostream>		// Standard input/output libs
#include <time.h>		// Used to seed random numbers

enum	pieceType	{ Corner, Border, Inner } ;	// type of piece

// PuzzleType/PuzzlePiece attributes and subprograms
// The EII GA stores the information for the Eternity II puzzle pieces inthe PuzzlePiece class. 
// The pieces themselves are squares with their unique piece number on the reverse. On the 
// front surface they are divided twice diagonally into 4 triangles. Each triangle has a 
// background colour, and half of a differently coloured shape within it, with the exception of
// the unique grey triangles which are used to match up against the edge of the puzzle board. 
// Since there are only a small number of combinations of colours and shapes within the 256 piece
// puzzle (23 including the grey edge type), each unique combination was given an interger value. 
// These will be referred to as a piece’s “Edge Type” and will be used in the comparison between 
// piece edges and also to identify the piece’s type. Each edge type and their respective values 
// are shown below. A visual representation of each edge type can been seen in the 
// "EdgeTypeLegend.png" file.

// Edge Piece Types (256 piece puzzle):
//
// Edge type number, Background colour, Foreground colour and shape
// 
// Type 1, grey
// Type 2, orange, with light blue curvy cross 
// Type 3, pink, with light blue diamond mirror 
// Type 4, green, with dark blue nut 
// Type 5, dark blue, with yellow flower 
// Type 6, pink, with yellow bud cross 
// Type 7, dark blue, with orange bud cross  
// Type 8, green, with pink bud cross  
// Type 9, purple, with light blue spikes 
// Type 10, green, with orange spikes 
// Type 11, light blue, with pink spikes 
// Type 12, purple, with yellow circle cross 
// Type 13, dark blue, with pink circle cross 
// Type 14, mauve, with orange star cloud 
// Type 15, maroon, with light blue curvy cross 
// Type 16, light blue, with pink castle 
// Type 17, yellow, with dark blue castle 
// Type 18, pink, with yellow castle 
// Type 19, yellow, with green hollow diamond
// Type 20, dark blue, with ight blue hollow diamond 
// Type 21, yellow, with light blue star
// Type 22, mauve, with yellow star
// Type 23, orange, with purple star
// 



class PuzzlePiece
{
private:

public:

	pieceType	type ;			// Type of piece
	int			number ;		// The physical piece number
	int			rotation ;		// 0, 90, 180, and 270 rotation. Default: 0
	int			topEdge ;		// Edge types, 1-23. default: 0
	int			rightEdge ;		// Edge types, 1-23. default: 0
	int			bottomEdge ;	// Edge types, 1-23. default: 0
	int			leftEdge ;		// Edge types, 1-23. default: 0
	bool		movable ;		// Piece can be moved? Default: true

	// Used to initialise and set piece values (including the updating of edge valyes by rotation)
	// from the puzzle peice data file(s)
	void setInitialPiece( pieceType	_pType,	int		_pNum, 
						  int		_pRot,	int		_top, 
						  int		_right,	int		_bottom, 
						  int		_left,	bool	_movable) ;

	// Used to set a piece's rotation (Used within setInitialPiece())
	void setInitialRotation() ;

	// Increments a piece's rotation by an extra 90 degrees and moves the edge values on each piece
	// accordingly
	void updateRotation() ;

	// Used to set  piece values (as in setInitialPiece()) within scanAndFix(), mutation(), and 
	// replacePopulation() subprograms
	// HOWEVER, it is not used when a piece's rotation needs to be updated. 
	// The piece data has already been modified by its rotation value upon loading/saving or by
	// the mutation() subprogram, therefore setPiece() does NOT rotate the edge values of the 
	// piece/solution as this would cause the data to be altered from its current/saved form.
	void setPiece( pieceType	_pType,		int		_pNum,
				   int			_pRot,		int		_top, 
				   int			_right,		int		_bottom,
				   int			_left,		bool	_movable) ;

} ;//end Class PuzzlePiece



// The Chromosome class is composed of an array of PuzzlePieces (the chromosome's genes), a 
// static const value for the number of max pieces within the array, and the numbers of rows and 
// columns on the puzzle board. A float value is used to store the fitness of each chromosome as 
// a percentage. This class represents a candidate solution to the puzzle with each array position
// (0 - MAX_PIECES) representing a position on the puzzle board (referred to as positional 
// representation).
class Chromosome
{
private:
	
public:

	static const int	MAX_PIECES	= 36 ;	// Number of puzzle pieces/genes
	static const int	rows		= 6 ;	// Number of rows on the puzzle board
	static const int	columns		= 6 ;	// Number of columns on the puzzle board
	float				chromosomeFitness ;	// Stores fitness value as a percentage

	PuzzlePiece			piece[MAX_PIECES] ;	// Array of puzzle pieces (a chromosome)
	

	// Fitness Subprogram: Determines this chromosome's fitness value
	// For a more in depth explination of what this subprogram does, see 
	// 'determinePopulationfitness()' subprogram notes
	float	determineChromosomeFitness() ;

	int		getMaxPieces() ;
	float	getChromosomeFitness() ;

} ;//end Class Chromosome


// The population class is composed of a collection of chromosomes. This class consists of two 
// arrays of chromosomes that will be created on the stack for greater speed processing the large
// amount of data over hundreds/thousands of generations. 
// The first array makes up the main population, the second is a temporary population which is 
// used for storing the most recently bred offspring. The maximum and average fitness of the main
// population can also be stored within floats. An integer variable allows for a count to be kept
// of the number of generations passed. Lastly, there are static const integers to hold the 
// maximum population size (used in creating the mainPopulation and tempPopulation arrays), and
// the mutation rate (which is used in the mutation subprogram).
class Population
{
public:
	static const int	MAX_POPULATION	= 40000 ;				// Maximum population 
																// (MUST be a multiple of 4) e.g. 1,000
	static const int	MUTATION_RATE	= MAX_POPULATION* 0.33 ;// Number of mutations per generation (0.17)(0.31)

	int					numberOfGenerations ;					// Keeps count of the number of generations
	float				avePopulationFitness ;					// Holds the populations average fitness
	float				maxPopulationFitness ;					// Holds the populations maximum fitness

	Chromosome			mainPopulation[MAX_POPULATION] ;		// Main GA population
	Chromosome			tempPopulation[MAX_POPULATION] ;		// Temporary population for storing offspring


	// Initialises the main and temp population
	// Sets all values to defaults.
	void initialisePopulation();


	// selection() Subprogram: tournament selection (random)
	// Uses tournament selection to choose parent chromosomes for breeding. The selection process
	// takes two random chromosomes from the unsorted mainPopulation array. The chromosome fitness
	// value of each parent is compared and the fitter chromosome of the two is selected as the 
	// first parent. The second parent is found using the same process.
	// Besides the fact this method is fast and efficient, the main reasons for using this random 
	// selection method is that it does not guarantee that the fittest chromosome within the 
	// population will be used as a parent (elitism). As parents are randomly selected each 
	// generation, this method was used to attempt to keep the population as diverse as possible 
	// and to reduce convergence.
	// The selection process continues until a complete population of offspring of size 
	// 'MAX_POPULATION' is created. 
	void selection() ;


	// Breeding Subprogram (Uses two point crossover)
	// crossover() is used within the selection() subprogram to simplify the GA. It was decided 
	// that extra arrays, and the calculations to set, select and breed chromosomes, would 
	// complicate the process. Therefore offspring are bred when each pair of parents are initially
	// chosen. To keep track of newly bred offspring using this method, an integer variable 
	// 'populationPosition' is used and passed in to the breeding subprogram so offspring can be 
	// placed in the correct position within 'tempPopulation'.
	// Once the parent chromosomes and 'populationPosition' have been passed into the breeding 
	// subprogram, parent information is copied into tempPopulation. This information is copied 
	// from populationPosition up to populationPosition+1, the reason for which is that two 
	// offspring are created from a pair of parents. When this is complete crossover begins. 
	// The EII GA uses two point crossover to breed offspring; two random points are selected and 
	// the information from the respective parent is copied into each position between the 
	// crossover points, two new chromosomes are therefore created. Two point crossover was used 
	// as single point crossover’s ability to produce diverse offspring as been criticized.
	void crossoverTwoPoint( Chromosome	_chromosome1,	
							Chromosome	_chromosome2, 
							int			_populationPosition ) ;


	// Mutation Subprogram: Randomly alters chromosome values to mimic mutation. 
	// (Ideal rate for EII GA is 10-30% of max population, MUTATION_RATE value can be changed above)
	// Used after scanAndFix() once the new generation of offspring have been created and errors 
	// fixed. Every chromosome in the new generation of offspring has a chance of their genes 
	// being mutated. 
	// There are two sections in the mutation subprogram: Rotate and Swap
	// Rotate: A random chromosome is chosen, followed by a random puzzle piece within that 
	// chromosome. The rotation of that piece is then adjusted. This process is repeated until 
	// the value stored in MUTATION_RATE is reached. 
	// Swap: Involves the random mutation of puzzle piece locations (is a form of internal shuffle)
	// This uses a similar method to the rotate mutation. Once again a random chromosome is 
	// selected from the population. Two random pieces are selected from that chromosome and their 
	// data is swapped. 
	// Both of these mutation methods introduce new candidate solutions into the population, 
	// potentially allowing the population’s fitness level to rise even after it has converged.
	void mutation() ;


	// scanAndFix() subprogram to correct erroneous chromosome data
	// It was decided early on in the project to implement a ‘true’ GA and use crossover. This 
	// however, brought about the duplication and deletion problem therefore scanAndFix() was 
	// implemented within the Population class to scan for corrupted chromosomes and fix the data 
	// within them.
	// Two arrays are used to keep track of pieces that are already used/missing/duplicates. 
	// These arrays are 'usedPieces' and 'duplicatePieces', both are set to their default values
	// before each chromosome is checked.
	// The first section, "scan"  checks each piece number and notes pieces used. If a duplicate
	// piece is found, it infers a piece must also be missing and a Boolean flag is set so that 
	// this chromosome will be fixed. 
	// If flagged to be fixed, the chromosome will enter the next stage of the algorithm. Here the
	// first missing piece within the chromosome is determined (by which pieces within 'usedPieces'
	// array are false) and then each piece is randomly looped through until one of the previously
	// flagged duplicate pieces is found. This duplicate piece data is then replaced with the 
	//missing piece data taken from the mainPopulation array as this data is known to be correct.
	// The flag for the duplicate pieces and pieces that are not used (and therefore missing) are 
	// updated and the fix process loops through the chromosome again to determine if there are 
	// any more missing pieces.
	void scanAndFix() ;


	// Replace the mainPopulation with the new population created within tempPopulation during the
	// selection/crossover (breeding) process
	void replacePopulation() ;


	// Determines the fitness of each chromosome (solution) within the main population by assessing 
	// the constraint values set within the subprogram. Checks are carried out for each piece on the
	// board and their respective positions checked for matching constraints.
	// Constraint values typically inflict penalties to a fitness value, with a low fitness value
	// being the desired outcome. The EII GA however, uses these constraint values in the opposite
	// fashion, with values awarded for meeting certain conditions. This method was used as 
	// reaching 100% fitness for a complete solution seemed more logical than striving for a value
	// of zero.
	// At the end of the fitness subprogram the fitness of the chromosome is determined by 
	// multiplying the constraint variables by their respective values/weights. Inner piece types 
	// have their edges calculated in two separate variables innerRowEdges and innerColumnEdges 
	// as this made it easier to debug and test the fitness subprogram during implementation. 
	// (The fitness of each chromosome is saved within the chromosome itself)
	void determinePopulationfitness() ;

	// Return various values. Mostly used to display data on screen.
	float getAveragePopulationFitness() ;
	float getMaxPopulationFitness() ;
	int getMaxPopulation() ;

};//end class Population


#endif