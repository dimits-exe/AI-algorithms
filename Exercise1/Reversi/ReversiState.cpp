#include "ReversiState.h"

using namespace std;

ReversiState::ReversiState(PLAYER p, PLAYER CPU_SYMBOL, const Board& other_board) : CPU_SYMBOL(CPU_SYMBOL), board(other_board), turn(p) {}

ReversiState::ReversiState(const ReversiState& other): CPU_SYMBOL(other.CPU_SYMBOL), board(other.board), turn(other.turn) {}

Board ReversiState::getBoard() const {
	return board;
}

void ReversiState::setFather(const ReversiState* father) {
	this->father = father;
}

bool ReversiState::operator==(const ReversiState& other) const {
	return this->hashCode() == other.hashCode();
}

double ReversiState::hashCode() const {
	return board.hashcode();
}

size_t ReversiState::HashFunction::operator()(const ReversiState& state) const {
	return (size_t) state.hashCode();
}

bool ReversiState::isFinal() const {
	return (board.getValidMoves(this->turn).size() == 0) &&
		(board.getValidMoves(nextTurn(this->turn)).size() == 0); 
}

int ReversiState::getValue() const {
	int score = board.getScore(CPU_SYMBOL);
	if (isFinal()) { //if game won return infinity depending on who won
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

	for (Position move : board.getValidMoves(nextTurn(turn))) {
		Board newBoard(this->board);
		newBoard.makeMove(nextTurn(turn), move);

		ReversiState newState (nextTurn(turn), CPU_SYMBOL, newBoard);
		newState.setFather(this);

		children.push_back(newState);
	}

	return children; //probably the only bottleneck in this program because of the copying h
}
