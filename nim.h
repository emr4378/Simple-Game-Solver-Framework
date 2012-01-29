// File:         $Id: nim.h,v 1.1 2011/11/09 03:44:01 emr4378 Exp emr4378 $
// Revisions:
//              $Log: nim.h,v $
//              Revision 1.1  2011/11/09 03:44:01  emr4378
//              Initial revision
//
//              Revision 1.1  2011/11/02 03:29:14  p334-01o
//              Initial revision
//
//              Revision 1.1  2011/10/19 21:02:18  p334-01o
//              Initial Check-in
//
//
// Authors: Jeffrey Zullo - jlz9811
//			Eduardo Rodrigues - emr4378
//			
//
#ifndef NIM_H
#define NIM_H

#include <vector>
#include "game.h"

/**
 * Nim
 *
 * A simple 2 player game in which players remove stones from different piles
 *
 * Rules:
 * - can take any # of stones from any pile, but only from one pile each turn.
 * - MUST always take at least 1 stone
 *
 * Actual Nim class only contains the rules of the specific game.
 */
class Nim : public Game< std::vector<int> > {

	public:
		Nim(std::vector<int> nStones);
		~Nim();

		bool IsEndBoard(std::vector<int> gB);
		std::vector< std::vector<int> > GenerateMoves(std::vector<int> gB,
									std::vector< std::vector<int> > *moves);
		std::vector<int> ReverseBoard(std::vector<int> gB);
		void PrintBoard(std::vector<int> gB);
		void ApplyMove(std::vector<int>* gB, std::vector<int> move);
		int ScoreGameBoard(std::vector<int>* gB);

	private:
		

};
#endif
