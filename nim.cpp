// File:         $Id: nim.cpp,v 1.1 2011/11/09 03:44:01 emr4378 Exp emr4378 $
// Revisions:
//              $Log: nim.cpp,v $
//              Revision 1.1  2011/11/09 03:44:01  emr4378
//              Initial revision
//
//              Revision 1.2  2011/11/02 03:41:17  p334-01o
//              Added comments.
//
//              Revision 1.2  2011/10/28 01:29:44  p334-01o
//              Fixed issues where everything was awful
//
//              Revision 1.1  2011/10/19 21:02:18  p334-01o
//              Initial Check-in
//
//
// Authors: Jeffrey Zullo - jlz9811
//			Eduardo Rodrigues - emr4378
//			
//

#include <algorithm>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "nim.h"
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
 * usage: nim [play] piles
 * 
 * Examples:
 * > nim play 5 1 3
 * > nim 2 4 1 6 3 2
 * > nim play 23
 *
 * @param argc - int, # of command line args (including name of program
 * @param argv - char**, array consisting of cmd line args, in char* format
 * @return err - int, 0 if no error, else error code 
 */
int main(int argc, char** argv) {
	int err = 0;
	char *pEnd;
	int num_stones;
	vector<int> initBoard;
	bool play = false;

	if (argc <= 1) {
		err = ERR_ARG_NUM;
	}

	if(err == 0 && strcmp(argv[1], "play") == 0) {
		play = true;
	}

	if (play && argc <= 2) {
		err = ERR_ARG_NUM;
	}

	for (int i = (int)play + 1; i < argc && err == 0; i++) {
		num_stones = strtol(argv[i], &pEnd, 10);

		if (num_stones == 0 && pEnd == argv[i]) {	
			err = ERR_NOT_NUM;
		} else if (num_stones < 1) {
			err = ERR_NOT_POS;
		}

		initBoard.push_back(num_stones);
	}

	if (err == 0) {
		Nim game(initBoard);
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
					game.PrintBoard(*game.GetBoard());
					cout << "----------------" << endl;

					move.clear();
					move.resize(initBoard.size());

					cout << "Your move: " << endl;
					char pos[256];
					char amt[256];
					string input;
					cin.getline(pos, 256, ' ');
					cin.getline(amt, 256);
					
					move[ atoi(pos) ] = atoi(amt);

					iter = find(validMoves.begin(), validMoves.end(), move);
					
					if (iter == validMoves.end()) {
						cout << "Invalid move; try again." << endl;
					}

				} while (iter == validMoves.end());


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
	}

	if (err != 0) {
		//error found; print out useful message
		switch(err) {
			case ERR_ARG_NUM:
				cerr << "error: Incorrect number of arguments" << endl;
				break;
			case ERR_NOT_NUM:
				cerr << "error: each num_stones must be valid number" << endl;
				break;
			case ERR_NOT_POS:
				cerr << "error: each num_stones must be positive" << endl;
				break;
			default:
				break;
		}
		cerr << "usage: nim [play] num_stones [num_stones]..." << endl;
	}

	return err;
}



//////////////////////////////Nim starts here//////////////////////////////////


/**
 * Constructor
 *
 * @param nStones - vector<int>, contains # of stones in each pile
 */
Nim::Nim(vector<int> nStones) : Game< vector<int> >(nStones) { 
}

/**
 * Destructor
 */
Nim::~Nim() { 
}

/**
 * IsEndBoard
 *
 * Determines if gameboard is end state (# of stones, for all piles <= 0)
 *
 * @param gB - vector<int>, gameboard to be evaluated
 * @return bool, true if no stones in any pile, false otherwise
 */
bool Nim::IsEndBoard(vector<int> gB) { 
	bool ret = true;
	for (int i = 0; i < gB.size() && ret; i++) {
		if (gB[i] != 0) {
			ret = false;
		}
	}

	return ret;
}

/**
 * GenerateMoves
 *
 * Looks at current gameboard and determines all valid moves and resulting
 * gameboards
 *
 * @param gB - vector<int>, gameboard to be evaluated
 * @param moves - vector< vector<int>* >, points to where potential moves will be stored
 * @return boards - vector< vector<int> >, contains all potential resulting gameboards
 */
vector< vector<int> > Nim::GenerateMoves(vector<int> gB,
										vector< vector<int> > *moves) { 

	moves->clear();
	vector< vector<int> > boards;

	for (int i = 0; i < gB.size(); i++) {
		for (int j = 1; j <= gB[i]; j++) {
			vector<int> tempBoard = gB;
			vector<int> tempMove (gB.size(), 0);
			tempBoard[i] -= j;
			tempMove[i] = j;

			moves->push_back(tempMove);
			boards.push_back(tempBoard);
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
vector<int> Nim::ReverseBoard(vector<int> gB) { 
	return gB;
}

/**
 * PrintBoard
 *
 * Prints the passed gameboard to standard output
 *
 * @param gB - vector<int>, the current gameboard (# of stones in each pile)
 */
void Nim::PrintBoard(vector<int> gB) { 
	for (int i = 0; i < gB.size(); i++) {
		cout << "(" << gB[i] << ") ";
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
void Nim::ApplyMove(vector<int>* gB, vector<int> move) {
	for (int i = 0; i < move.size(); i++) {
		(*gB)[i] -= move[i];
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
 * @param gB - vector< vector<int> >*, the gameboard to be scored
 */
int Nim::ScoreGameBoard(vector<int>* gB){
	if(IsEndBoard(*gB)){
		return -1;
	}
}
