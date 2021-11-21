#include <iostream>
#include "Minimax.h"

#define INPUT_SYMBOL		"\n>"
#define WELCOME_MSG			"Welcome to Reversi! This is a singleplayer game where you get against an AI"
#define SELECT_DEPTH		"Please select the depth of the algorithm" INPUT_SYMBOL
#define SELECT_TURN			"Do you want to play first? (y/n)" INPUT_SYMBOL
#define MAKE_MOVE			"Play the next move (Give x-y coordinates)." INPUT_SYMBOL
#define WRONG_MOVE			"This move is invalid."
#define MAX_VALID_DEPTH		20

using namespace std;

void makePlayerMove(PLAYER turn, Board& currentBoard);

int main(void) {
	cout << WELCOME_MSG << endl;

	int depth = -1;
	do {
		cout << SELECT_DEPTH;
		cin >> depth;
	} while (depth <0 && depth > MAX_VALID_DEPTH);

	string answer;
	do {
		cout << SELECT_TURN;
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

	//main game loop
	while (gameBoard.getValidMoves(PLAYER::PLAYER1).size() != 0 &&
		gameBoard.getValidMoves(PLAYER::PLAYER2).size() != 0) { //crashes here, board's array size is 0

		if (turn == CPU)
			gameBoard = mini_max(turn, gameBoard, depth);
		else
			makePlayerMove(turn, gameBoard);

		turn = nextTurn(turn);
	}

	cout << "Player "<< static_cast<char>(turn) << " won!" << endl;

	return 0;
}

void makePlayerMove(PLAYER turn, Board& currentBoard) {
	int x, y;
	bool succeded = true;

	cout << currentBoard.toString();
	do {
		cout << MAKE_MOVE;
		cin >> y >> x;
		x--; y--; //convert to 1-8 range
		cout << "Player " << static_cast<char>(turn) << "played " << y << '-' << x << endl;

		if (x <= 0 || x > 8 || y <= 0 || y > 8) {
			cout << WRONG_MOVE << endl;
			succeded = false;
		}
		else {
			try {
				currentBoard.makeMove(turn, Position(x, y));
				succeded = true;
			}
			catch (logic_error) {
				cout << WRONG_MOVE << endl;
				succeded = false;
			}
		}

	} while (!succeded);
}
