#pragma once
#include <memory>
#include <list>

#include "AbstractState.h"
#include "Board.h"

class ReversiState {

public:
	ReversiState(PLAYER p, const Board& board);

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
	
	/// <summary>
	/// Gets the board described by the state.
	/// </summary>
	/// <returns>A copy of the described board.</returns>
	Board getBoard() const;

	void setFather(const ReversiState* father);

private:

	//hold a pointer to a stack allocated variable,
	//no memory management needed for changing the pointer
	const ReversiState* father = nullptr; 

	const Board board;

	const PLAYER turn;
};

