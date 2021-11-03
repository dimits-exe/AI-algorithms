#pragma once

#include <iostream>
#include <vector>

#include "Board.h"

class State
{
public:
	State(int dimension);
	
	State(const State& other);

	~State();

	void print();

	std::vector<State>* get_children();

	bool isFinal();

	int getScore();

	bool operator==(const State& other) const;

	struct HashFunction {
		size_t operator()(const State& state) const;
	};
	
private:
	bool *data;
	int dimension, score;

	State *father;

	void evaluate();
};

