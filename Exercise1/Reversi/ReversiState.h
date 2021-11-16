#pragma once
#include <memory>
#include <list>

#include "AbstractState.h"
#include "Board.h"

class ReversiState {

public:
	ReversiState(PLAYER p, const Board& board);

	ReversiState(const ReversiState&);

	bool operator==(const AbstractState& other) {
		return this->hashCode() == other.hashCode();
	}

	std::list<ReversiState> getChildren() const;

	double hashCode() const;

	bool isFinal() const;

	Board getBoardCopy() const {
		return board;
	}

	int getValue() const {
		return value;
	}

private:
	/*Used in the constructor.*/
	static int calculateValue(ReversiState* state);

	static const int BOARD_SIZE = 8;

	std::unique_ptr<const AbstractState> father;

	const int value;

	const Board board;

	const PLAYER turn;
};

