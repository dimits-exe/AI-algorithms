#include "Board.h"

Board::Board(int dimension) : dimension(dimension) { }

bool Board::operator()(int x, int y)
{
	return data[x * dimension + y];
}

void Board::operator()(int x, int y, bool value)
{
	data[x * dimension + y] = value;
}

