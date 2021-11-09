#include <iostream>
#include "Board.h"

using namespace std;

int main() {
	Board board(8);
	cout << board.toString() << endl;
	
	cout << "Score: " << board.getScore(PLAYER::PLAYER1) << endl;

	Board board2(8);
	cout << "Boards are the same: " << (board.hashcode() == board2.hashcode()) << endl;

	/*
	cout << "Invalid move: " << board.isValidMove(PLAYER::PLAYER1, Position(0, 0)) << endl;
	cout << "Valid move: " << board.isValidMove(PLAYER::PLAYER1, Position(2, 3)) << endl;

	board.makeMove(PLAYER::PLAYER1, Position(2, 3));
	cout << board.toString() << endl;

	list<Position> valids = board.getValidMoves(PLAYER::PLAYER1);
	cout << "Valid move count: " << valids.size() << endl;
	
	* for(auto move : board.getValidMoves(PLAYER::PLAYER2))
			cout << move.X() <<  "-" << move.Y() << " ";
	cout << endl;
	*/
	

	return 0;
}
