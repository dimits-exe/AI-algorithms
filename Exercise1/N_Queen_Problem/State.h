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
	/// Returns a state with a random queen configuration with a given size
	/// </summary>
	static State* random(int size);

	State(int size);
	State(int size, bool* data);
	State(const State&);

	~State();

	void print() const;

	std::list<State*>* get_children() const;

	bool isFinal();

	/// <summary>
	/// Returns a score that determines how good this state is. A bigger score indicates a better state.
	/// </summary>
	/// <returns>The number of pairs of queens that are not attacked</returns>
	int getScore();

	bool operator==(const State&) const;

	ChildrenIterator begin() const;
	ChildrenIterator end() const;

	friend ChildrenIterator;

private:
	static const int SCORE_NOT_EVALUATED = -1;

	bool *data;
	const int size, max_score;
	int score;

	/// <summary>
	/// Sets this state's score equal to the number of pairs of queens that are attacked
	/// </summary>
	void evaluate();

	inline bool get(int row, int col) const
	{
		return this->data[row * this->size + col];
	}

	inline void set(int row, int col, bool value) const
	{
		this->data[row * this->size + col] = value;
	}

	inline int findQ(int row) const
	{
		int queen_col = 0;

		for (; this->get(row, queen_col) == false; queen_col++) {
			if (queen_col == this->size)
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
	ChildrenIterator(const State&);
	~ChildrenIterator();

	ChildrenIterator& operator++();
	ChildrenIterator operator++(int);
	bool operator==(const ChildrenIterator&) const;
	bool operator!=(const ChildrenIterator&) const;
	State* operator*();

private:
	static const int INVALID_POSITION = -1;

	const State& original_state;
	State* generated_state;
	int cx, cy, qy;
};

