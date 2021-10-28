#pragma once

#include <iostream>
#include <vector>

#include "Board.h"

class State
{
public:
	State(int dimension);

	void print() {
		std::cout << board << std::endl;
	}

	std::vector<State> get_children();

	bool isFinal();

	bool operator==(const State& other) const;

	struct HashFunction {
		size_t operator()(const State& state) const;
	};
	
private:
	Board *board;
	int curr_x, curr_y, queens_added;

	State* father;

	bool add_at_column(int column);
};

