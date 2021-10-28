#pragma once
#include <ostream>

class Board {

public:
	const int dimension;

	Board(int dimension);

	bool operator() (int x, int y) const;
	void operator() (int x, int y, bool v);

private:
	bool* data;
};

const std::ostream& operator<< (const std::ostream&, const Board &b);

