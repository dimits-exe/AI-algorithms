#include <iostream>
#include "Minimax.h"


#define WELCOME_MSG		"Welcome to Reversi! This is a singleplayer game where you get against an AI"
#define SELECT_DEPTH	"Please select the depth of the algorithm"
#define SELECT_TURN		"Do you want to play first? (y/n)"
#define MAX_VALID_DEPTH	20

using namespace std;

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

	PLAYER turn;
	if (answer == "y" || answer == "Y")
		turn = PLAYER::PLAYER1;
	else
		turn = PLAYER::PLAYER2;

	Board* gameBoard = new Board(8);
	while (gameBoard->getValidMoves(PLAYER::PLAYER1).size() != 0 &&
		gameBoard->getValidMoves(PLAYER::PLAYER2).size() != 0) { //crashes here, board's array size is 0

		Board board = mini_max(turn, *gameBoard, depth);
		delete(gameBoard);
		gameBoard = &board;
		turn = nextTurn(turn);
	}

	return 0;
}

/*
	Board board(8);
	cout << mini_max(PLAYER::PLAYER1, board, 2).toString() << endl;

	cout << mini_max(PLAYER::PLAYER2, board, 2).toString() << endl;

	return 0;*/