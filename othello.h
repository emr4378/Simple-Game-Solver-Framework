// File:         $Id: othello.h,v 1.1 2011/11/09 03:44:01 emr4378 Exp emr4378 $
// Revisions:
//              $Log: othello.h,v $
//              Revision 1.1  2011/11/09 03:44:01  emr4378
//              Initial revision
//
//              Revision 1.2  2011/11/02 03:40:46  p334-01o
//              Added comments, other tweaks.
//
//              Revision 1.1  2011/10/19 21:02:18  p334-01o
//              Initial Check-in
//
//
// Authors: Jeffrey Zullo - jlz9811
//			Eduardo Rodrigues - emr4378
//			
//
#ifndef OTHELLO_H
#define OTHELLO_H

#include <vector>
#include "game.h"

/**
 * Othello
 *
 * A 2-player game in which players capture one anothers pieces and attempt to have the most
 * pieces on the board at the end of the game.
 *
 * Rules:
 * - can place a piece anywhere on the board that is empty.
 * - if two of your pieces sandwich one or more of your opponents pieces, you capture those pieces
 *   and they become yours.
 *
 * Actual Othello class only contains the rules of the specific game.
 */
class Othello : public Game< std::vector< std::vector<int> > > {

	public:
		Othello(std::vector< std::vector<int> > startingBoard);
		~Othello();

		bool IsEndBoard(std::vector< std::vector<int> > gB);
		std::vector< std::vector< std::vector<int> > > GenerateMoves(std::vector< std::vector<int> > gB, 
																	 std::vector< std::vector< std::vector<int> > > *moves);
		std::vector< std::vector<int> > ReverseBoard(std::vector< std::vector<int> > gB);
		void PrintBoard(std::vector< std::vector<int> > gB);
		void ApplyMove(std::vector< std::vector<int> >* gB, std::vector< std::vector<int> > move);
		int ScoreGameBoard(std::vector< std::vector<int> >* gB);

	private:
		

};
#endif