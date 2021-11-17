#pragma once
#include <memory>
#include <list>
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
	static const int BOARD_SIZE = 8;

	std::unique_ptr<const AbstractState> father;

	const Board board;

	const PLAYER turn;
};

