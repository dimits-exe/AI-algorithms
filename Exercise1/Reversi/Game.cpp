#include <iostream>
#include "Minimax.h"

#define INPUT_SYMBOL		"\n>"
#define WELCOME_MSG			"Welcome to Reversi! This is a singleplayer game where you get against an AI"
#define SELECT_DEPTH		"Please select the depth of the algorithm" INPUT_SYMBOL
#define SELECT_TURN			"Do you want to play first? (y/n)" INPUT_SYMBOL
#define MAKE_MOVE			"Play the next move (Give x-y coordinates)." INPUT_SYMBOL
#define WRONG_MOVE			"This move is invalid."
#define MAX_VALID_DEPTH		10

using namespace std;

void makePlayerMove(PLAYER turn, Board& currentBoard);

int main(void) {
	std::cout << WELCOME_MSG << endl;

	int depth = -1;
	do {
		std::cout << SELECT_DEPTH;
		cin >> depth;
	} while (depth <0 && depth > MAX_VALID_DEPTH);

	string answer;
	do {
		std::cout << SELECT_TURN;
		cin >> answer;
	} while (answer != "y" && answer != "n" && answer != "Y" && answer != "N");

	PLAYER CPU;
	if (answer == "y" || answer == "Y")
		CPU = PLAYER::PLAYER2;
	else
		CPU = PLAYER::PLAYER1;

	//initialize variables
	PLAYER turn = PLAYER::PLAYER1;
	Board gameBoard(8);

	std::cout << "CPU symbol: " << static_cast<char>(CPU) << endl;
	std::cout << "Player symbol: " << static_cast<char>(nextTurn(CPU)) << endl;

	//main game loop
	while (gameBoard.getValidMoves(PLAYER::PLAYER1).size() != 0 &&
		gameBoard.getValidMoves(PLAYER::PLAYER2).size() != 0) { 

		if (turn == CPU) {
			cout << "Calculating next move..." << endl;
			Position move = mini_max(turn, gameBoard, depth);
			if (Position::is_invalid(move))
				std::cout << "No possible moves for the computer." << endl;
			else {
				try {
					gameBoard.makeMove(turn, move);
				}
				catch (logic_error e) {
					std::cout << e.what() << endl;
				}
			}
				
		}	
		else {
			makePlayerMove(turn, gameBoard);
		}
			

		turn = nextTurn(turn);
	}

	std::cout << "Player "<< static_cast<char>(turn) << " won!" << endl;
	std::cout << gameBoard.toString() << endl;

	return 0;
}

void makePlayerMove(PLAYER turn, Board& currentBoard) {
	int x, y;
	bool succeded = true;

	if (currentBoard.getValidMoves(turn).size() == 0) {
		std::cout << "No possible moves for the player" << endl;
		return;
	}

	std::cout << currentBoard.toString();
	do {
		std::cout << MAKE_MOVE;
		cin >> y >> x;
		std::cout << "The player played " << y << '-' << x << endl;
		x--; y--; //convert to 1-8 range

		if (x <= 0 || x > 8 || y <= 0 || y > 8) {
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
