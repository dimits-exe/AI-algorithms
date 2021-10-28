#include "Board.h"

Board::Board(int dimension) : dimension(dimension) { }

bool Board::operator() (int x, int y)  const
{
	return data[x * dimension + y];
}

void Board::operator()(int x, int y, bool value)
{
	data[x * dimension + y] = value;
}

const std::ostream& operator<<(const std::ostream& stream, const Board &b)
{
	for (int i = 0, height = b.dimension; i < height; i++) {
		for (int j = 0, width = b.dimension; j < width; j++) {
			stream << b(i, j) ? "1" : "0";
		}
	}

	return stream;
}


