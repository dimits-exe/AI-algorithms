#pragma once
#include <stdexcept>
#include <limits>
#include <iostream>

#define INT_MIN std::numeric_limits<int>::min()
#define INT_MAX std::numeric_limits<int>::max()

/// <summary>
/// An immutable x-y value pair referencing a square in the board.
/// Supports tagging a position as 'invalid', to be used as a return value.
/// </summary>
class Position {

public:

	static Position create_invalid() {
		return Position(INVALID_VALUE, INVALID_VALUE);
	}

	Position(int x, int y) : x(x), y(y){}

	Position(): x(INVALID_VALUE), y(INVALID_VALUE){}

	friend std::ostream& operator<<(std::ostream& os, const Position& p) {
		return os << " X=" << p.x << " Y=" << p.y;
	}

	/// <summary>
	/// Returns the x coordinate or throws an exception if
	/// it's tagged as invalid.
	/// </summary>
	/// <returns>The x coordinate.</returns>
	/// <exception>logic_error if the position was tagged as invalid.</exception>
	int X() const {
		if (x == INVALID_VALUE)
			throw std::invalid_argument("Position invalid.");

		return x;
	}


	/// <summary>
	/// Returns the y coordinate or throws an exception if
	/// it's tagged as invalid.
	/// </summary>
	/// <returns>The y coordinate.</returns>
	/// <exception>logic_error if the position was tagged as invalid.</exception>
	int Y() const {
		if (y == INVALID_VALUE)
			throw std::invalid_argument("Position invalid.");

		return y;
	}

	/// <summary>
	/// Check whether the position was tagged as invalid.
	/// </summary>
	bool isInvalid() {
		return x == INVALID_VALUE || y == INVALID_VALUE;
	}


private:
	static const int INVALID_VALUE = INT_MIN;
	int x, y;
};

