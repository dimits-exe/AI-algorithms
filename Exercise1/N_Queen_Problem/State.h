#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING

#pragma once

#include <iostream>
#include <list>

class ChildrenIterator;

/// <summary>
/// A State of the N Queen Problem
/// </summary>
class State
{
public:
	/// <summary>
	/// Returns a state with a random queen configuration with a given dimension
	/// </summary>
	static State* random(int dimension);

	State(int dimension);
	State(int dimension, bool* data);
	State(const State& other);

	~State();

	void print() const;

	std::list<State*>* get_children() const;

	bool isFinal();

	/// <summary>
	/// Returns a score that determines how good this state is. A bigger score indicates a better state.
	/// </summary>
	/// <returns>The number of pairs of queens that are not attacked</returns>
	int getScore();

	bool operator==(const State& other) const;

	ChildrenIterator begin() const;
	ChildrenIterator end() const;

	friend ChildrenIterator;

private:
	static const int SCORE_NOT_EVALUATED = -1;

	bool *data;
	const int dimension, max_score;
	int score;

	/// <summary>
	/// Sets this state's score equal to the number of pairs of queens that are attacked
	/// </summary>
	void evaluate();

	inline bool get(int row, int col) const
	{
		return this->data[row * this->dimension + col];
	}

	inline void set(int row, int col, bool value) const
	{
		this->data[row * this->dimension + col] = value;
	}

	inline int findQ(int row) const
	{
		int queen_col = 0;

		for (; this->get(row, queen_col) == false; queen_col++) {
			if (queen_col == this->dimension)
				std::cout << "No Queen at row: " << row << std::endl;
		}

		return queen_col;
	}
};

/// <summary>
/// An iterator over the children of a State
/// </summary>
class ChildrenIterator : public std::iterator<std::input_iterator_tag, State*>
{

public:
	ChildrenIterator(const State& state);
	~ChildrenIterator();

	ChildrenIterator& operator++();
	ChildrenIterator operator++(int);
	bool operator==(const ChildrenIterator& rhs) const;
	bool operator!=(const ChildrenIterator& rhs) const;
	State* operator*();

private:
	static const int INVALID_POSITION = -1;

	const State& original_state;
	State* generated_state;
	int cx, cy, qy;
};

