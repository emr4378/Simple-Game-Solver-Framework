// File:         $Id: evaluator.h,v 1.1 2011/11/09 03:44:01 emr4378 Exp emr4378 $
// Revisions:
//              $Log: evaluator.h,v $
//              Revision 1.1  2011/11/09 03:44:01  emr4378
//              Initial revision
//
//              Revision 1.1  2011/11/02 03:28:39  p334-01o
//              Initial revision
//
//              Revision 1.2  2011/10/28 01:28:41  p334-01o
//              Fixed issue where score was wrong
//
//              Revision 1.1  2011/10/19 21:02:18  p334-01o
//              Initial Check-in
//
//
// Authors: Jeffrey Zullo - jlz9811
//			Eduardo Rodrigues - emr4378
//			
//

#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <map>
#include <iostream>
#include <vector>
#include "game.h"

using namespace std;

/**
 * Evaluator
 *
 * Performs one real function; determining the best move, given a position
 *
 * Set up to use generics/templates so new 2-player games can be easily added
 * and make use of it.
 */
template <class T>
class Evaluator {
	public:
		
		/**
		 * Constructor
		 * 
		 * @param g - Game<T>*, points to local game instance for evaluation
		 */
		Evaluator(Game<T> *g) {
			game = g;
		}

		/**
		 * Destructor
		 */
		~Evaluator() {}

		/**
		 * EvaluatePosition
		 * 
		 * 
		 * @param gB - T, gameBoard to be evaluated
		 * @param bestMove - T*, pointer to the best move
 		 * @param resultBoard - T*, pointer to the best resulting board
		 * @return int - score of resulting move, assuming perfect play
		 * 				(-1 for loss, 0 for draw, 1 for win)
		 * 
		 */
		int EvaluatePosition(T gB, T *bestMove, T *resultBoard) {
			int ret = 0;
			int bestScore = -2;
			T curBestMove;
			T curResultBoard;

			if (memo.find(gB) != memo.end()) {
				ret = scoreMemo[ memo[gB] ];
				*bestMove = moveMemo[ memo[gB] ];
				*resultBoard = boardMemo[ memo[gB] ];
			} else {
				if (game->IsEndBoard(gB)) {
					ret = (*game).ScoreGameBoard(&gB); //-1;
				} else {
					vector<T> moves;
					vector<T> boards = game->GenerateMoves(gB, &moves);

					for (int i = 0; i < boards.size(); i++) {
						boards[i] = game->ReverseBoard(boards[i]);
						int score = -EvaluatePosition(boards[i], 
													  bestMove, 
													  resultBoard);
						if (score > bestScore) {
							curBestMove = moves[i];
							curResultBoard = boards[i];
							bestScore = score;
						}
					}
					*bestMove = curBestMove;
					*resultBoard = curResultBoard;
					ret = bestScore;

					memo.insert( pair<T, int>(gB, scoreMemo.size()) );
					scoreMemo.push_back(ret);
					moveMemo.push_back(*bestMove);
					boardMemo.push_back(*resultBoard);
				}
			}
			return ret;
		}

	private:
		Game<T> *game;
		map<T, int> memo;
		vector<int> scoreMemo;
		vector<T> moveMemo;
		vector<T> boardMemo;
};
#endif
