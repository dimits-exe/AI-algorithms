#pragma once
#include "Board.h"
#include <memory>
#include <list>

/// <summary>
/// A class describing the state of a Board object.
/// </summary>
class ReversiState {

public:

	//No "father" pointer needed as the search is implemented in the stack

	/// <summary>
	/// Create a new initial state.
	/// </summary>
	/// <param name="p">The board's next turn.</param>
	/// <param name="CPU_SYMBOL">The CPU's turn symbol.</param>
	/// <param name="board">The board to be described by the state.</param>
	ReversiState(PLAYER p, PLAYER CPU_SYMBOL, const Board& board);

	bool operator==(const ReversiState& other) const;

	struct HashFunction { //used for set comparisons
		size_t operator()(const ReversiState& state) const;
	};

	/// <summary>
	/// Get all the states that can occur after this one.
	/// </summary>
	/// <returns>A list with all possible future states.</returns>
	std::list<ReversiState> getChildren() const;

	/// <summary>
	/// Whether or not the state can be mutated further.
	/// </summary>
	/// <returns>
	/// True if there are no other possible states, 
	/// false otherwise.
	/// </returns>
	bool isFinal() const;

	/// <summary>
	/// Get heuristic estimate of the computer's advantage over
	/// the player.
	/// </summary>
	/// <returns>The state's estimated value.</returns>
	int getValue() const;

	/// <summary>
	/// Get the move that created this state.
	/// </summary>
	/// <returns>The square in which the last move was played.</returns>
	Position getLastMove() const;
	
	/// <summary>
	/// Gets the board described by the state.
	/// </summary>
	/// <returns>A copy of the described board.</returns>
	Board getBoard() const;

private:
	//Use this and this only for computing the value.
	const PLAYER CPU_SYMBOL;

	//The current turn.
	const PLAYER turn;

	//The board described by the state.
	const Board board;

	double hashcode() const;
};

