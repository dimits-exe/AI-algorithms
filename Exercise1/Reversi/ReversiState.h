#pragma once

#include "Board.h"
#include <memory>
#include <list>

/// <summary>
/// A class describing the state of a Board object.
/// </summary>
class ReversiState {

public:
	ReversiState(PLAYER p, PLAYER CPU_SYMBOL, const Board& board);

	ReversiState(const ReversiState&);

	bool operator==(const ReversiState& other) const;

	struct HashFunction { //used for hash function
		size_t operator()(const ReversiState& state) const;
	};

	/// <summary>
	/// Get all the states that can occur after this one.
	/// </summary>
	/// <returns>A list with all possible future states.</returns>
	std::list<ReversiState> getChildren() const;

	double hashCode() const;

	/// <summary>
	/// Whether or not the state can be mutated further.
	/// </summary>
	/// <returns>
	/// True if there are no other possible states, 
	/// false otherwise
	/// </returns>
	bool isFinal() const;

	int getValue() const;

	Position getLastMove() const;
	
	/// <summary>
	/// Gets the board described by the state.
	/// </summary>
	/// <returns>A copy of the described board.</returns>
	Board getBoard() const;

	void setFather(const ReversiState* father);

	const PLAYER turn;

private:
	//use this and this only for computing the value
	const PLAYER CPU_SYMBOL;

	//hold a pointer to a stack allocated variable,
	//no memory management needed for changing the pointer
	const ReversiState* father = nullptr; 

	const Board board;

};

