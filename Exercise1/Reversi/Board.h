#pragma once

#include <vector>
#include <list>
#include <utility>
#include "Position.h"

/*An enum describing who occupies a certain square.*/
enum class PLAYER : char { EMPTY = '-', PLAYER1 = '1', PLAYER2 = '2' }; //define these are constants?

inline PLAYER nextTurn(PLAYER currentTurn) {
	if (currentTurn == PLAYER::PLAYER1)
		return PLAYER::PLAYER2;
	else
		return PLAYER::PLAYER1;
}

/// <summary>
/// A class describing the game's board.
/// </summary>
class Board {

public:

	Board(int size);

	Board(const Board& oldBoard);

	void operator=(const Board& other);

	/*
	Marks a player's move on the board.
	*/
	void makeMove(PLAYER, Position);

	Position getLastMove() const;

	/*
	Returns all the valid moves in this board for the next player.
	*/
	std::list<Position> getValidMoves(PLAYER p) const;

	/*
	Returns whether or not a move would be valid.
	*/
	bool isValidMove(PLAYER, Position) const;

	/*Get a unique identifier for the board.*/
	double hashcode() const;

	/*
	Get the score for one of the players. Score is defined as
	the number of squares controlled by the selected player
	minus the squares controlled by his opponent.
	*/
	int getScore(PLAYER p) const;

	/*Get a string representation of the board.*/
	std::string toString() const;

private:
	int DIMENSION;

	std::vector<std::vector<PLAYER>> gameBoard;
	
	Position lastMovePlayed;

	bool IsRangeValid(Position) const;

	/*
	Get a pair containing the furthest pair of player controlled squares in a line.
	Returns an invalid position if a pair isn't found.
	*/
	static std::pair<Position, Position> limits_in_x(PLAYER p, const std::vector<std::vector<PLAYER>>& board, Position last_move);

	/*
	Get a pair containing the furthest pair of player controlled squares in a row.
	Returns an invalid position if a pair isn't found.
	*/
	static std::pair<Position, Position> limits_in_y(PLAYER p, const std::vector<std::vector<PLAYER>>& board, Position last_move);

	/*
	Get a pair containing the furthest pair of player controlled squares in the move's square's
	main diagonal. Returns an invalid position if a pair isn't found.
	*/
	static std::pair<Position, Position> limits_in_main_diag(PLAYER p, const std::vector<std::vector<PLAYER>>& board, Position last_move);

	/*
	Get a pair containing the furthest pair of player controlled squares in the move's square's
	secondary diagonal. Returns an invalid position if a pair isn't found.
	*/
	static std::pair<Position, Position> limits_in_sec_diag(PLAYER, const std::vector<std::vector<PLAYER>>& board, Position last_move);

	static std::pair<Position, Position> limits_everywhere(PLAYER p, const std::vector<std::vector<PLAYER>>& board, Position last_move, int dx, int dy);
};
