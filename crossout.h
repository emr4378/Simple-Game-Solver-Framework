// File:         $Id: crossout.h,v 1.1 2011/11/09 03:44:01 emr4378 Exp emr4378 $
// Revisions:	$Log: crossout.h,v $
// Revisions:	Revision 1.1  2011/11/09 03:44:01  emr4378
// Revisions:	Initial revision
// Revisions:
// Revisions:	Revision 1.1  2011/11/09 03:40:05  emr4378
// Revisions:	Initial revision
// Revisions:
//
//
// Authors: Eduardo Rodrigues - emr4378
//			
//
#ifndef CROSSOUT_H
#define CROSSOUT_H

#include <vector>
#include "game.h"

/**
 * Crossout
 *
 * A simple 2 player game in which players remove numbers from a list
 *
 * Rules:
 * - can take up to 2 numbers from the list, so long as the numbers sum 
 *	 isn't higher than max_sum
 * - MUST always take at least 1 number
 *
 * Actual Crossout class only contains the rules of the specific game.
 */
class Crossout : public Game< std::vector<int> > {

	public:
		Crossout(std::vector<int> nums, int mSum);
		~Crossout();

		bool IsEndBoard(std::vector<int> gB);
		std::vector< std::vector<int> > GenerateMoves(std::vector<int> gB,
									std::vector< std::vector<int> > *moves);
		std::vector<int> ReverseBoard(std::vector<int> gB);
		void PrintBoard(std::vector<int> gB);
		void ApplyMove(std::vector<int>* gB, std::vector<int> move);
		int ScoreGameBoard(std::vector<int>* gB);

	private:
		int maxSum;

};
#endif
