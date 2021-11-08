#include "ReversiState.h"

using namespace std;

ReversiState::ReversiState(PLAYER p, Board& other_board): board(other_board), turn(p), value(calculateValue(this)) {}

ReversiState::ReversiState(const ReversiState& other): board(other.board), turn(nextTurn(other.turn)), value(other.value) {}

double ReversiState::hashCode() const {
	return board.hashcode();
}

bool ReversiState::isFinal() const {
	return board.getValidMoves(turn).size() == 0; //current turn or next turn?
}

int ReversiState::calculateValue(ReversiState* state) {
	int score = state->board.getScore(state->turn);

	if (state->isFinal()) { //if game won return infinity depending on who won
		if(score > 0)
			return INT_MAX;
		else
			return INT_MIN;
	}
	else {
		return score;
	}
}

list<ReversiState> ReversiState::getChildren() const {
	list<ReversiState> children;

	for (Position move : board.getValidMoves(turn)) {
		Board newBoard(BOARD_SIZE);
		newBoard.makeMove(turn, move);
		children.push_back(ReversiState(nextTurn(turn), newBoard));
	}

	return children;
}