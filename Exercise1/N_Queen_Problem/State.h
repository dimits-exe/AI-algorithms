#pragma once

#include <iostream>
#include <list>

class State
{
	class ChildrenIterator;

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

	class ChildrenIterator : public std::iterator<std::input_iterator_tag, State*>
	{

	public:
		ChildrenIterator(const State& state);

		ChildrenIterator& operator++();
		ChildrenIterator operator++(int);
		bool operator==(const ChildrenIterator& rhs);
		bool operator!=(const ChildrenIterator& rhs);
		State* operator*();

	private:
		const State& original_state;
		State* generated_state;
		int dim, cx, cy, qy;
	};

private:
	bool *data;
	int dimension, score;

	State *father;

	void evaluate();
};

