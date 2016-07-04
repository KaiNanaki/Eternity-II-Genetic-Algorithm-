//#include <iostream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "FileLoader.h"
#include "DataStructures.h"

extern enum	piecetype ;//	{ Corner, Border, Inner } ;	// Type of piece

using namespace std ;


void loadSolution( Population *_population )
{
	// Reads puzzle peice data from a file and transfers it into a chromosome in the population class

	// Piecetype, BoardPosition(index), Piece Number, Edge1, Edge2, Edge3, Edge4, Rotation
	char		type ;
	int			index, number, edge1, edge2, edge3, edge4, rotation ;

	ifstream	solutionFile ;					// Used to get output from/input into files
	static int	puzzlePieceLoadCount  = 0 ;		// Keeps tall of number of pieces loaded from file

	//solutionFile.open("ErrorSolutionFor36.txt") ;	// Use to test fitness subprogram has no errors
	solutionFile.open("100SolutionFor36.txt") ;	// Use to test fitness, 100% complete solution
	//solutionFile.open("36Data.txt") ;				// Piece data for 36 piece puzzle
	//solutionFile.open("256Data.txt") ;			// Piece data for 256 piece puzzle
	//solutionFile.open("GASave - Fittest Chromosome.txt") ; // Use to test fitness of previously saved solution


	if ( solutionFile.is_open() )
	{
		cout << "Solution file opened" << endl ;
		while ( solutionFile.eof() == false )
		{
			
			//Load file contents into specified variables
			solutionFile >> type ;
			solutionFile >> index ;
			solutionFile >> number ;
			solutionFile >> edge1 ;
			solutionFile >> edge2 ;
			solutionFile >> edge3 ;
			solutionFile >> edge4 ;
			solutionFile >> rotation ;

			// Format:
			// Piecetype, index(BoardPosition), Piece Number, Rotation 
			// Edge1, Edge2, Edge3, Edge4, Movable(bool)
			switch (type)
			{
			case 'C':	
				//Corner piece (type = 0)
				_population->mainPopulation->piece[index].setPiece(Corner, number, rotation, edge1, edge2, edge3, edge4, true) ;
				break ;
			case 'B':	
				//Boarder piece (type = 1)
				_population->mainPopulation->piece[index].setPiece(Border, number, rotation, edge1, edge2, edge3, edge4, true) ;
				break ;
			case 'I':	
				//Inner piece (type = 2)
				_population->mainPopulation->piece[index].setPiece(Inner, number, rotation, edge1, edge2, edge3, edge4, true) ;
				break ;
			default:
				//#(a comment) or other invalid values
				break ;
			}//end switch

			// Increment puzzlePieceLoadCount so number of pieces loaded can be displayed later
			puzzlePieceLoadCount++ ;
		}//end while
		cout << puzzlePieceLoadCount << " Puzzle pieces loaded" << endl ;
		cout << "File loading complete" << endl ;
		solutionFile.close() ;
		cout << "Solution file closed" << endl ;
	}//end if
	else
	{
		cout << "Unable to open file!" << endl ;
		system("PAUSE") ;
	}
}//end loadSolution()



void createRandomSolution( Chromosome *_mainPopulation )
{
	//# 36 Piece Clue Puzzle Data Info
	//
	//# Format
	//# Piecetype BoardPosition Piecenumber edge1 edge2 edge3 edge4 rotation
	//
	//# Piecetypes: 
	//# C (Corner), B (Boarder), I(Inner)
	//
	//# Piecenumber: 
	//# 1-36
	//
	//# Edgetypes: 
	//# 01 grey
	//# 02 green with blue cog
	//# 03 pink with blue diamond cog
	//# 04 blue with yellow flower
	//# 05 yellow with blue star
	//# 06 orange with blue spikey cross
	//# 07 purple with blue star
	//# 08 pink with yellow bud cross

	char		type ;						// Stores piece's type
	int			index,						// Stores index (board position)
				Tempindex,					// Storage of temporary index (used to 
											// randomise piece placement on board).
				number,						// Stores piece's number
				edge1, edge2, edge3, edge4, // Stores piece's edge type
				rotation ;					// Stores piece's rotation
	bool		usedPosition[36] ;			// Stores if a board/chromosome position is used
	ifstream	solutionFile ;				// Storage for the solution file


	solutionFile.open("36Data.txt") ;		// 36 piece data file containing puzzle pieces (in numerical order)
	//solutionFile.open("256Data.txt") ;	// 256 piece data file containing puzzle pieces (in numerical order)

	Tempindex = 0;
	// Intialise boolean array used for determining if a random position is already taken
	for (int i = 0 ; i < _mainPopulation->getMaxPieces() ; i++)
	{
		usedPosition[i] = false ;
	}

	if ( solutionFile.is_open() )
	{
		//cout << "Solution file opened" << endl ;
		while ( solutionFile.eof() == false )
		{
			// Load file contents into specified variables
			solutionFile >> type ;
			solutionFile >> index ;
			solutionFile >> number ;
			solutionFile >> edge1 ;
			solutionFile >> edge2 ;
			solutionFile >> edge3 ;
			solutionFile >> edge4 ;
			solutionFile >> rotation ;

			// To make each chromosome random, their piece's rotation and index(BoardPosition)
			// are randomised

			// Randomise tempindex(tempBoardPosition) and check if it is used. If so, repeat until 
			// an unused position is found
			do
			{
				Tempindex = rand()%_mainPopulation->getMaxPieces() ;
				//cout << "Try position " << Tempindex << endl ;
			}while(usedPosition[Tempindex] == true) ;

			// Set index(BoardPosition) when the position is free 
			index = Tempindex ;
			usedPosition[Tempindex] = true ;
			//cout << "Position " << index << " free, position set." << endl ;

			// Randomise rotation
			rotation = rand()%4 ;

			// Format:
			// Piecetype index(BoardPosition) Piecenumber rotation 
			// edge1 edge2 edge3 edge4 Movable
			switch (type)
			{
			case 'C':	
				//Corner piece (type = 0)
				_mainPopulation->piece[index].setInitialPiece(Corner, number, rotation, 
					edge1, edge2, edge3, edge4, true) ;
				break ;
			case 'B':	
				//Boarder piece (type = 1)
				_mainPopulation->piece[index].setInitialPiece(Border, number, rotation, 
					edge1, edge2, edge3, edge4, true) ;
				break ;
			case 'I':	
				//Inner piece (type = 2)
				_mainPopulation->piece[index].setInitialPiece(Inner, number, rotation, 
					edge1, edge2, edge3, edge4, true) ;
				break ;
			default:
				//#(a comment) or other invalid values
				break ;
				cout << endl;
			}//end switch
		}//end while
		solutionFile.close() ;
	}//end if
	else
	{
		cout << "\n\nUnable to open puzzle piece data file." << endl ;
		system("PAUSE") ;
	}
}//end createRandomSolution()



void createRandomPopulation( Population *_population )
{
	// Loop through population and randomise each chromosome's values
	cout << "Randomising Population Values... " ;
	for (int i = 0 ; i < _population->getMaxPopulation() ; i++)
	{
		createRandomSolution(&_population->mainPopulation[i]) ;
	}
	cout << "Complete" << endl ;

	// Clean up fitness record file
	cout << "Cleaning old fitness record file... " ;
	if(remove("GASave - Fitness Data.txt") != 0 )
	{
		// Advise user no such file exists but will be created
		// The file is created upon the initial saving of the fitness data file
		cout << "File does not exist! " << endl ;
		cout << "Fitness File will be created..." << endl ;
	}
	else
	{
		cout << "Complete" << endl ;
	}
}// end createRandomPopulation()



void introduceRandomChromosomes( Population *_population )
{
	for (int i = 0 ; i < (_population->getMaxPopulation()*0.3) ; i++)
	{
		createRandomSolution(&_population->mainPopulation[rand()%_population->getMaxPopulation()]) ;
	}
}//end introduceRandomChromosomes



void saveChromosome( Chromosome *_chromosome )
{
	// Save chromosome to the default save file (Only save one at a time)

	char		type ;						// Storage of piece type
	ofstream	GASaveFile ;				// Used to output to file

	//solutionFile.open("SolutionFor36.txt") ;
	GASaveFile.open("GASave - Fittest Chromosome.txt");

	if (GASaveFile.is_open())
	{
		GASaveFile.precision(2) ;			// Set precision to 2 significant digits
		GASaveFile << showpoint ;			// Show significant digits after decimal point, also 
											// used to format whole numbers)

		//cout << "Save file opened" << endl ;
		for (int k = 0 ; k < _chromosome->getMaxPieces() ; k++)
		{
			//Save chromosome contents into file
			switch(_chromosome->piece[k].type)
			{
				case Corner:
					type = 'C' ;
					break;
				case Border:
					type = 'B' ;
					break;
				case Inner:
					type = 'I' ;
					break;
				default:
					break;
			}
			if (k != 0)
			{// Place a return before each new piece
				GASaveFile << "\n" ;
			}
			GASaveFile << type								<< " " ;// type ;
			GASaveFile << k									<< " " ;// index
			GASaveFile << _chromosome->piece[k].number		<< " " ;// number ;

			// Set precision to 1 for edge types/rotation
			GASaveFile.precision(1) ;					

			GASaveFile << _chromosome->piece[k].topEdge		<< " " ;// edge1 ;
			GASaveFile << _chromosome->piece[k].rightEdge	<< " " ;// edge2 ;
			GASaveFile << _chromosome->piece[k].bottomEdge	<< " " ;// edge3 ;
			GASaveFile << _chromosome->piece[k].leftEdge	<< " " ;// edge4 ;
			GASaveFile << _chromosome->piece[k].rotation ;			// rotation ;

		}//end for piece < maxPieces
		GASaveFile.close() ;

	}//end if
	else
	{
		cout << "Unable to open fittest chromosome save file." << endl ;
		system("Pause") ;
	}
}//end saveChromosome()



void saveFittestChromosome( Population *_population )
{
	cout << "\nFinding fittest chromosome... " ;
	// Loop through population...
	for (int i = 0 ; i < _population->MAX_POPULATION ; i++)
	{
		// ...until a chromosome is found that matches the maximum stored fitness
		if (_population->mainPopulation[i].getChromosomeFitness() == _population->maxPopulationFitness)
		{
			// Save this particular chromosome to file
			saveChromosome(&_population->mainPopulation[i]) ;
		}
	}
	cout << "Complete" << endl ;
}//end saveFittestChromosome()



void printFittestChromosome( Population *_population )
{
	//This sub program is used for the 36 piece puzzle configuration 

	cout << "\nPrinting fittest chromosome... " << endl ;

	bool fittestFound = false ;		// Used to indicate fittest chromosome has been found so check
									// can halt and chromosome can be displayed on screen

	// Loop through the population
	for (int i = 0 ; i < _population->MAX_POPULATION ; i++)
	{
		// If chromosome fitness matches highest fitness in population 
		// AND the fittestFound bool is still false
		if (_population->mainPopulation[i].getChromosomeFitness() == _population->maxPopulationFitness  
			&& !fittestFound)
		{
			// Fittest chromosome found, set bool to true
			fittestFound = true ;

			// Draw the fittest chromosome, draw on screen row by row
			for (int j = 0 ; j < _population->mainPopulation->rows ; j++)
			{

				// Each displayed piece will look like this:
				// -----------
				// |\   2   /| <--- Top edge
				// | \     / |
				// |4 36  0 5| <--- Left edge, Piece number, Piece rotation, Right edge
				// | /     \ |
				// |/   3   \| <--- Bottom edge
				// ----------- <--- is not drawn so no overlap with the next row

				int k = (j*_population->mainPopulation->columns) ;
				cout << "-------------------------------------------------------------" << endl ;
				cout << "|\\   " << _population->mainPopulation[i].piece[k].topEdge << "   /|\\   " << _population->mainPopulation[i].piece[k+1].topEdge << "   /|\\   " << _population->mainPopulation[i].piece[k+2].topEdge << "   /|\\   " << _population->mainPopulation[i].piece[k+3].topEdge << "   /|\\   " << _population->mainPopulation[i].piece[k+4].topEdge << "   /|\\   " << _population->mainPopulation[i].piece[k+5].topEdge << "   /|" << endl ;
				cout << "| \\     / | \\     / | \\     / | \\     / | \\     / | \\     / |" << endl ;
				cout << "|" << _population->mainPopulation[i].piece[k].leftEdge << " " << setfill('0') << setw(2) <<  _population->mainPopulation[i].piece[k].number << "  " << _population->mainPopulation[i].piece[k].rotation << " " << _population->mainPopulation[i].piece[k].rightEdge 
					<< "|" << _population->mainPopulation[i].piece[k+1].leftEdge << " " << setfill('0') << setw(2) << _population->mainPopulation[i].piece[k+1].number << "  " << _population->mainPopulation[i].piece[k+1].rotation << " " << _population->mainPopulation[i].piece[k+1].rightEdge 
					<< "|" << _population->mainPopulation[i].piece[k+2].leftEdge << " " << setfill('0') << setw(2) << _population->mainPopulation[i].piece[k+2].number << "  " << _population->mainPopulation[i].piece[k+2].rotation << " " << _population->mainPopulation[i].piece[k+2].rightEdge 
					<< "|" << _population->mainPopulation[i].piece[k+3].leftEdge << " " << setfill('0') << setw(2) << _population->mainPopulation[i].piece[k+3].number << "  " << _population->mainPopulation[i].piece[k+3].rotation << " " << _population->mainPopulation[i].piece[k+3].rightEdge 
					<< "|" << _population->mainPopulation[i].piece[k+4].leftEdge << " " << setfill('0') << setw(2) << _population->mainPopulation[i].piece[k+4].number << "  " << _population->mainPopulation[i].piece[k+4].rotation << " " << _population->mainPopulation[i].piece[k+4].rightEdge 
					<< "|" << _population->mainPopulation[i].piece[k+5].leftEdge << " " << setfill('0') << setw(2) << _population->mainPopulation[i].piece[k+5].number << "  " << _population->mainPopulation[i].piece[k+5].rotation << " " << _population->mainPopulation[i].piece[k+5].rightEdge 
					<< "|" << endl ;
				cout << "| /     \\ | /     \\ | /     \\ | /     \\ | /     \\ | /     \\ |" << endl ;
				cout << "|/   " << _population->mainPopulation[i].piece[k].bottomEdge << "   \\|/   " << _population->mainPopulation[i].piece[k+1].bottomEdge << "   \\|/   " << _population->mainPopulation[i].piece[k+2].bottomEdge << "   \\|/   " << _population->mainPopulation[i].piece[k+3].bottomEdge << "   \\|/   " << _population->mainPopulation[i].piece[k+4].bottomEdge << "   \\|/   " << _population->mainPopulation[i].piece[k+5].bottomEdge << "   \\|" << endl ;
			}//end for j
			// All pieces drawn, draw last line of fittest puzzle solution
			cout << "-------------------------------------------------------------\n" << endl ;
		}//end if
	}//end for i

}//end printFittestChromosome()



void saveFitnessData( Population *_population ) 
{
	// Create and open a file to save fitness data
	static bool headersDrawn = false;	// Static bool used so headers are only drawn to file once
										// and not each generation (improves readability)
	ofstream	GASaveFile ;			// Used to output to fitness data file

	GASaveFile.open("GASave - Fitness Data.txt", ios::app) ;

	if (GASaveFile.is_open())
	{
		GASaveFile.precision(4) ;			// Set precision to 4 significant digits
		GASaveFile << showpoint ;			// Show significant digits after decimal point, also 
											// used to format whole numbers)

		// If the headers have not been saved to file, save them and set headersDrawn to true
		if (!headersDrawn)
		{
			GASaveFile << "Generation " ;
			GASaveFile.width(17) ;
			GASaveFile << "Max Fitness " ;
			GASaveFile.width(20) ;
			GASaveFile << "Average Fitness " << endl ;
			headersDrawn = true ;
		}

		// Save population data to file and close it
		GASaveFile << _population->numberOfGenerations << " \t\t" ;
		GASaveFile << _population->maxPopulationFitness << " \t\t" ;
		GASaveFile << _population->avePopulationFitness << " \t\t" ;
		GASaveFile << "\n" ;
		GASaveFile.close() ;
	}
	else
	{
		cout << "Unable to open Max Fitness save file." << endl ;
		system("Pause") ;
	}
}// end saveFitnessData()