#pragma once
#include "Position.h"

#include <vector>
#include <list>
#include <utility>


/// <summary>
/// An enum describing who occupies a certain square.
/// </summary>
enum class PLAYER : char { EMPTY = '-', PLAYER1 = '@', PLAYER2 = 'X' }; 

/// <summary>
/// Get the next player's turn.
/// </summary>
/// <param name="currentTurn">The current turn.</param>
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
	/// <summary>
	/// Create a new board initialized with 4 occupied squares.
	/// </summary>
	/// <param name="size">The size of the board.</param>
	Board(int size);

	/// <summary>
	/// Marks a player's move on the board.
	/// </summary>
	/// <param name="turn">The current player's turn.</param>
	/// <param name="move">The position of the move to be played.</param>
	void makeMove(PLAYER, Position);

	/// <summary>
	/// Get the last move played in the board.
	/// </summary>
	Position getLastMove() const;

	/// <summary>
	/// Returns all the valid moves in this board for the next player.
	/// </summary>
	/// <param name="p">The turn to be played.</param>
	/// <returns></returns>
	std::list<Position> getValidMoves(PLAYER turn) const;

	/// <summary>
	/// Returns whether or not a move would be valid.
	/// </summary>
	/// <param name="turn">The turn to be played.</param>
	/// <param name="move">The move to be played.</param>
	/// <returns>True if the move would be legal, false otherwise.</returns>
	bool isValidMove(PLAYER, Position) const;

	/// <summary>
	/// Get a unique identifier for the board.
	/// </summary>
	double hashcode() const;

	/// <summary>
	/// Get the score for one of the players. Score is defined as
	/// the number of squares controlled by the selected player
	///	minus the squares controlled by his opponent.
	/// </summary>
	/// <param name="p">The player to which the score is attributed.</param>
	/// <returns>The score of the board.</returns>
	int getScore(PLAYER p) const;

	/// <summary>
	/// Get a string representation of the board.
	/// </summary>
	std::string toString() const;

private:
	int DIMENSION;

	std::vector<std::vector<PLAYER>> gameBoard;
	
	Position lastMovePlayed;

	/// <summary>
	/// Checks whether the range of the move is within the board's limits.
	/// </summary>
	bool IsRangeValid(Position) const;

	/// <summary>
	/// Get a pair containing the furthest pair of player controlled squares in a line.
	/// </summary>
	static std::pair<Position, Position> limits_in_x(PLAYER p, const std::vector<std::vector<PLAYER>>& board, Position last_move);

	/// <summary>
	/// Get a pair containing the furthest pair of player controlled squares in a row.
	/// </summary>
	static std::pair<Position, Position> limits_in_y(PLAYER p, const std::vector<std::vector<PLAYER>>& board, Position last_move);

	/// <summary>
	/// Get a pair containing the furthest pair of player controlled squares in the move's square's main diagonal.
	/// </summary>
	static std::pair<Position, Position> limits_in_main_diag(PLAYER p, const std::vector<std::vector<PLAYER>>& board, Position last_move);

	/// <summary>
	/// Get a pair containing the furthest pair of player controlled squares in the move's square's secondary diagonal.
	/// </summary>
	static std::pair<Position, Position> limits_in_sec_diag(PLAYER, const std::vector<std::vector<PLAYER>>& board, Position last_move);

	/// <summary>
	///  Get a pair containing the furthest pair of player controlled squares in the move's square's in a specified direction.
	/// </summary>
	/// <param name="p">The turn in which the move is played.</param>
	/// <param name="board">The board to be checked.</param>
	/// <param name="last_move">The move to be played.</param>
	/// <param name="dx">The direction in which the scan will proceed horizontally.</param>
	/// <param name="dy">The direction in which the scan will proceed vertically.</param>
	/// <returns>The furthest possible pair in the specified direction.</returns>
	static std::pair<Position, Position> limits_everywhere(PLAYER p, const std::vector<std::vector<PLAYER>>& board, Position last_move, int dx, int dy);
};
