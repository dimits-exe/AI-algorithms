#pragma once
#include <stdexcept>
#include <limits>
#include <iostream>

/// <summary>
/// An immutable x-y value pair referencing a square in the board.
/// Supports tagging a position as 'invalid', to be used as a return value.
/// </summary>
class Position {

public:

	static Position create_invalid() {
		return Position(INVALID_VALUE, INVALID_VALUE);
	}

	static bool is_invalid(Position p) {
		return p.x == INVALID_VALUE || p.y == INVALID_VALUE;
	}

	Position(int x, int y) : x(x), y(y){}

	Position(): x(INVALID_VALUE), y(INVALID_VALUE){}

	friend std::ostream& operator<<(std::ostream& os, const Position& p) {
		return os << " X=" << p.x << " Y=" << p.y;
	}

	int X() const {
		if (x == INVALID_VALUE)
			throw std::invalid_argument("Position invalid.");

		return x;
	}

	int Y() const {
		if (y == INVALID_VALUE)
			throw std::invalid_argument("Position invalid.");

		return y;
	}

private:
	static const int INVALID_VALUE = INT_MIN;
	int x, y;
};

