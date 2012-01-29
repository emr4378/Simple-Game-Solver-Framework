// File:         $Id: takeaway.cpp,v 1.1 2011/11/09 03:44:01 emr4378 Exp emr4378 $
// Revisions:
//              $Log: takeaway.cpp,v $
//              Revision 1.1  2011/11/09 03:44:01  emr4378
//              Initial revision
//
//              Revision 1.2  2011/11/02 03:41:28  p334-01o
//              Added comments.
//
//              Revision 1.2  2011/10/28 01:29:16  p334-01o
//              Fixed error where reported winner as loser
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
#include "game.h"
#include "takeaway.h"
#include "evaluator.h"

using namespace std;

#define ERR_ARG_NUM		1	//error, wrong # of args
#define ERR_INC_ARG		2	//error, incorrect argument
#define ERR_NOT_NUM		3 	//error, num_pennies NAN
#define ERR_NOT_POS		4	//error, num_pennies negative


/**
 * main
 *
 * This is where the magic happens. All input is handled in this beast of a
 * function. Does some basic error checking, then based on command line
 * arguments either lets user play game against AI, or just determines
 * best first move.
 *
 * usage: takeaway [play] num_pennies
 *
 * @param argc - int, # of command line args (including name of program
 * @param argv - char**, array consisting of cmd line args, in char* format
 * @return err - int, 0 if no error, else error code 
 */
int main(int argc, char** argv) {
	int err = 0;
	char *pEnd;
	int num_pennies = strtol(argv[argc-1], &pEnd, 10);

	if (argc >= 2 && argc <= 3) {
		if (num_pennies == 0 && pEnd == argv[argc-1]) {	
			err = ERR_NOT_NUM;
		} else if (num_pennies < 0) {
			err = ERR_NOT_POS;
		}

		if (argc == 3 && !err) {
			if(strcmp(argv[1], "play") == 0) {
			} else {
				err = ERR_INC_ARG;
			}
		}
	} else {
		err = ERR_ARG_NUM;
	}

	if (err == 0) {
		//initial error checking done; run program
		Takeaway game(num_pennies);
		Evaluator<int> eval(&game);

		if (argc == 2) {
			//non-interactive mode; find/print best move
			game.PrintBoard(*game.GetBoard());
			int bestMove;
			int result;
			int score;
			cout << "--------------" << endl;
			if (game.IsEndBoard(*game.GetBoard())) {
				cout << "Best Move: no move" << endl;
				score = 1;
			} else {
				score = eval.EvaluatePosition(*game.GetBoard(), 
													&bestMove, 
													&result);

				cout << "Best Move: " << bestMove << endl;
				game.PrintBoard(result);
			}
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
			
		} else {
			//interactive mode; play a game
			int move;
			int winner = 0;
			vector<int>::iterator iter;
			vector<int> validMoves;

			do {
				if (game.IsEndBoard(*game.GetBoard())) {
					winner = 1;
				} else {
					game.GenerateMoves(*game.GetBoard(), &validMoves);

					//Prompt for user input
					do {
						cout << endl << "Current Position" << endl 
							 << "----------------" << endl;
						game.PrintBoard(*game.GetBoard());
						cout << "----------------" << endl;

						cout << "Your move: " << endl;
						string input;
						cin >> input;
						move = atoi(input.c_str());

						iter = find(validMoves.begin(), 
									validMoves.end(), 
									move);

						if (iter == validMoves.end()) {
							cout << "Invalid move; try again." << endl;
						}

					} while (iter == validMoves.end());
				
					//Perform user move
					game.ApplyMove(game.GetBoard(), move);

					if (game.IsEndBoard(*game.GetBoard())) {
						winner = -1;
					} else {
						cout << endl << "Current Position" << endl 
							 << "----------------" << endl;
						game.PrintBoard(*game.GetBoard());
						cout << "----------------" << endl;

						//AI move
						int bestResult;
						eval.EvaluatePosition(*game.GetBoard(), 
												&move, 
												&bestResult);
						cout << "AI move: " << endl;
						game.PrintBoard(move);

						(*game.GetBoard()) = bestResult;
					}
				}
			} while (winner == 0);

			if (winner == 1) {
				cout << "You win!" << endl;
			} else {
				cout << "You lose." << endl;
			}

		}
	}

	if (err != 0) {
		//error found; print out useful message
		switch(err) {
			case ERR_ARG_NUM:
				cerr << "error: Incorrect number of arguments" << endl;
				break;
			case ERR_INC_ARG:
				cerr << "error: Wrong argument given" << endl;
				break;
			case ERR_NOT_NUM:
				cerr << "error: num_pennies must be valid number" << endl;
				break;
			case ERR_NOT_POS:
				cerr << "error: num_pennies must be positive" << endl;
				break;
			default:
				break;
		}
		cerr << "usage: takeaway [play] num_pennies" << endl;
	}

	return err;
}



//////////////////////////////Takeaway starts here/////////////////////////////


/**
 * Constructor
 *
 * @param nPennies - int, initial number of pennies in game
 */
Takeaway::Takeaway(int nPennies) : Game<int>(nPennies) {
	cout << "Takeaway created with " << *GetBoard() 
			<< " pennies" << endl;
}

/**
 * Destructor
 */
Takeaway::~Takeaway() {

}

/**
 * IsEndBoard
 *
 * Determines if gameboard is end state (# of pennies <= 0)
 *
 * @param gB - int, gameboard to be evaluated
 * @return bool, true if gB <= 0, false otherwise
 */
bool Takeaway::IsEndBoard(int gB){
	return (gB <= 0);
}

/**
 * GenerateMoves
 *
 * Looks at current gameboard and determines all valid moves and resulting
 * gameboards
 *
 * @param gB - int, gameboard to be evaluated
 * @param moves - vector<int>*, points to where potential moves will be stored
 * @return boards - vector<int>, contains all potential resulting gameboards
 */
vector<int> Takeaway::GenerateMoves(int gB, vector<int> *moves){
	moves->clear();
	vector<int> boards;

	//always can take 1
	boards.push_back(gB - 1);
	moves->push_back(1);

	if (gB % 3 == 0 && gB != 3) {
		//take 1/3
		boards.push_back(gB - gB/3);
		moves->push_back(gB/3);
	}

	if (gB % 2 == 0 && gB != 2) {
		//take 1/2
		boards.push_back(gB - gB/2);
		moves->push_back(gB/2);
	}

	return boards;
}

/**
 * ReverseBoard
 *
 * Takes a gameboard and reverses it to the other players' point of view
 * In Takeaway, board is same for both players so it just returns the passed
 * value and nothing happens.
 *
 * @param gB - int, the current gameboard (# of pennies)
 * @return gB - int, the current gameboard (# of pennies)
 */
int Takeaway::ReverseBoard(int gB){
	return gB;
}

/**
 * PrintBoard
 *
 * Prints the passed gameboard to standard output
 *
 * @param gB - int, the current gameboard (# of pennies)
 */
void Takeaway::PrintBoard(int gB) {
	cout << "Num Pennies on Board: " << gB << endl;
}

/**
 * ApplyMove
 *
 * Applys the move to the given gameboard
 *
 * @param gB - int* to the gameboard to be changed
 * @param move - int, the move to be applied
 */
void Takeaway::ApplyMove(int* gB, int move) {
	(*gB) -= move;
}

/**
 * ScoreGameBoard
 *
 * Determines the score of a given gameboard if the game were to end
 * right now. 
 * Returns 1 if gameboard is in end state, since that implies other
 * player took last piece (and player to take last piece loses, and -1
 * represents the score of a loser).
 *
 * @param gB - vector< vector<int> >*, the gameboard to be scored
 */
int Takeaway::ScoreGameBoard(int* gB){
	if(IsEndBoard(*gB)){
		return 1;
	}
}






