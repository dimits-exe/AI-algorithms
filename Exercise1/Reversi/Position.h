#pragma once
#include <stdexcept>
#include <limits>

/*
An immutable x-y value pair referencing a square in the board.
Supports tagging a position as 'invalid', as a return value.
*/
class Position {

public:

	static Position create_invalid() {
		return Position(INVALID_VALUE, INVALID_VALUE);
	}

	static bool is_invalid(Position p) {
		return p.x == INVALID_VALUE || p.y == INVALID_VALUE;
	}

	Position(int x, int y) : x(x), y(y){}

	int X() {
		if (x == INVALID_VALUE)
			throw std::invalid_argument("Position invalid.");

		return x;
	}

	int Y() {
		if (y == INVALID_VALUE)
			throw std::invalid_argument("Position invalid.");

		return y;
	}

private:
	static const int INVALID_VALUE = INT_MIN;
	const int x, y;
};

