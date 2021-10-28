#pragma once
#include <list>
#include "Board.h"

using namespace std;

class ReversiState {

public:
	ReversiState(Board& board);
	~ReversiState();

	ReversiState& getFather();
	void setFather(ReversiState&);
	bool isFinal();
	void makeMove(Move&);

private:
	ReversiState& father;
	Board board;
};

