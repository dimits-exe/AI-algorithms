#pragma once

/*
An immutable x-y value pair referencing a move in the board.
*/
class Move {

public:
	Move(int x, int y) : x(x), y(y){}

	int X() {
		return x;
	}

	int Y() {
		return y;
	}

private:
	const int x, y;
};

