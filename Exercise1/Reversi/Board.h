#pragma once

#include <array>
#include<list>
#include "Move.h"

using namespace std;

/*An enum describing who occupies a certain square.*/
enum class PLAYER { EMPTY, PLAYER1, PLAYER2 };

/*
A class describing the game's board.
*/
class Board {

public:
	Board();
	Board(const Board& oldBoard);
	~Board();

	/*
	Returns all the valid moves in this board for the next player.
	*/
	list<Move> getValidMoves();

	/*
	Returns whether or not a move would be valid.
	*/
	bool isValidMove(Move& move);

	/*
	Marks a player's move on the board.
	*/
	void makeMove(PLAYER p, Move& move);

private:
	const static int DIMENSION = 8; //cant make this non-static AND have an array with fixed size
	array<array<PLAYER, DIMENSION>, DIMENSION> board;

};

