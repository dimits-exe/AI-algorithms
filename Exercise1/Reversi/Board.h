#pragma once

#include <array>
#include <list>
#include <utility>
#include "Position.h"

using namespace std;

/*An enum describing who occupies a certain square.*/
enum class PLAYER { EMPTY, PLAYER1, PLAYER2 };

/*
A class describing the game's board.
*/
class Board {

public:
	Board(const Board& oldBoard);

	/*
	Returns all the valid moves in this board for the next player.
	*/
	list<Position> get_valid_moves(PLAYER p);

	/*
	Returns whether or not a move would be valid.
	*/
	bool is_valid_move(PLAYER, Position);

	/*
	Marks a player's move on the board.
	*/
	void make_move(PLAYER, Position) ;

	string to_string();

private:
	const static int DIMENSION = 8; //cant make this non-static AND have an array with fixed size
	array<array<PLAYER, DIMENSION>, DIMENSION> board;

	bool range_is_valid(Position);

	/*
	Get a pair containing the furthest pair of player controlled squares in a line.
	Returns an invalid position if a pair isn't found.
	*/
	pair<Position, Position> limits_in_x(PLAYER p, int line);
	
	/*
	Get a pair containing the furthest pair of player controlled squares in a row.
	Returns an invalid position if a pair isn't found.
	*/
	pair<Position, Position> limits_in_y(PLAYER p, int row);

	/*
	Get a pair containing the furthest pair of player controlled squares in the move's square's
	main diagonal. Returns an invalid position if a pair isn't found.
	*/
	pair<Position, Position> limits_in_main_diag(PLAYER, Position);

	/*
	Get a pair containing the furthest pair of player controlled squares in the move's square's
	secondary diagonal. Returns an invalid position if a pair isn't found.
	*/
	pair<Position, Position> limits_in_sec_diag(PLAYER, Position);
};

