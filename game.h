// File:         $Id: game.h,v 1.1 2011/11/09 03:44:01 emr4378 Exp emr4378 $
// Revisions:
//              $Log: game.h,v $
//              Revision 1.1  2011/11/09 03:44:01  emr4378
//              Initial revision
//
//              Revision 1.1  2011/11/02 03:28:59  p334-01o
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

#ifndef GAME_H
#define GAME_H

#include <vector>

/**
 * Game
 *
 * An abstract implementation of a 2-player game for this framework.
 * Lists all the methods a game would need to implement in order to function
 * properly.
 * 
 * Templated to easily adapt to games with different types of game boards
 * (Takeaway - ints, Othello - 2d vector, Nim - 1d vector)
 *
 * Contains 1 non-abstract/virtual method, GetBoard, as all games will need to
 * access the stored gameboard
 */
template <class T>
class Game {

	public:
		/**
		 * Constructor
		 *
		 * @param gB - T, initial gameboard to be stored
		 */
		Game(T gB) : gameBoard(gB) {};

		/**
		 * Virtual Destructor
		 *
		 * Should be implemented by child classes, but not required
		 */
		virtual ~Game() {};

		/**
		 * Pure-Virtual IsEndBoard
		 *
		 * Checks if the gameBoard is an game ending position
		 * Must be implemented by child classes
		 *
		 * @param gB - T, the given gameboard
		 * @return bool - true if end position, false otherwise
		*/
		virtual bool IsEndBoard(T gB) = 0;

		/**
		 * Pure-Virtual GenerateMoves
		 *
		 * Generates all possible positions from given position
		 * Must be implemented by child classes
		 *
		 * @param gB - T, the given gameboard
		 * @param moves - vector<T>, will contain all the potential moves
		 * @return vector<T> - all possible resulting positions
		 */
		virtual std::vector<T> GenerateMoves(T gB, std::vector<T> *moves) = 0;

		/**
		 * Pure-Virtual ReverseBoard
		 * 
		 * Reverses the gameboard to POV of other player
		 * Must be implemented by child classes
		 *
		 * @param gB - T, the given gameboard
		 * @return T - gameboard, reversed
		*/
		virtual T ReverseBoard(T gB) = 0;

		/**
		 * Pure-Virtual PrintBoard
		 * 
		 * Prints board to standard out
		 * Must be implemented by child classes
		 * 
		 * @param gB - T, the gameboard to be printed
		 */
		virtual void PrintBoard(T gB) = 0;

		/**
		 * Pure-Virtual ApplyMove
		 *
 		 * Applies the given move to the given gameboard
		 * Must be implemented by child classes
		 *
		 * @param gB - T* to the gameboard to be changed
		 * @param move - T, the move to apply
		 */
		virtual void ApplyMove(T* gB, T move) = 0;
		
		/**
		 * Pure-Virtual ScoreGameBoard
		 * 
		 * Scores the given end-position gameboard
		 * Must be implemented by child classes
		 *
		 * @param gB - T* to the gameboard that should be evaluated.
		 * @return integer score for this ending gameboard
		 */
		virtual int ScoreGameBoard(T* gB) = 0;

		/**
		 * GetBoard
		 * 
		 * @return &gameboard, T* to stored gameBoard
		 */
		T* GetBoard() { return &gameBoard; };

	private:
		T gameBoard;	//the stored gameboard

};
#endif
