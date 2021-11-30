#include "ReversiState.h"

using namespace std;

ReversiState::ReversiState(PLAYER p, PLAYER CPU_SYMBOL, const Board& other_board) : CPU_SYMBOL(CPU_SYMBOL), board(other_board), turn(p) {}

Board ReversiState::getBoard() const {
	return board;
}

bool ReversiState::operator==(const ReversiState& other) const {
	return this->hashcode() == other.hashcode();
}

double ReversiState::hashcode() const {
	return board.hashcode();
}

size_t ReversiState::HashFunction::operator()(const ReversiState& state) const {
	return (size_t) state.hashcode();
}

bool ReversiState::isFinal() const {
	return (board.getValidMoves(this->turn).size() == 0) &&
		(board.getValidMoves(nextTurn(this->turn)).size() == 0); 
}

int ReversiState::getValue() const {
	int score = board.getScore(CPU_SYMBOL);
	if (isFinal()) {		//if game won return infinity depending on who won
		if (score > 0)
			return INT_MAX;
		else
			return INT_MIN;
	}
	else {					//else return normal score
		return score;
	}
}

Position ReversiState::getLastMove() const {
	return board.getLastMove();
}

list<ReversiState> ReversiState::getChildren() const {
	list<ReversiState> children;

	for (Position move : board.getValidMoves(turn)) {	//for all moves that could be played
		Board newBoard(this->board);					//create a new board and make a move
		newBoard.makeMove(turn, move);
		ReversiState newState (nextTurn(turn), CPU_SYMBOL, newBoard);	//create a state describing that board
		children.push_back(newState);					//add it to the list of possible states
	}

	return children; //probably the only bottleneck in this program because of the copying here
}
