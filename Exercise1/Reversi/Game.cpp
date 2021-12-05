#include "Minimax.h"
#include <iostream>

#define INPUT_SYMBOL		"\n>"
#define WELCOME_MSG		"Welcome to Reversi! This is a singleplayer game where you get to play against an AI! \
\nVisit https://en.wikipedia.org/wiki/Reversi for the rules of the game.\n"
#define SELECT_DEPTH_PROMPT	"Please select the depth of the algorithm (1-9):" INPUT_SYMBOL
#define SELECT_TURN_PROMPT	"Do you want to play first? (y/n)" INPUT_SYMBOL
#define MAKE_MOVE_PROMPT(x)	"Select " x ":" INPUT_SYMBOL
#define WRONG_MOVE		"This move is invalid."
#define MAX_VALID_DEPTH		9
#define BOARD_SIZE		8

using namespace std;

void makePlayerMove(PLAYER turn, Board& currentBoard);

int main(void) {
	cout << WELCOME_MSG << endl;

	//initialize program parameters
	int depth = -1;
	do {
		cout << SELECT_DEPTH_PROMPT;
		cin >> depth;
	} while (depth <= 0 || depth > MAX_VALID_DEPTH);

	string answer;
	do {
		cout << SELECT_TURN_PROMPT;
		cin >> answer;
	} while (answer != "y" && answer != "n" && answer != "Y" && answer != "N");

	PLAYER CPU;
	if (answer == "y" || answer == "Y")
		CPU = PLAYER::PLAYER2;
	else
		CPU = PLAYER::PLAYER1;

	//initialize game variables
	PLAYER turn = PLAYER::PLAYER1;
	Board gameBoard(BOARD_SIZE);

	cout << "CPU symbol: " << static_cast<char>(CPU) << endl;
	cout << "Player symbol: " << static_cast<char>(nextTurn(CPU)) << endl;

	//main game loop
	while (gameBoard.getValidMoves(PLAYER::PLAYER1).size() != 0 &&
		gameBoard.getValidMoves(PLAYER::PLAYER2).size() != 0) { 

		if (turn == CPU) {
			cout << "Calculating next move..." << endl;

			Position move = mini_max(turn, gameBoard, depth);
			if (move.isInvalid()) {
				cout << "No possible moves for the computer." << endl;
			}
			else {
				gameBoard.makeMove(turn, move);
				cout << "The Computer played " << Position(move.X() +1, move.Y()+1) << endl;
			}
				
		}	
		else {
			makePlayerMove(turn, gameBoard);
		}
		
		turn = nextTurn(turn);
	}

	cout << "Player "<< static_cast<char>(turn) << " won!" << endl;
	cout << gameBoard.toString() << endl;

	return 0;
}

void makePlayerMove(PLAYER turn, Board& currentBoard) {
	long long x, y; // try to prevent undefined behavior from oveflow and compiler optimization 
			// (seems to work unless a value of trillions is given as input)
	bool succeded = true;

	if (currentBoard.getValidMoves(turn).size() == 0) {
		std::cout << "No possible moves for the player" << endl;
		return;
	}

	do {
		cout << " Current Board: \n" << currentBoard.toString() << endl;
		cout << MAKE_MOVE_PROMPT("row");
		cin >> x;
		cout << MAKE_MOVE_PROMPT("column");
		cin >> y;
		cout << "The player played " << Position(x, y) << endl;
		x--; y--; //convert to 0-7 range

		if (x <= 0 || x > BOARD_SIZE || y <= 0 || y > BOARD_SIZE) {
			std::cout << WRONG_MOVE << endl;
			succeded = false;
		}
		else {
			try {
				currentBoard.makeMove(turn, Position(x, y));
				succeded = true;
				cout << currentBoard.toString() << endl;
			}
			catch (logic_error e) {
				std::cout << e.what() << endl;
				succeded = false;
			}
		}

	} while (!succeded);
}
