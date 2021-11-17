#include <iostream>
#include "Minimax.h"

#define INPUT_SYMBOL    	"\n>"
#define WELCOME_MSG		"Welcome to Reversi! This is a singleplayer game where you get against an AI"
#define SELECT_DEPTH		"Please select the depth of the algorithm" INPUT_SYMBOL
#define SELECT_TURN		"Do you want to play first? (y/n)" INPUT_SYMBOL
#define MAKE_MOVE		"Play the next move (Give x-y coordinates)." INPUT_SYMBOL
#define WRONG_MOVE		"This move is invalid."
#define MAX_VALID_DEPTH		20

using namespace std;

Board makePlayerMove(PLAYER turn, const Board* currentBoard);

int main(void) {
	cout << WELCOME_MSG << endl;

	int depth;
	do {
		cout << SELECT_DEPTH << endl;
		cin >> depth;
	} while (depth <0 && depth > MAX_VALID_DEPTH);

	string answer;
	do {
		cout << SELECT_TURN << endl;
		cin >> answer;
	} while (answer != "y" && answer != "n" && answer != "Y" && answer != "N");

	PLAYER CPU;
	if (answer == "y" || answer == "Y")
		CPU = PLAYER::PLAYER2;
	else
		CPU = PLAYER::PLAYER1;

	PLAYER turn = PLAYER::PLAYER1;

	Board* gameBoard = new Board(8);
	while (gameBoard->getValidMoves(PLAYER::PLAYER1).size() != 0 &&
		gameBoard->getValidMoves(PLAYER::PLAYER2).size() != 0) { //crashes here, board's array size is 0

		Board* newBoard;
		if (turn == CPU)
			newBoard = &mini_max(turn, *gameBoard, depth);
		else
			makePlayerMove(turn, gameBoard);

		delete(gameBoard);
		gameBoard = newBoard;
		turn = nextTurn(turn);
	}

	return 0;
}

void makePlayerMove(PLAYER turn, Board* const currentBoard) {
	int x, y;
	bool succeded = true;
	do {
		cout << MAKE_MOVE << endl;
		cin >> x >> y;
		try {
			currentBoard->makeMove(turn, Position(x, y));
			succeded = true;
		}
		catch (logic_error) {
			cout << WRONG_MOVE << endl;
			succeded = false;
		}
	} while (!succeded);
}
