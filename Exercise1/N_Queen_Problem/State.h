#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING

#pragma once

#include <iostream>
#include <list>

class ChildrenIterator;

class State
{
public:
	static State* random(int dimension);

	State(int dimension);
	State(int dimension, bool* data);
	State(const State& other);

	~State();

	void print();

	std::list<State*>* get_children();

	State* get_father();

	bool isFinal();

	int getScore();

	bool operator==(const State& other) const;

	struct HashFunction {
		size_t operator()(const State& state) const;
	};

	ChildrenIterator begin();
	ChildrenIterator end();

	friend ChildrenIterator;

private:
	static const int SCORE_NOT_EVALUATED = -1;

	bool *data;
	const int dimension, max_score;
	int score;

	State *father;

	void evaluate();

	inline bool get(int row, int col) {
		return this->data[row * this->dimension + col];
	}

	inline void set(int row, int col, bool value) {
		this->data[row * this->dimension + col] = value;
	}

	inline int findQ(int row) {
		int queen_col = 0;
		for (; this->get(row, queen_col) == 0; queen_col++)
			;

		return queen_col;
	}
};

class ChildrenIterator : public std::iterator<std::input_iterator_tag, State*>
{

public:
	ChildrenIterator(const State& state);
	~ChildrenIterator();

	ChildrenIterator& operator++();
	ChildrenIterator operator++(int);
	bool operator==(const ChildrenIterator& rhs);
	bool operator!=(const ChildrenIterator& rhs);
	State* operator*();

	void print() {
		printf("%d, %d\n", cx, cy);
	}

private:
	const State& original_state;
	State* generated_state;
	int cx, cy, qy;
};

