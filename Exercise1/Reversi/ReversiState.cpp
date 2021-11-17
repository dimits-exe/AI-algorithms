#include "ReversiState.h"

using namespace std;

ReversiState::ReversiState(PLAYER p, const Board& other_board) : board(other_board), turn(p) {}

ReversiState::ReversiState(const ReversiState& other): board(other.board), turn(nextTurn(other.turn)) {}

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
	return state.hashCode();
}

bool ReversiState::isFinal() const {
	return (board.getValidMoves(this->turn).size() == 0) &&
		(board.getValidMoves(nextTurn(this->turn)).size() == 0); 
}

int ReversiState::getValue() const {
	int score = board.getScore(turn);
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

list<ReversiState> ReversiState::getChildren() const {
	list<ReversiState> children;

	for (Position move : board.getValidMoves(nextTurn(turn))) {
		Board newBoard(this->board);
		newBoard.makeMove(nextTurn(turn), move);

		ReversiState* newState = new ReversiState(nextTurn(turn), newBoard);
		newState->setFather(this);

		children.push_back(*newState);
	}

	return children;
}
