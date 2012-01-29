// File:         $Id:
// Revisions:
//              $Log:
//
//
// Authors: Jeffrey Zullo - jlz9811
//			Eduardo Rodrigues - emr4378
//			
//

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include "othello.h"
#include "evaluator.h"

using namespace std;

#define ERR_ARG_NUM		1	//error, wrong # of args
#define ERR_NO_FILE		3 	//error, input file not found
#define ERR_BRD_HGT		4	//error, board height <= 0
#define ERR_BRD_WID     5   //error, board width <= 0

/**
 * main
 *
 * This is where the magic happens. All input is handled in this beast of a
 * function. Does some basic error checking, then based on command line
 * arguments either lets user play game against AI, or just determines
 * best first move.
 *
 * usage: othello [play] { - | <file-name> }
 * 
 * Examples:
 * > othello -
 * > othello play -
 * > othello myfile.txt
 * > othello play myfile2.txt
 *
 * @param argc - int, # of command line args (including name of program)
 * @param argv - char**, array consisting of cmd line args, in char* format
 * @return err - int, 0 if no error, else error code 
 */
int main(int argc, char** argv) {
	int err = 0;
	char *pEnd;

	bool play = false;
	bool manualInput = false;
	char* filename;

	if (argc <= 1 || argc > 3) {
		err = ERR_ARG_NUM;
	}

	if (err == 0 && strcmp(argv[1], "play") == 0) {
		play = true;
	} else if (err == 0 && strcmp(argv[1], "-") == 0) {
		manualInput = true;
	} else {
		filename = argv[1];
	}

	if (play && argc <= 2) { //No file or lack there-of was specified.
		err = ERR_ARG_NUM;
	} else if (play && strcmp(argv[2], "-") == 0) {
		manualInput = true;
	} else if (play) {
		filename = argv[2];
	}
	
	vector< vector<int> > initBoard;
	if (err == 0 && manualInput) {
		int height;
		int width;
		
		cout << "Enter board dimensions:" << endl;
		char h[256];
		char w[256];
		cin.getline(w, 256, ' ');
		cin.getline(h, 256);
		
		height = atoi(h);
		width = atoi(w);
		
		if (height < 1) {
			err = 4;
		} else if (width < 1){
			err = 5;
		} else {
			cout << "Enter board setup:" << endl;
		}
		for (int i = 0; i < height && err == 0; i++) {
			initBoard.push_back(vector<int>(width, 0));
			for (int j = 0; j < width - 1; j++) {
				char token[2];
				cin.getline(token, 2, ' ');
				if (strcmp(token, "X") == 0) {
					initBoard[i][j] = 1;
				} else if (strcmp(token, "O") == 0) {
					initBoard[i][j] = -1;
				}
			}
			char token[2];
			cin.getline(token, 2);
			if (strcmp(token, "X") == 0) {
				initBoard[i][width - 1] = 1;
			} else if (strcmp(token, "O") == 0) {
				initBoard[i][width - 1] = -1;
			}
		}
	} else {
		ifstream filestream(filename, fstream::in); //not being made, idk,
		if( filestream.is_open() ){
		
			int height;
			int width;

			char h[256];
			char w[256];
			filestream.getline(w, 256, ' ');
			filestream.getline(h, 256);
			
			height = atoi(h);
			width = atoi(w);
			
			if (height < 1) {
				err = 4;
			} else if (width < 1){
				err = 5;
			}

			for (int i = 0; i < height && err == 0; i++) {
				initBoard.push_back(vector<int>(width, 0));
				for (int j = 0; j < width - 1; j++) {
					char token[2];
					filestream.getline(token, 2, ' ');
					if (strcmp(token, "X") == 0) {
						initBoard[i][j] = 1;
					} else if (strcmp(token, "O") == 0) {
						initBoard[i][j] = -1;
					}
				}
				char token[2];
				filestream.getline(token, 2);
				if (strcmp(token, "X") == 0) {
					initBoard[i][width - 1] = 1;
				} else if (strcmp(token, "O") == 0) {
					initBoard[i][width - 1] = -1;
				}
			}
			filestream.close();
		} else {
			err == 3;
		}
	}
	
	if (err == 0) {
		Othello game(initBoard);
		Evaluator< vector< vector<int> > > eval(&game);

		if (play) {
			//interactive mode; play a game
			int winner = 0;
			bool isOver = false;
			vector< vector< vector<int> > >::iterator iter;
			vector< vector< vector<int> > > validMoves;
			vector< vector<int> > move;
			vector<int> moveVector;

			do {
				game.GenerateMoves(*(game.GetBoard()), &validMoves);

				//Prompt for user input
				do {
					cout << endl << "Current Position" << endl 
						 << "----------------" << endl;
					game.PrintBoard(*game.GetBoard());
					cout << "----------------" << endl;

					move.clear();
					moveVector.clear();

					cout << "Your move: " << endl;
					int height;
					int width;

					char h[256];
					char w[256];
					cin.getline(w, 256, ' ');
					cin.getline(h, 256);
					
					height = atoi(h);
					width = atoi(w);
					
					moveVector.push_back(height);
					moveVector.push_back(width);
					move.push_back(moveVector);
					
					bool found = false;
					for (iter = validMoves.begin(); !found && iter != validMoves.end(); iter++) {
						if ((*iter)[0][0] == moveVector[0] && (*iter)[0][1] == moveVector[1]) {
							found = true;
							break; //don't increment the iterator past this one, prevents an off by one error for last element.
						}
					}

					//iter = find(validMoves.begin(), validMoves.end(), move);
					
					if (iter == validMoves.end()) {
						cout << "Invalid move; try again." << endl;
					}

				} while (iter == validMoves.end());


				//Perform user move
				game.ApplyMove(game.GetBoard(), move);

				if (game.IsEndBoard(*game.GetBoard())) {
					//winner = game.ScoreGameBoard(game.GetBoard());
					isOver = true;
				} else {
					cout << endl << "Current Position" << endl 
						 << "----------------" << endl;
					game.PrintBoard(*game.GetBoard());
					cout << "----------------" << endl;

					//AI move
					(*game.GetBoard()) = game.ReverseBoard(*game.GetBoard());
					move.clear();

					vector< vector<int> > bestResult;
					eval.EvaluatePosition(*game.GetBoard(), 
											&move, 
											&bestResult);
					cout << "AI move: " << endl;
					cout << "Row- " << move[0][0] << " Column- " << move[0][1] << endl;

					(*game.GetBoard()) = bestResult;
					//(*game.GetBoard()) = game.ReverseBoard(*game.GetBoard());
					game.PrintBoard(*game.GetBoard());

					if (game.IsEndBoard(*game.GetBoard())) {
						//winner = game.ScoreGameBoard(game.GetBoard());
						
						isOver = true;
					}
				}

			} while (!isOver);

			winner = game.ScoreGameBoard(game.GetBoard());
			switch(winner) {
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
			//non-interactive mode; find/print best move
			vector< vector<int> > bestMove;
			vector< vector<int> > resultBoard;
			int score = eval.EvaluatePosition(*game.GetBoard(), 
												&bestMove, 
												&resultBoard);

			vector< vector< vector<int> > > moves;
			vector< vector< vector<int> > > boards = game.GenerateMoves(
															*game.GetBoard(), 
															&moves);

			for (int i = 0; i < boards.size(); i++) {
				game.PrintBoard(boards[i]);
			}

			cout << "Best Move: Row- " << bestMove[0][0] << " Column- " << bestMove[0][1] << endl;
			cout << "Result: " << endl;
		 	game.PrintBoard(game.ReverseBoard(resultBoard));
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
				cerr << "error: incorrect number of arguments" << endl;
				break;
			case ERR_NO_FILE:
				cerr << "error: file not found" << endl;
				break;
			case ERR_BRD_HGT:
				cerr << "error: the board height must be positive" << endl;
				break;
			case ERR_BRD_WID:
				cerr << "error: the board width must be positive" << endl;
				break;
			default:
				break;
		}
		cerr << "usage: othello [play] {-|filename}" << endl;
	}
	return err;
}



//////////////////////////////Othello starts here//////////////////////////////////


/**
 * Constructor
 *
 * @param startingBoard - vector< vector<int> >, contains the starting board position as read in from the user
 */
Othello::Othello(vector< vector<int> > startingBoard) : Game< vector< vector<int> > >(startingBoard) { 
}

/**
 * Destructor
 */
Othello::~Othello() { 
}

/**
 * IsEndBoard
 *
 * Determines if gameboard is end state (No more places left to place a piece.)
 *
 * @param gB - vector< vector<int> >, game board to be evaluated. 0 represents an empty space.
 * @return bool, true if no open spaces, false otherwise.
 */
bool Othello::IsEndBoard(vector< vector<int> > gB) { 
	for (int i = 0; i < gB.size(); i++) {
		for (int j = 0; j < gB[i].size(); j++){
			if (gB[i][j] == 0) {
				return false;
			}
		}
	}
	return true;
}

/**
 * GenerateMoves
 *
 * Looks at current gameboard and determines all valid moves and resulting
 * gameboards
 *
 * @param gB - vector< vector<int> >, gameboard to be evaluated
 * @param moves - vector< vector< vector<int> > >*, points to where potential moves will be stored
 * @return boards - vector< vector< vector<int> > >, contains all potential resulting gameboards
 */
vector< vector< vector<int> > > Othello::GenerateMoves(vector< vector<int> > gB, vector< vector< vector<int> > > *moves) { 
	moves->clear();
	vector< vector< vector<int> > > boards;

	for (int i = 0; i < gB.size(); i++) {
		for (int j = 0; j < gB[i].size(); j++) {
			if (gB[i][j] == 0){
				vector< vector<int> > tempBoard = gB;
				vector< vector<int> > moveVector;
				vector<int> tempMove(2, 0); //row, col

				tempMove[0] = i;
				tempMove[1] = j;

				moveVector.push_back(tempMove);
				ApplyMove(&tempBoard, moveVector);

				moves->push_back(moveVector);
				boards.push_back(tempBoard);
			}
		}
	}

	return boards;
}

/**
 * ReverseBoard
 *
 * Takes a gameboard and reverses it to the other players' point of view
 * In Othello, this means swapping the values of X (1) and O (-1).
 *
 * @param gB - vector< vector<int> >, the current gameboard
 * @return gB - vector< vector<int> >, the reversed gameboard
 */
vector< vector<int> > Othello::ReverseBoard(vector< vector<int> > gB) { 
	vector< vector<int> > newBoard;
	for (int i = 0; i < gB.size(); i++) {
		newBoard.push_back(vector<int>(gB[i].size(), 0));
		for (int j = 0; j < gB[i].size(); j++) {
			newBoard[i][j] = gB[i][j] * -1;
		}
	}
	return newBoard;
}

/**
 * PrintBoard
 *
 * Prints out a representation of the game board to standard out.
 *
 * @param gB - vector< vector<int> >, the current gameboard
 */
void Othello::PrintBoard(vector< vector<int> > gB) { 
	for (int i = 0; i < gB.size(); i++) {
		for (int j = 0; j < gB[i].size(); j++) {
			if (gB[i][j] == -1) {
				cout << "O";
			} else if (gB[i][j] == 0) {
				cout << ".";
			} else if (gB[i][j] == 1) {
				cout << "X";
			}
		}
		cout << endl;
	}
	cout << endl;
}

/**
 * ApplyMove
 *
 * Applies the move to the given gameboard
 * Directions are numbered like so:
 * 1 2 3
 * 4 X 5
 * 6 7 8
 *
 * where X is the piece that has just been placed.
 *
 * @param gB - vector< vector<int> >*, the gameboard to be changed
 * @param move - vector<int>, the move to be applied
 */
void Othello::ApplyMove(vector< vector<int> >* gB, vector< vector<int> > move) {
	const int row = move[0][0];
	const int col = move[0][1];
	const int height = (*gB).size();
	const int width = (*gB)[0].size();
	
	(*gB)[row][col] = 1;
	
	//Direction 1 (North-West)
	int currentRow = row - 1;
	int currentCol = col - 1;
	int furthestRow = row;
	int furthestCol = col;
	bool done = false;
	
	//TODO: Convert to function?
	while (!done && currentRow >= 0 && currentCol >= 0) {
		int val = (*gB)[currentRow][currentCol];
		if (val == 0){
			done = true;			
		} else if (val == 1){
			done = true;
			furthestRow = currentRow;
			furthestCol = currentCol;
		} else {
			currentRow--;
			currentCol--;
		}
	}
	if (furthestRow != row || furthestCol != col) {
		currentRow = row - 1;
		currentCol = col - 1;
		while(currentRow != furthestRow && currentCol != furthestCol) {
			(*gB)[currentRow][currentCol] = 1;
			currentRow--;
			currentCol--;
		}
	}
	
	//Direction 2 (North)
	currentRow = row - 1;
	furthestRow = row;
	done = false;
	
	//TODO: Convert to function?
	while (!done && currentRow >= 0) {
		int val = (*gB)[currentRow][col];
		if (val == 0){
			done = true;			
		} else if (val == 1){
			done = true;
			furthestRow = currentRow;
		} else {
			currentRow--;
		}
	}
	if (furthestRow != row) {
		currentRow = row - 1;
		while(currentRow != furthestRow) {
			(*gB)[currentRow][col] = 1;
			currentRow--;
		}
	}
	
	//Direction 3 (North-East)
	currentRow = row - 1;
	currentCol = col + 1;
	furthestRow = row;
	furthestCol = col;
	done = false;
	
	//TODO: Convert to function?
	while (!done && currentRow >= 0 && currentCol < width) {
		int val = (*gB)[currentRow][currentCol];
		if (val == 0){
			done = true;			
		} else if (val == 1){
			done = true;
			furthestRow = currentRow;
			furthestCol = currentCol;
		} else {
			currentRow--;
			currentCol++;
		}
	}
	if (furthestRow != row || furthestCol != col) {
		currentRow = row - 1;
		currentCol = col + 1;
		while(currentRow != furthestRow && currentCol != furthestCol) {
			(*gB)[currentRow][currentCol] = 1;
			currentRow--;
			currentCol++;
		}
	}
	
	//Direction 4 (West)
	currentCol = col - 1;
	furthestCol = col;
	done = false;
	
	//TODO: Convert to function?
	while (!done && currentCol >= 0) {
		int val = (*gB)[row][currentCol];
		if (val == 0){
			done = true;			
		} else if (val == 1){
			done = true;
			furthestCol = currentCol;
		} else {
			currentCol--;
		}
	}
	if (furthestCol != col) {
		currentCol = col - 1;
		while(currentCol != furthestCol) {
			(*gB)[row][currentCol] = 1;
			currentCol--;
		}
	}
	
	//Direction 5 (East)
	currentCol = col + 1;
	furthestCol = col;
	done = false;
	
	//TODO: Convert to function?
	while (!done && currentCol < width) {
		int val = (*gB)[row][currentCol];
		if (val == 0){
			done = true;			
		} else if (val == 1){
			done = true;
			furthestCol = currentCol;
		} else {
			currentCol++;
		}
	}
	if (furthestCol != col) {
		currentCol = col + 1;
		while(currentCol != furthestCol) {
			(*gB)[row][currentCol] = 1;
			currentCol++;
		}
	}
	
	//Direction 6 (South-West)
	currentRow = row + 1;
	currentCol = col - 1;
	furthestRow = row;
	furthestCol = col;
	done = false;
	
	//TODO: Convert to function?
	while (!done && currentRow < height && currentCol >= 0) {
		int val = (*gB)[currentRow][currentCol];
		if (val == 0){
			done = true;			
		} else if (val == 1){
			done = true;
			furthestRow = currentRow;
			furthestCol = currentCol;
		} else {
			currentRow++;
			currentCol--;
		}
	}
	if (furthestRow != row || furthestCol != col) {
		currentRow = row + 1;
		currentCol = col - 1;
		while(currentRow != furthestRow && currentCol != furthestCol) {
			(*gB)[currentRow][currentCol] = 1;
			currentRow++;
			currentCol--;
		}
	}
	
	//Direction 7 (South)
	currentRow = row + 1;
	furthestRow = row;
	done = false;
	
	//TODO: Convert to function?
	while (!done && currentRow < height) {
		int val = (*gB)[currentRow][col];
		if (val == 0){
			done = true;			
		} else if (val == 1){
			done = true;
			furthestRow = currentRow;
		} else {
			currentRow++;
		}
	}
	if (furthestRow != row) {
		currentRow = row + 1;
		while(currentRow != furthestRow) {
			(*gB)[currentRow][col] = 1;
			currentRow++;
		}
	}
	
	//Direction 8 (South-East)
	currentRow = row + 1;
	currentCol = col + 1;
	furthestRow = row;
	furthestCol = col;
	done = false;
	
	//TODO: Convert to function?
	while (!done && currentRow < height && currentCol < width) {
		int val = (*gB)[currentRow][currentCol];
		if (val == 0){
			done = true;			
		} else if (val == 1){
			done = true;
			furthestRow = currentRow;
			furthestCol = currentCol;
		} else {
			currentRow++;
			currentCol++;
		}
	}
	if (furthestRow != row || furthestCol != col) {
		currentRow = row + 1;
		currentCol = col + 1;
		while(currentRow != furthestRow && currentCol != furthestCol) {
			(*gB)[currentRow][currentCol] = 1;
			currentRow++;
			currentCol++;
		}
	}
}

/**
 * ScoreGameBoard
 *
 * Determines the score of a given gameboard for player 1 if it were to end right now.
 * The score is 1 if there are more X's than O's, -1 if there are more O's than X's,
 * and 0 if they are tied.
 *
 * @param gB - vector< vector<int> >*, the gameboard to be scored
 */
int Othello::ScoreGameBoard(vector< vector<int> > *gB) {
	int xCount = 0;
	int oCount = 0;
	
	for (int i = 0; i < (*gB).size(); i++) {
		for (int j = 0; j < (*gB)[i].size(); j++) {
			if ((*gB)[i][j] == -1) {
				oCount++;
			} else if ((*gB)[i][j] == 1) {
				xCount++;
			}
		}
	}
	
	if (xCount < oCount) {
		return -1;
	} else if (xCount == oCount) {
		return 0;
	} else if (xCount > oCount) {
		return 1;
	}
}
