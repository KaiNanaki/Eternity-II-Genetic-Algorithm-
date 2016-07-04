# Eternity II - Genetic Algorithm
A Genetic Algorithm designed to attempt to solve the "Eternity II" puzzle, a large NP-Complete combinatorial problem.

About the Eternity II puzzle
Eternity II (EII) is an NP-Complete signed edge matching puzzle released in 2007. It involves 
the placement of 256 unique square pieces onto a 16 x 16 grid; each puzzle square is spilt 
twice diagonally into 4 coloured triangles which each contain half of an individually coloured shape.
These must match up with an identical triangle/shape on the adjacent piece’s edge. The border and 
corner pieces contain triangles that are grey with no shape in them; these edges must be 
adjacent to the board edge.
To date no complete solution has been found. This is not surprising considering there are at most 
256! × 4 to the 256th power possible solutions (or 1.15×10 to the 661th power possible 
combinations). As such an exhaustive search method was decided against and a Genetic Algorithm 
was used in hopes that its heuristic properties would allow the puzzle to be solved in a much 
shorter space of time.

UPDATED (April 2016)
I have updated the EII GA in 2016 by added further commenting, making minor improvements to the code
and general tidying of the program. 
Improvements include: 
- Formatted data displayed in window to make it more readable.
- Adding the ability to display the fittest chromosome on screen in text format.
- Added window resizing so the on screen chromosome can be more easily seen.
- Adding the option to pause the GA so this chromosome/GA data can been checked if the data is
streaming down the window too quickly.
- Redesigning the fitness subprogram to be more efficient and to check only hard constraints.
- Altering how the fittest chromosome/generation text files are saved to make them more readable.
