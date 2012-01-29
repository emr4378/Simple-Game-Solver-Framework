CS4 Project
-Takeaway, Nim, Othello and Crossout-

Eduardo Rodrigues - emr4378
Jeffrey Zullo - jlz9811 (Nim and Othello only)

*NOTE: All the "EDIT" entries list design changes over course of project*


------------------------------------
------------Instructions------------
------------------------------------

--------------
---Takeaway---
--------------

Takeaway can be run using the following:

usage: takeaway [play] num_pennies

Where num_pennies is some interger values greater than 0. The [play] paramater is surround by brackets to indicate that it is optional. When used the program will play against the user, prompting for input and then choosing the best move possible.

Example usages:
> takeaway play 11
> takeaway 6

--------------
------Nim-----
--------------

Nim can be run using the following:

usage: nim [play] num_stones [num_stones]...

Where num_stones is some integer value greater than 0. [num_stones]... indicates that you can specify any number of piles.

The [play] parameter is optional and when used will cause the program to enter an interactive mode, prompting for input from the user to play the game against the algorithm/AI.

When in interactive mode, you enter your move in the format:

index amount

Where index is the 0-based index of the pile to remove from, and amount is the number of stones to remove from the pile.

Example usages:
> nim play 2 5 1 10
> nim 5 2 4 3

-------------
---Othello---
-------------

Othello can be run using the following:

usage: othello [play] {-|filename}

Where filename is the path to the file to read from. The empty file parameter (-) is specified, the game will prompt you 
to enter a board size like so:

Enter board dimensions:
[rowCount] [columnCount]

Where rowCount is the number of rows in the board, followed by a space, followed by columnCount, which is the number of
columns in the board.
Following this, the game will prompt you to define the initial board setup. For each row, enter one character for each column
with each separated by a space. Valid characters are X (your piece), O (the opponent's piece), and . (empty space).
For example:
. . . .   or
. X . .   or
O X O X   etc.

To complete entering a row, hit enter. Repeat for each row.

The [play] parameter is optional an when used will cause the program to enter an interactive mode, prompting for input from the user
to play the game against the algorithm/AI.

When in interactive mode, you enter your move in the format:

row column

Where the space designated by (row, column) is an empty space (.). Row and Column are 0-based indicies. For example:
Given the board:
. . . .
. X O .
. O X .
. . . .

you want to place a piece in the top left corner. To complete this action, you would enter
0 0
when prompted for your move.
The bottom right corner would be the move
3 3

Example Usages:
>othello -
>othello play -
>othello myfile.txt
>othello play myfile2.txt

--------------
---Crossout---
--------------

Crossout can be run using the following:

usage: crossout [play] max_num max_sum 

Where max_num and max_sum are positive integers. The initial game board will consist of numbers in range of [1, max_num], inclusive. max_sum specifies the maximum sum that a players move can add up to.

The [play] parameter is optional and when used will cause the program to enter an interactive mode, prompting for input from the user to play the game against the algorithm/AI.

When in interactive mode, you enter your move in the format:

num_1 [num_2]

Where both num_1 and num_2 are numbers that are present on the gameboard. The player is required to make a move, so num_1 is required. However num_2 is not needed; it should only be given if the player wants to remove more than one number for their turn. num_1 and num_2 can be specified in any order (ascending or descending); it makes no difference.

Example usages:
> crossout play 10 20
> crossout 4 6

------------------------------------
------------Design Summary----------
------------------------------------
The framework is designed with simplicity and ease of adding in new games in mind.

There are two main framework components, game.h and evaluator.h.

---------
game.h
---------
EDIT: New method was added; ScoreGameBoard. Othello requires a more complex way of scoring the board than simply determining who made the last move in the game. To this end, the Game objects had to provide a way of properly scoring the resulting boards for each game. The Nim and Takeaway scoring methods remain simple while the Othello scoring method evaluates a board to determine who is winning/has won.

EDIT: New method was added; ApplyMove. This was made because Takeaway made an assumption that applying a move would always be as simple as subtracting the move from the board (integer subtraction). Nim uses a vector of integers, so you can't apply a move like that.

The Game class is an abstract class. It is meant to be the base class for all games this framework can handle (Takeaway, Nim, Othello, etc).

It contains methods for things that every game would require to use the framework. These methods are:

	IsEndBoard
	GenerateMoves
	ReverseBoard
	PrintBoard
	GetBoard
	ApplyMove
	ScoreGameBoard

With the exception of GetBoard, all of these are purely virtual. This requires that all classes that extend this base class have their own implementation.

Additionally, the Game class is set up to use generics/templates. This allows different representations of the game board/position to easily be used with this class (for example, Takeaways' board is just a simple int, but Nims' will be a vector<int>).

The Game class also has a variable to represent the current game board in addition to an accessor (GetBoard) for it, since all game types will require it.

---------
evaluator.h
---------
EDIT: The Evaluator now also uses memoization to speed up the evaluation process. It uses a map to store positions (the key) and integers (the value). The integer value indicates the index in the vectors that store the values previously computed (which are the score, best move and best resulting board).

The Evaluator class handles one function; Evaluating the game board for any game. This is handled in the function:

EvaluatePosition

The Evaluator is also set up to use generics/templates for easier adaptability. 

The EvaluatePosition function follows the pseudocode outline the in requirements pages for this project, found here (http://www.cs.rit.edu/~cs4/Projects/GamePlaying_111/writeup.html).

---------
Adding in new games
---------
EDIT: Crossout was added; no big deal. Did fix an issue with takeaway, though, where it was reporting wins as losses (ScoreGameBoard was negative when it should have been positive, due to a misunderstanding of takeaway is actually played. The winner is the person who DOESN'T take away the last penny).

EDIT: Othello was added; the only change to the framework was adjusting Evaluator to use the ScoreGameBoard method instead of simply returning -1.

EDIT: Nim was added; no substantial changes to framework were needed. Hopefully will hold true for other games as well.

Adding additional games is extremely easy. It's simply a matter of extending the base Game class and implementing the required functions, all of which should be simple (GenerateMoves is the only one that may require some thought).

If done correctly, the evaluator can be used with this new game type.

Each game class also contains a main method, so each one handles its own prompting for user input.

------------------------------------
------------Known Problems----------
------------------------------------
EDIT: 2 people were working on this release. Errors found in the previous release were fixed for this submission. The system added a new game which is substantially more complex than the previous games. Everything appears to run correctly. The supplied sample board runs successfully (without crashing) although it does take a while to complete. Perfect input is assumed for both manual input and file input. othello.cpp is huge file and needs refactoring, there are many places where it can be rewritten to take advantage of code reuse (notably the capturing logic).

EDIT: 2 people working on this now, so double the error checking power and still nothing was found because we're awesome. Memory usage was suprising, but that can be fixed for later releases (probably something with memoization).

NOTHING SUBSTANTIAL.

Not quite as good as last submit, but still nothing that'll break the program. There's some issue with user input; it's not quite clear how to input your move unless you read the readme. Also it assumes perfect input (no errors). If you enter 1 number and hit enter things get confusing, though they may still work.

------------------------------------
-----------Revision Control---------
------------------------------------
We used GIT for revision control; we have a repository set up on a friend's server.

We also checked the files into RCS, for kicks.

------------------------------------
---------------Status---------------
------------------------------------
1) All games (Takeaway, Nim, Othello and Crossout) are working/are runnable.

2) As far as I know, all games produce the best move/resulting position. I've checked the code and it matches the algorithm given, and all games pass every test case that is thrown at it. There is a chance I'm wrong though; developing test cases for this game solving algorithm is a pain so there's not as many as I'd like (espeically for Othello, I'm god-awful at that game).

3) Yes, all games produce a score value and that is how the winner is determined. The method of producing the score may be different per game, however. For example, Othello required the addition of a method called "ScoreGameBoard", which returns the score of the given gameboard. Most games can determine a score by just seeing if the game has ended on a players turn. In other words, for games that determine the winner by looking at the game board, ScoreGameBoard will be used. 

4) Yes, as #2 was also a yes (it produces the best move/resulting position). Getting it to play against a human correctly was simple. The majority of the work is done in each games' main method, and a lot of that code was copy/paste-able between games.

5) Yes, the answers are correct (as far as I know). Isn't this question #2 again, just rephrased? Unless you meant the answers to these status questions. But even then the answer is yes. I clearly have a problem with this question; I'm not a fan.

6) No, though given a large enough game board the amount of moves it has to process will make it seem like it won't ever stop running.

7) None of the games leak memory. I ran valgrind on normal and interactive modes for each of them. For Othello I also tested it when reading in a game board file. Everything is freed like a boss.
