// File:         $Id: crossout.cpp,v 1.1 2011/11/09 03:44:01 emr4378 Exp emr4378 $
// Revisions:	$Log: crossout.cpp,v $
// Revisions:	Revision 1.1  2011/11/09 03:44:01  emr4378
// Revisions:	Initial revision
// Revisions:
// Revisions:	Revision 1.1  2011/11/09 03:40:05  emr4378
// Revisions:	Initial revision
// Revisions:
// Revisions:	Revision 1.1  2011/11/09 02:44:42  emr4378
// Revisions:	Initial revision
// Revisions:
//
//
// Authors: Eduardo Rodrigues - emr4378
//			
//
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include "crossout.h"
#include "evaluator.h"

using namespace std;

#define ERR_ARG_NUM		1	//error, wrong # of args
#define ERR_NOT_NUM		3 	//error, argument NAN
#define ERR_NOT_POS		4	//error, argument negative

/**
 * main
 *
 * This is where the magic happens. All input is handled in this beast of a
 * function. Does some basic error checking, then based on command line
 * arguments either lets user play game against AI, or just determines
 * best first move.
 *
 * usage: crossout [play] max_num max_sum
 * 
 * Examples:
 * > crossout play 10 20
 * > crossout 4 6
 *
 * @param argc - int, # of command line args (including name of program
 * @param argv - char**, array consisting of cmd line args, in char* format
 * @return err - int, 0 if no error, else error code 
 */
int main(int argc, char** argv) {
	int err = 0;
	char *pEnd;
	int max_num;
	int max_sum;
	vector<int> initBoard;
	bool play = false;

	if (argc > 4 || argc < 3) {
		err = ERR_ARG_NUM;
	} else {
		if (strcmp(argv[1], "play") == 0) {
			play = true;
		}

		if ((play && argc != 4) || (!play && argc != 3)) {
			err = ERR_ARG_NUM;
		}
	}

	if (err == 0) {
		int tInd = (int)play + 1;
		max_num = strtol(argv[tInd], &pEnd, 10);
		if (max_num == 0 && pEnd == argv[tInd]) {
			err = ERR_NOT_NUM;
		} else if (max_num <= 0) {
			err = ERR_NOT_POS;
		}

		tInd = (int)play + 2;
		max_sum = strtol(argv[tInd], &pEnd, 10);
		if (max_sum == 0 && pEnd == argv[tInd]) {
			err = ERR_NOT_NUM;
		} else if (max_sum <= 0) {
			err = ERR_NOT_POS;
		}
	}

	//at this point play, max_num and max_sum & input checked for errors
	if (err == 0) {
		vector<int> initGB;
		for (int i = 1; i <= max_num; i++) {
			initGB.push_back(i);
		}

		Crossout game(initGB, max_sum);
		Evaluator< vector<int> > eval(&game);
		
		if (play) {
			//interactive mode; play a game
			int winner = 0;
			vector< vector<int> >::iterator iter;
			vector< vector<int> > validMoves;
			vector<int> move;

			do {
				game.GenerateMoves(*game.GetBoard(), &validMoves);

				//Prompt for user input
				do {
					cout << endl << "Current Position" << endl 
						 << "----------------" << endl;
					cout << "(max_sum: " << max_sum << ")" << endl;
					game.PrintBoard(*game.GetBoard());
					cout << "----------------" << endl;

					move.clear();

					cout << "Your move: " << endl;
					string input;
					getline(cin, input);

					int sPos;
					int oSPos = 0;


					do {
						sPos = input.find(' ', oSPos);
						move.push_back(
									atoi(input.substr(oSPos, sPos).c_str()));
						oSPos = sPos + 1;
					} while ( sPos != -1);

					sort(move.begin(), move.end());

					iter = find(validMoves.begin(), validMoves.end(), move);
					
					if (iter == validMoves.end()) {
						cout << "Invalid move; try again." << endl;
					}

				} while (iter == validMoves.end());

				cout << "Move: ";
				game.PrintBoard(move);

				//Perform user move
				game.ApplyMove(game.GetBoard(), move);

				if (game.IsEndBoard(*game.GetBoard())) {
					winner = 1;
				} else {
					cout << endl << "Current Position" << endl 
						 << "----------------" << endl;
					game.PrintBoard(*game.GetBoard());
					cout << "----------------" << endl;

					//AI move
					move.clear();
					move.resize(initBoard.size());

					vector<int> bestResult;
					eval.EvaluatePosition(*game.GetBoard(), 
											&move, 
											&bestResult);
					cout << "AI move: " << endl;
					game.PrintBoard(move);

					(*game.GetBoard()) = bestResult;

					if (game.IsEndBoard(*game.GetBoard())) {
						winner = -1;
					}
				}

			} while (winner == 0);


			if (winner == 1) {
				cout << "You win!" << endl;
			} else {
				cout << "You lose." << endl;
			}

		} else {
			//non-interactive mode; find/print best move
			vector<int> bestMove;
			vector<int> resultBoard;
			int score = eval.EvaluatePosition(*game.GetBoard(), 
												&bestMove, 
												&resultBoard);

			vector< vector<int> > moves;
			vector< vector<int> > boards = game.GenerateMoves(
															*game.GetBoard(), 
															&moves);

			cout << "Best Move: ";
			game.PrintBoard(bestMove);
			cout << "Result: ";
		 	game.PrintBoard(resultBoard);
			switch(score) {
				case 0:
					cout << "Draw" << endl;
					break;
				case 1:
					cout << "Win" << endl;
					break;
				case -1:
					cout << "Loss" << endl;
					break;
			}
		}
	} else {
		//error found; print out useful message
		switch(err) {
			case ERR_ARG_NUM:
				cerr << "error: Incorrect number of arguments" << endl;
				break;
			case ERR_NOT_NUM:
				cerr << "error: max_num & max_sum must be valid numbers" 
					 << endl;
				break;
			case ERR_NOT_POS:
				cerr << "error: max_num & max_sum must be positives" << endl;
				break;
			default:
				break;
		}
		cerr << "usage: crossout [play] max_num max_sum" << endl;
	}

	return err;
}

////////////////////////////Crossout starts here///////////////////////////////

/**
 * Constructor
 *
 * @param nums - vector<int>, initial game board
 * @param mSum - int, the maximum allowed sum for any move
 */
Crossout::Crossout(vector<int> nums, int mSum) : Game< vector<int> > (nums),
												 maxSum(mSum) {}
/**
 * Deconstructor, ka-pow!
 */
Crossout::~Crossout() {}

/**
 * IsEndBoard
 *
 * Determines if gameboard is end state.
 * End states are:
 *			- gB is empty, so no numbers left to remove
 *			- all numbers in gB are greater than maxSum
 *
 * Btw, we can cheat when checking for the second one, since gB is sorted
 * when it's created and will never change it's ordering.
 *
 * @param gB - vector<int>, gameboard to be evaluated
 * @return bool, true if end state, false otherwise
 */
bool Crossout::IsEndBoard(vector<int> gB) {
	return (gB.empty() || gB[0] > maxSum);
}

/**
 * GenerateMoves
 *
 * Looks at current gameboard and determines all valid moves and resulting
 * gameboards
 *
 * @param gB - vector<int>, gameboard to be evaluated
 * @param moves - vector< vector<int>* >, points to where potential moves 	
 *				  will be stored
 * @return boards - vector< vector<int> >, contains all potential resulting 
 *					gameboards
 */
vector< vector<int> > Crossout::GenerateMoves(vector<int> gB,
							vector< vector<int> > *moves) {

	moves->clear();
	vector< vector<int> > boards;

	for (int i = 0; i < gB.size() && gB[i] <= maxSum; i++) {
		vector<int> tempBoard = gB;
		vector<int> tempMove;

		tempMove.push_back(gB[i]);
		ApplyMove(&tempBoard, tempMove);

		moves->push_back(tempMove);
		boards.push_back(tempBoard);


		int tSum = tempMove[0];
		for(int j = i+1; j < gB.size() && tSum <= maxSum; j++) {
			tSum = tempMove[0];
			tempBoard = gB;

			tempMove.push_back(gB[j]);
			tSum += tempMove[1];

			if (tSum <= maxSum) {
				ApplyMove(&tempBoard, tempMove);

				moves->push_back(tempMove);
				boards.push_back(tempBoard);

				tempMove.pop_back();
			}
		}
	
	}

	return boards;
}

/**
 * ReverseBoard
 *
 * Takes a gameboard and reverses it to the other players' point of view
 * In Nim, board is same for both players so it just returns the passed
 * value and nothing happens.
 *
 * @param gB - vector<int>, the current gameboard
 * @return gB - vector<int>, the current gameboard
 */
vector<int> Crossout::ReverseBoard(vector<int> gB) {
	return gB;
}

/**
 * PrintBoard
 *
 * Prints the passed gameboard to standard output
 *
 * @param gB - vector<int>, the current gameboard (list of numbers)
 */
void Crossout::PrintBoard(vector<int> gB) {
	for (int i = 0; i < gB.size(); i++) {
		cout << gB[i] << " ";
	}
	cout << endl;
}

/**
 * ApplyMove
 *
 * Applies the move to the given gameboard
 *
 * @param gB - vector<int>* to the gameboard to be changed
 * @param move - vector<int>, the move to be applied
 */
void Crossout::ApplyMove(vector<int>* gB, vector<int> move) {
	vector<int>::iterator iter;

	for (int i = 0; i < move.size(); i++) {
		iter = find(gB->begin(), gB->end(), move[i]);
		gB->erase(iter);
	}
}

/**
 * ScoreGameBoard
 *
 * Determines the score of a given gameboard if the game were to end
 * right now. 
 * Returns -1 if gameboard is in end state, since that implies other
 * player took last piece (and player to take last piece win, and -1
 * represents the score of a loser).
 *
 * @param gB - vector<int>*, the gameboard to be scored
 */
int Crossout::ScoreGameBoard(vector<int>* gB) {
	if(IsEndBoard(*gB)){
		return -1;
	}
}


