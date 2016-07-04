#include "DataStructures.h"
#include <math.h>

using namespace std;


//////////////////////////////////////////////////////////////////////////
//
// PuzzlePiece Class subprograms
//
//////////////////////////////////////////////////////////////////////////

// Sets up the initial puzzle pieces
void PuzzlePiece::setInitialPiece( pieceType	_pType,	int		_pNum, 
								   int			_pRot,	int		_top, 
								   int			_right,	int		_bottom, 
								   int			_left,	bool	_movable )
{
	// Copy values into current piece
	type		= _pType ;
	number		= _pNum ;
	rotation	= _pRot ;
	topEdge		= _top ;
	rightEdge	= _right ;
	bottomEdge	= _bottom;
	leftEdge	= _left ;
	movable		= _movable ;

	// Update the piece's rotation
	setInitialRotation() ;
}


//sets up the initial puzzle piece rotation
void PuzzlePiece::setInitialRotation()
{
	int tempEdge = 0 ;		// Used to hold an edge value for updating
							// piece rotation

	for (int i = 0 ; i < rotation ; i++)
	{// Apply new rotation value to the piece
		tempEdge	= leftEdge ;
		leftEdge	= bottomEdge ;
		bottomEdge	= rightEdge ;
		rightEdge	= topEdge ;
		topEdge		= tempEdge ;
	}//end for

	//cout << "Piece Rotated by " << rotation << endl ;
}


//updates the puzzle piece rotation
void PuzzlePiece::updateRotation()
{
	int tempEdge = 0 ;		// Used to hold an edge value for updating
							// piece rotation

	// Apply one rotation to the current piece
	tempEdge		= leftEdge ;
	leftEdge		= bottomEdge ;
	bottomEdge		= rightEdge ;
	rightEdge		= topEdge ;
	topEdge			= tempEdge ;

	// If rotation value is 3, reset it to 0 (0-3 for 0, 90, 180, and 270 degrees)
	if (rotation == 3)
	{
		rotation = 0 ;
	}
	// Else add 1 to the rotation value
	else if (rotation < 3)
	{
		rotation++ ;
	}

}

void PuzzlePiece::setPiece( pieceType	_pType,	int		_pNum, 
						    int			_pRot,	int		_top, 
							int			_right,	int		_bottom,
							int			_left,	bool	_movable )
{
	// Copy values into current piece without applying rotation like setInitialPiece()
	type		= _pType ;
	number		= _pNum ;
	rotation	= _pRot ;
	topEdge		= _top ;
	rightEdge	= _right ;
	bottomEdge	= _bottom;
	leftEdge	= _left ;
	movable		= _movable ;
}


//////////////////////////////////////////////////////////////////////////
//
// Chromosome Class subprograms
//
//////////////////////////////////////////////////////////////////////////

float Chromosome::determineChromosomeFitness()
{
	// Constraint variables: Used for tallying constraints
	int cornerOnCorner		= 0 ;
	int cornerMatchOuter	= 0 ;
	int cornerMatchBorder	= 0 ;
	int borderOnBorder		= 0 ;
	int borderMatchOuter	= 0 ;
	int borderMatchBorder	= 0 ;
	int borderMatchInner	= 0 ;
	int innerOnInner		= 0 ;
	int innerRowEdges		= 0 ;
	int innerColumnEdges	= 0 ;

	//////////////////////////////////////////////////////////////////////////
	// Row/Column edge matching check + piece type on correct squares
	//////////////////////////////////////////////////////////////////////////
	for (int i = 0 ; i < rows ; i++)
	{
		//cout << "\nRow: " << i ;
		// Check vertical/horizontal edges for pieces on this row
		for (int j = (i*columns) ; j <= ((i*columns)+(columns-1)) ; j++)
		{
			//cout << "\nPosition: " << j << "\tPiece: " << piece[j].number << endl ;
			////////////////////////////////
			// First and last row checks
			////////////////////////////////
			if (i == 0 || i == (rows-1))
			{// If on first and last row

				//cout << "i=0 or i=rows-1. " ;
				// Corner piece checks (first column)
				if (j == (i*columns) && piece[j].type == Corner)
				{// If on first column (corner position) and is a corner piece

					//cout << "1st col CoC. " ;
					// Piece is confirmed to be a corner piece
					cornerOnCorner += 1 ;

					// Row checks
					if (piece[j].leftEdge == 1)
					{// Corner piece left edge matches outer board
						cornerMatchOuter += 1 ;
						//cout << "CmO. " ;
					}
					if (piece[j].rightEdge == piece[(j+1)].leftEdge
						&& piece[j+1].type == Border)
					{// Corner piece right edge matches the next piece's left 
					 // edge AND that piece is a border piece
						cornerMatchBorder += 1 ;
						//cout << "CmB. " ;
					}

					// Column checks
					if (i == 0 && piece[j].topEdge == 1)
					{// Top left corner piece, top edge matches outer board
						cornerMatchOuter += 1 ;
						//cout << "CmO(c). " ;
					}
					if (i == 0 && piece[j].bottomEdge == piece[j+columns].topEdge
						&& piece[j+columns].type == Border)
					{// Top left corner piece, bottom edge matches the top
					 // edge of piece below AND that piece is a border piece
						cornerMatchBorder += 1 ;
						//cout << "CmB(c). " ;
					}
					if (i == (rows-1) && piece[j].bottomEdge == 1)
					{// Bottom left corner piece, bottom edge matches outer board
						cornerMatchOuter += 1 ;
						//cout << "CmO(c). " ;
					}
					if (i == (rows-1) && piece[j].topEdge == piece[j-columns].bottomEdge
						&& piece[j-columns].type == Border)
					{// Bottom left corner piece, top edge matches the bottom
					 // edge of piece above AND that piece is a border piece
						cornerMatchBorder += 1 ;
						//cout << "CmB(c). " ;
					}
				}// end if - Corner piece checks (first column)



				//Corner piece checks (last column)
				if (j == ((i*columns)+(columns-1)) && piece[j].type == Corner)
				{//If also on left edge of board and is a corner piece

					// Piece is confirmed to be a corner piece
					cornerOnCorner += 1 ;
					//cout << "last col CoC. " ;

					// Row checks
					if (piece[j].rightEdge == 1)
					{// If corner piece right edge matches outer board
						cornerMatchOuter	+= 1 ;
						//cout << "CmO. " ;
					}
					if (piece[j].leftEdge == piece[j-1].rightEdge 
						&& piece[j-1].type == Border) // <--------remove?2016
					{// If corner piece left edge matches the previous piece's right edge
					 // AND that piece is a border piece
						cornerMatchBorder += 1 ;
						//cout << "CmB. " ;
					}

					// Column checks
					if (i == 0 && piece[j].topEdge == 1)
					{// Top right corner piece, top edge matches outer board
						cornerMatchOuter += 1 ;
						//cout << "CmO(c). " ;
					}
					if (i == 0 && piece[j].bottomEdge == piece[j+columns].topEdge
						&& piece[j+columns].type == Border)
					{// Top right corner piece, bottom edge matches the top
					 // edge of piece below AND that piece is a border piece
						cornerMatchBorder += 1 ;
						//cout << "CmB(c). " ;
					}
					if (i == (rows-1) && piece[j].bottomEdge == 1)
					{// Bottom right corner piece, bottom edge matches outer board
						cornerMatchOuter += 1 ;
						//cout << "CmO(c). " ;
					}
					if (i == (rows-1) && piece[j].topEdge == piece[j-columns].bottomEdge
						&& piece[j-columns].type == Border)
					{// Bottom right corner piece, top edge matches the bottom
					 // edge of piece above AND that piece is a border piece
						cornerMatchBorder += 1 ;
						//cout << "CmB(c). " ;
					}
				}// end if - Corner piece checks (last column)



				// Border piece checks (First and last row)
				if (j > (i*columns) && j < ((i*columns)+(columns-1)) 
					&& piece[j].type == Border)
				{// If between first and last columns of the row (border piece positions)
				 // AND the piece is a border piece

					//cout << "1st/last row BoB. " ;
					// Piece is confirmed to be a border piece
					borderOnBorder += 1 ;
					
					// Row checks
					if ((j+1) != ((i*columns)+(columns-1)))
					{// If next piece along is not the last piece on the row (as corner/border
					 // edge checks have been done earlier)

						if (piece[j].rightEdge == piece[j+1].leftEdge && piece[j+1].type == Border)
						{// If border piece right edge matches the next piece's left edge
						 // AND the next piece is a border piece
							borderMatchBorder += 1 ;
							//cout << "BmB. " ;
						}
					}

					// Column checks
					if (i == 0 && piece[j].topEdge == 1)
					{// Top border piece, top edge matches outer board
						borderMatchOuter += 1 ;
						//cout << "BmO(c). " ;
					}
					if (i == 0 && piece[j].bottomEdge == piece[j+columns].topEdge
						&& piece[j+columns].type == Inner)
					{// Top border piece, bottom edge matches the top
					 // edge of piece below AND that piece is an inner piece
						borderMatchInner += 1 ;
						//cout << "BmI(c). " ;
					}
					if (i == (rows-1) && piece[j].bottomEdge == 1)
					{// Bottom border piece, bottom edge matches outer board
						borderMatchOuter += 1 ;
						//cout << "BmO(c). " ;
					}
					if (i == (rows-1) && piece[j].topEdge == piece[j-columns].bottomEdge
						&& piece[j-columns].type == Inner)
					{// Bottom border piece, top edge matches the bottom
					 // edge of piece above AND that piece is an inner piece
						borderMatchInner += 1 ;
						//cout << "BmI(c). " ;
					}

				}// End if - Border piece checks (First and last row)

			}////////////////////////////////
			 // End If - First and last row checks
			 ////////////////////////////////



			////////////////////////////////
			// Else - Checks for rows between first and last
			////////////////////////////////
			else
			{
				//cout << "else not 1st/last rows. " ;
				// Border piece checks (First column)
				if (j == (i*columns) && piece[j].type == Border)
				{// Piece is on first column of the current row (border position)
				 // and is a border piece

					//cout << "1st Col BoB. " ;
					// Piece is confirmed to be a border piece
					borderOnBorder += 1 ;

					// Row checks
					if (piece[j].leftEdge == 1)
					{// Border piece left edge matches outer board
						//cout << "BmO. " ;
						borderMatchOuter += 1 ;
					}
					if (piece[j].rightEdge == piece[j+1].leftEdge
						&& piece[j+1].type == Inner)
					{// Border piece right edge matches the next piece's left edge
					 // AND that piece is an inner piece
						//cout << "BmI. " ;
						borderMatchInner += 1 ;
					}

					// Column checks
					if ((j+columns) != ((rows*columns)-columns)) 
					{// If the next piece down is not the first piece on the last row (as these 
					 // corner/border edge matching checks have been done earlier)

						if (piece[j].bottomEdge == piece[j+columns].topEdge 
							&& piece[j+columns].type == Border)
						{// If border piece right edge matches the next piece's left edge
						 // AND the next piece is a border piece
							borderMatchBorder += 1 ;
							//cout << "BmB(c). " ;
						}
					}
				}//end if - Border piece checks (First column)

				// Border piece checks (Last column)
				if (j == ((i*columns)+(columns-1)) && piece[j].type == Border)
				{// Piece is on last column of the current row (border position)
				 // AND is a border piece

					//cout << "last Col BoB. " ;
					// Piece is confirmed to be a border piece
					borderOnBorder += 1 ;

					// Row checks
					if (piece[j].rightEdge == 1)
					{// Border piece right edge matches outer board
						borderMatchOuter += 1 ;
						//cout << "BmO. " ;
					}
					if (piece[j].leftEdge == piece[j-1].rightEdge
						&& piece[j-1].type == Inner)
					{// Border piece left edge matches the previous piece's right edge
					 // AND that piece is an inner piece
						borderMatchInner += 1 ;
						//cout << "BmI. " ;
					}

					// Column checks
					if ((j+columns) != ((rows*columns)-1)) 
					{// If the next piece down is not the last piece on the last column (as these
					 // corner/border edge matching checks have been done earlier)

						if (piece[j].bottomEdge == piece[j+columns].topEdge 
							&& piece[j+columns].type == Border)
						{// If border piece right edge matches the next piece's left edge
						 // AND the next piece is a border piece
							borderMatchBorder += 1 ;
							//cout << "BmB(c). " ;
						}
					}
				}//end if - Border piece checks (Last column)



				// Inner piece checks (center section of board)
				if (j > (i*columns) && j < ((i*columns)+(columns-1)) 
					&& piece[j].type == Inner)
				{// Pieces are between first and last rows and if between
				 // first and last columns also then...

					//cout << "Inner checks, IoI. " ;
					// Piece is confirmed to be an inner piece
					innerOnInner += 1 ;

					// Row checks
					if ((j+1) != ((i*columns)+(columns-1)))
					{// If next piece along is not the last piece on the row 
					 // (as the border edge checks have been done earlier)

						if (piece[j].rightEdge == piece[j+1].leftEdge && piece[j+1].type == Inner)
						{// If inner piece right edge matches the next piece's
						 // left edge AND the next piece is an inner piece
							//cout << "IrE. " ;
							innerRowEdges += 1 ;
						}
					}

					// Column checks
					if ((j+columns) != ((rows*columns)-1)) 
					{// If the next piece down is not the last piece on the column (as these 
					 // corner/border edge matching checks have been done earlier)

						if (piece[j].bottomEdge == piece[j+columns].topEdge 
							&& piece[j+columns].type == Inner)
						{// If inner piece bottom edge matches the next piece's
						 // top edge AND the next piece is an inner piece
							innerColumnEdges += 1 ;
							//cout << "IcE(c). " ;
						}
					}

				}// End If - Inner piece checks (center section of board)

			}////////////////////////////////
			 // End Else - Checks for rows between first or last
			 ////////////////////////////////

		}// End For - Check vertical edges in this row
	}// End For - Row/Column edge matching check + piece type on correct squares
	

	///////////////////////
	// 256 piece
	//
	// Determine % of fitness
	// Edges = 256* 2 (|_) , then add 16*2 == 544, 
	// Positions = 544+ 256 if each piece is in correct place on board
	// Total = 800
	//
	// Counts for maximum constraint varibles on 256 piece board
	// CoC (4), CmO (8), CmB (8),				// Always 4 corners with 8 inner and outer edges
	// BoB (56), BmO (56), BmB (52), BmI (56),	//
	// IoI (196), IrE (182), IcE (182).			//
	// Total = 800
	///////////////////////


	///////////////////////
	// 36 piece
	//
	// Counts for maximum constraint varibles on 36 piece board
	// CoC (4), CmO (8), CmB (8),				// Always 4 corners with 8 inner and outer edges
	// BoB (16), BmO (16), BmB (12), BmI (16),	//
	// IoI (16), IrE (12), IcE (12).			//
	// Total = 120
	///////////////////////


	// Constrain values
	// Can be changed, *1 currently but can be given higher values for each individual constrain
	// Increasing these weighted values will allow for favouring of certain pieces or placements with a higher
	
	// Chromosome Fitness = SUM of ConstraintVaribles * Constraint Values
	chromosomeFitness = cornerOnCorner*1		+ cornerMatchOuter*1 + 
						cornerMatchBorder*1		+ borderOnBorder*1 + 
						borderMatchOuter*1		+ borderMatchBorder*1 +
						borderMatchInner*1		+ innerOnInner*1 +
						innerRowEdges*1			+ innerColumnEdges*1 ;


	//chromosomeFitness	= ((chromosomeFitness/800)*100) ;	// Calculation of fitness for 256 piece
	
	chromosomeFitness	= ((chromosomeFitness/120)*100) ;	// Calculation of fitness for 36 piece

	return chromosomeFitness ;

	// Used for testing fitness subprograms constraint values
	//cout << "\n\nCoC: " << cornerOnCorner << endl;
	//cout << "CmO: " << cornerMatchOuter << endl;
	//cout << "CmB: " << cornerMatchBorder << endl;
	//cout << "BoB: " << borderOnBorder << endl;
	//cout << "BmO: " << borderMatchOuter << endl;
	//cout << "BmB: " << borderMatchBorder << endl;
	//cout << "BmI: " << borderMatchInner << endl;
	//cout << "IoI: " << innerOnInner << endl;
	//cout << "IrE: " << innerRowEdges << endl;
	//cout << "IcE: " << innerColumnEdges << endl;
}

float Chromosome::getChromosomeFitness()
{
	return chromosomeFitness ;
}

int Chromosome::getMaxPieces()
{
	return MAX_PIECES ;
}

//////////////////////////////////////////////////////////////////////////
//
// Population Class subprograms
//
//////////////////////////////////////////////////////////////////////////

void Population::initialisePopulation()
{
	numberOfGenerations		= 0 ;	// Keeps count of the number of generations
	avePopulationFitness	= 0 ;	// Holds the populations average fitness
	maxPopulationFitness	= 0 ;	// Holds the populations maximum fitness

	cout << "Initialising Population... " ;
	// Initialise the main population
	for (int i = 0 ; i < MAX_POPULATION ; i++)
	{
		for (int j = 0 ; j < mainPopulation->getMaxPieces() ; j++)
		{
			mainPopulation[i].piece[j].setInitialPiece(Corner, 0, 0, 0, 0, 0, 0, true) ;
			mainPopulation[i].chromosomeFitness = 0.0 ;
		}
	}

	// Initialise temp population
	for (int k = 0 ; k < MAX_POPULATION ; k++)
	{
		for (int l = 0 ; l < tempPopulation->getMaxPieces() ; l++)
		{
			tempPopulation[k].piece[l].setInitialPiece(Corner, 0, 0, 0, 0, 0, 0, true) ;
			tempPopulation[k].chromosomeFitness = 0 ;
		}
	}
	cout << "Complete" << endl ;
}// end initialisePopulation()


void Population::selection()
{
	int	currentPopulationPosition	= 0 ;	// Determines offspring placement in tempPopulation
	int	randChrom1					= 0 ;	// Holds random indices for potential parents
	int	randChrom2					= 0 ;	// Holds random indices for potential parents
	int	firstParent					= 0 ;	// Index of first parent
	int	secondParent				= 0 ;	// Index of second parent

	// Loops through the population, 2 parents are selected each loop so
	// only half of the populations max size needs to looped through
	for (int j = 0 ; j < MAX_POPULATION/2 ; j++)
	{
		// Randomly select two different parents
		do 
		{
			randChrom1 = rand()%MAX_POPULATION ;
			randChrom2 = rand()%MAX_POPULATION ;
		} while (randChrom1 == randChrom2 );

		// Determine first parent (tournament selection between randChrom1 & randChrom2)
		if (mainPopulation[randChrom1].getChromosomeFitness() >
			mainPopulation[randChrom2].getChromosomeFitness())
		{
			firstParent	= randChrom1 ;
		}
		else
		{
			firstParent	= randChrom2 ;
		}

		// Randomly select another two parents
		do 
		{
			randChrom1 = rand()%MAX_POPULATION ;
			randChrom2 = rand()%MAX_POPULATION ;
		} while (randChrom1 == randChrom2 );


		// Determine second parent (tournament selection between randChrom1 & randChrom2)
		if (mainPopulation[randChrom1].getChromosomeFitness() >
			mainPopulation[randChrom2].getChromosomeFitness())
		{
			secondParent	= randChrom1 ;
		}
		else
		{
			secondParent	= randChrom2 ;
		}

		// Breed firstParent with secondParent
		crossoverTwoPoint(mainPopulation[firstParent], mainPopulation[secondParent], 
			currentPopulationPosition) ;
		// Breeding complete, continue selection process loop

		// 2 offspring created so move 2 up the tempPopulation array
		currentPopulationPosition += 2 ;
	}//end for

}//end selection()

void Population::crossoverTwoPoint( Chromosome _chromosome1, 
									 Chromosome _chromosome2, 
									 int _populationPosition )
{
	// Crossover part 1: 
	// Parent information copied into offspring (tempPupulation)
	for ( int i = 0 ; i < _chromosome1.getMaxPieces() ; i++ )
	{
		// First pair of offspring
		tempPopulation[_populationPosition].piece[i]	= _chromosome1.piece[i] ;	// Child 1
		tempPopulation[_populationPosition+1].piece[i]	= _chromosome2.piece[i] ;	// Child 2
	}

	// Crossover part 2:
	// Crossover points chosen
	int crossOverPoint	= 0 ;	// holds random crossover point
	int crossOverPoint2	= 0 ;	// holds random crossover point

	do 
	{
		// Save two random crossover points where crossOverPoint is less than crossOverPoint2
		// (This ensures the next for loop copies data accross correctly/in order)
		crossOverPoint	= rand()%_chromosome1.getMaxPieces() ;	// 0-MAX_PIECES
		crossOverPoint2	= rand()%_chromosome1.getMaxPieces() ;	// 0-MAX_PIECES
	} while (crossOverPoint2 < crossOverPoint );

	// Crossover part 2: 
	// Crossover data from parents copied into children
	for ( int j = crossOverPoint ; j < crossOverPoint2 ; j++ )
	{
		// Copy parent 2 info directly in to child 1 info from crossover point
		tempPopulation[_populationPosition].piece[j] = _chromosome2.piece[j] ;

		// Copy parent 1 info directly into child 2 info from crossover point
		tempPopulation[_populationPosition+1].piece[j] = _chromosome1.piece[j] ;
	}

}//end crossoverTwoPoint



void Population::mutation()
{
	int			mutateChromosome	= 0 ;	// Holds random chromosome number
	int			mutatePiece			= 0 ;	// Holds random piece number
	int			mutatePiece2		= 0 ;	// Holds random piece number
	PuzzlePiece	tempPiece ;					// Holds temporary data while swapping pieces

	// Initialise tempPiece to default values
	tempPiece.setInitialPiece(Corner, 0, 0, 0, 0, 0, 0, true) ;


	// Mutate random chromosomes by rotating one of their pieces
	// Loop until mutation rate is reached
	for (int i = 0 ; i < MUTATION_RATE ; i++)
	{
		// Get a random value for a chromosome and a piece within that chromosome
		mutateChromosome	= rand()%MAX_POPULATION ;					// 0-MAX_POPULATION
		mutatePiece			= rand()%mainPopulation->getMaxPieces() ;	// 0-MAX_PIECES

		tempPopulation[mutateChromosome].piece[mutatePiece].updateRotation() ;
	}//end for i


	// Mutate a random chromosome by swapping two of its pieces
	for (int j = 0 ; j < MUTATION_RATE ; j++)
	{
		// Get a random value for a chromosome and a two pieces within that chromosome
		mutateChromosome	= rand()%MAX_POPULATION ;					// 0-MAX_POPULATION
		mutatePiece			= rand()%mainPopulation->getMaxPieces() ;	// 0-MAX_PIECES
		mutatePiece2		= rand()%mainPopulation->getMaxPieces() ;	// 0-MAX_PIECES

		// Store mutated piece 1 in tempPiece
		tempPiece.setPiece(
			tempPopulation[mutateChromosome].piece[mutatePiece].type,
			tempPopulation[mutateChromosome].piece[mutatePiece].number,
			tempPopulation[mutateChromosome].piece[mutatePiece].rotation,
			tempPopulation[mutateChromosome].piece[mutatePiece].topEdge,
			tempPopulation[mutateChromosome].piece[mutatePiece].rightEdge,
			tempPopulation[mutateChromosome].piece[mutatePiece].bottomEdge,
			tempPopulation[mutateChromosome].piece[mutatePiece].leftEdge,
			tempPopulation[mutateChromosome].piece[mutatePiece].movable) ;


		// Copy mutated piece 2 in piece 1
		tempPopulation[mutateChromosome].piece[mutatePiece].setPiece(
			tempPopulation[mutateChromosome].piece[mutatePiece2].type,
			tempPopulation[mutateChromosome].piece[mutatePiece2].number,
			tempPopulation[mutateChromosome].piece[mutatePiece2].rotation,
			tempPopulation[mutateChromosome].piece[mutatePiece2].topEdge,
			tempPopulation[mutateChromosome].piece[mutatePiece2].rightEdge,
			tempPopulation[mutateChromosome].piece[mutatePiece2].bottomEdge,
			tempPopulation[mutateChromosome].piece[mutatePiece2].leftEdge,
			tempPopulation[mutateChromosome].piece[mutatePiece2].movable) ;


		// Copy temp piece (mutated piece 1 data) in piece 2
		tempPopulation[mutateChromosome].piece[mutatePiece2].setPiece(
			tempPiece.type,
			tempPiece.number,
			tempPiece.rotation,
			tempPiece.topEdge,
			tempPiece.rightEdge,
			tempPiece.bottomEdge,
			tempPiece.leftEdge,
			tempPiece.movable) ;
	}//end for j

}//end mutation()



void Population::scanAndFix()
{
	bool	duplicatesFound ;
	bool	usedPieces[tempPopulation->MAX_PIECES] ;
	int		duplicatePieces[tempPopulation->MAX_PIECES] ;

	// Scan & Fix
	// Loop through the population
	for (int i = 0 ; i < MAX_POPULATION ; i++)
	{
		// Default boolean variable and arrays, ready for chromosome[i] to be checked
		duplicatesFound = false ;
		for (int j = 0 ; j < tempPopulation->getMaxPieces() ; j++)
		{
			usedPieces[j]		= false ;
			duplicatePieces[j]	= -1 ;
		}//end for


		//////////////////////
		// Scan Chromosome
		//////////////////////

		// Search through current chromosome and mark pieces
		for (int k = 0 ; k < tempPopulation->getMaxPieces() ; k++)
		{
			// Set check variable to current piece
			int checkPieceNumber = tempPopulation[i].piece[k].number ;

			// Check for duplicate pieces within the chromosome
			if (usedPieces[checkPieceNumber-1])
			{
				// This scan method is fine for crossover with two parents  but will need to be
				// changed if more than two parents are to breed
				// (Perhaps an int array to store repeat duplicates?)

				// Duplicates found so set bool to true
				// Must run "fix" section so record duplicate pieces and position
				duplicatesFound		= true ;
				duplicatePieces[k]	= checkPieceNumber;
				//cout << "Duplicate Found in chromosome: " << i << ". Position: " << k << 
				//	", PieceNo: " << checkPieceNumber << endl ;
			}
			else if (!usedPieces[checkPieceNumber-1])
			{
				// Check to see if the piece exists and is used
				usedPieces[checkPieceNumber-1] = true ;
				//cout << "Position: " << k << ", PieceNo: " << checkPieceNumber << " is now used" << endl ;
			}
		}//end for
		//cout << "Chromosome: " << i << " scanned" << endl ;
		//////////////////////
		// End of Scan
		//////////////////////

		//////////////////////
		// Fix Chromosome
		//////////////////////
		if (duplicatesFound)
		{
			//cout << "Fixing required on chromosome: " << i << endl ;
			for (int l = 0 ; l < tempPopulation->getMaxPieces() ; l++)
			{ 
				if (!usedPieces[l])
				{
					// If piece in position[l] has not been used (and is therefore missing)
					int m = 0 ;		// Used to hold duplicate piece number

					do
					{// Loop until the next duplicate piece is found
						
						// Random number between 0 and MaxPieces
						m = rand()%tempPopulation->getMaxPieces() ; 

						if (duplicatePieces[m] > -1 && !usedPieces[l])
						{
							// When duplicate piece (m) is found

							//cout << "Duplicate piece: " << duplicatePieces[m] << " in position: " << m << endl ;
							for (int n = 0 ; n < mainPopulation->getMaxPieces() ; n++)
							{
								// Find the missing piece[l+1] to overwrite the duplicate
								if (mainPopulation[i].piece[n].number == l+1)
								{
									//cout << "Replacing piece: " << duplicatePieces[m] << " in position: " << m <<
									//	" with missing piece: " << l+1 << endl ;

									// Fix duplicate piece m (in tempPop) using the missing
									// piece l+1 (missing in tempPop so obtained from mainPop)
									tempPopulation[i].piece[m].setPiece(
										mainPopulation[i].piece[n].type, 
										mainPopulation[i].piece[n].number,
										mainPopulation[i].piece[n].rotation, 
										mainPopulation[i].piece[n].topEdge, 
										mainPopulation[i].piece[n].rightEdge, 
										mainPopulation[i].piece[n].bottomEdge, 
										mainPopulation[i].piece[n].leftEdge,
										true) ;
									usedPieces[l] = true ;	// Missing piece has now been used
								}
							}//end for n													
						}//end if duplicatePieces[m] > -1

					} while(duplicatePieces[m] == -1) ;

					duplicatePieces[m] = -1 ;	// Duplicate piece has now been replaced

				}//end if !usedPieces[l]

			}//end for l
			//cout << "Fixing complete on Chromosome: " << i << endl ;

		}//end if (duplicatesFound)
		//////////////////////
		// End of Fix
		//////////////////////

	}//end for int i - Scan & Fix

}//end scanAndFix()



void Population::replacePopulation()
{
	// Loop through main population chromosomes
	for (int i = 0 ; i < MAX_POPULATION ; i++)
	{
		// Loop through each piece in the chromosome
		for (int j = 0 ; j < mainPopulation->getMaxPieces() ; j++)
		{
			// Replace the main population with the newly bred population (tempPopulation)
			// by copying over its piece data
			mainPopulation[i].piece[j].setPiece(tempPopulation[i].piece[j].type,
				tempPopulation[i].piece[j].number,
				tempPopulation[i].piece[j].rotation,
				tempPopulation[i].piece[j].topEdge,
				tempPopulation[i].piece[j].rightEdge,
				tempPopulation[i].piece[j].bottomEdge,
				tempPopulation[i].piece[j].leftEdge,
				tempPopulation[i].piece[j].movable) ;
		}
	}
}//end replacePopulation()



void Population::determinePopulationfitness()
{
	// Loop through main population chromosomes
	for (int i = 0 ; i < MAX_POPULATION ; i++)
	{
		// Determine each chromosomes fitness (fitness value saved within chromosome)
		mainPopulation[i].determineChromosomeFitness() ;
	}
}//end determinePopulationfitness()



float Population::getMaxPopulationFitness()
{
	float maxFitness = 0.0 ;	// Used to save the current highest fitness value

	// Loop through main population chromosomes
	for (int i = 0 ; i < MAX_POPULATION ; i++)
	{

		// Find each chromosome's fitness and save value (if higher than previous value)
		if (mainPopulation[i].getChromosomeFitness() > maxFitness)
		{
			// When found, save value
			maxFitness = mainPopulation[i].getChromosomeFitness() ;
		}
	}

	maxPopulationFitness = maxFitness ; // Save this generations maximum population fitness
	return maxFitness ;

}//end getMaxPopulationFitness()



float Population::getAveragePopulationFitness()
{
	float fitnessTotal = 0.0 ;	// Used to save the total fitness value

	// Loop through main population chromosomes
	for (int i = 0 ; i < MAX_POPULATION ; i++)
	{
		// Add current chromosome's fitness to the total
		fitnessTotal += mainPopulation[i].getChromosomeFitness() ;
	}

	// Work out the average population fitness and save value
	avePopulationFitness = (fitnessTotal/MAX_POPULATION) ;
	return (fitnessTotal/MAX_POPULATION) ;
}//end getAveragePopulationFitness()



int Population::getMaxPopulation()
{
	return MAX_POPULATION ;
}//end getMaxPopulation()