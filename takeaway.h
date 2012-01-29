// File:         $Id: takeaway.h,v 1.1 2011/11/09 03:44:01 emr4378 Exp emr4378 $
// Revisions:
//              $Log: takeaway.h,v $
//              Revision 1.1  2011/11/09 03:44:01  emr4378
//              Initial revision
//
//              Revision 1.1  2011/11/02 03:29:36  p334-01o
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

#ifndef TAKEAWAY_H
#define TAKEAWAY_H

#include "game.h"

/**
 * Takeaway
 *
 * A simple 2 player game in which players remove pennies from a pile.
 * The objective is to force the other play to take the last penny.
 *
 * Valid moves:
 * - if number of pennies in pile is multiple of 2, can take half
 * - if number of pennies in pile is multiple of 3, can take a third
 * - can always take one penny
 *
 * Actual Takeaway class only contains the rules of the specific game.
 */
class Takeaway : public Game<int> {

	public:
		Takeaway(int nPennies);
		~Takeaway();

		bool IsEndBoard(int gB);
		std::vector<int> GenerateMoves(int gB, std::vector<int> *moves);
		int ReverseBoard(int gB);
		void PrintBoard(int gB);
		void ApplyMove(int* gB, int move);
		int ScoreGameBoard(int* gb);

	private:

};
#endif
