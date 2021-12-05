#include "State.h"

#include <cstring>
#include <time.h>
#include <math.h>

using namespace std;

State* State::random(int size)
{
	// assumes srand has been called
	State* state = new State(size);
	for (int row = 0; row < state->size; row++) {
		// assumes one queen per row
		int col = rand() % state->size;
		state->set(row, col, true);
	}

	return state;
}

State::State(int boardSize) : data(new bool[boardSize * boardSize]), size(boardSize),
	max_score(boardSize * (boardSize - 1) / 2), score(SCORE_NOT_EVALUATED)
{
	// clear the data
	memset(this->data, false, this->size * this->size);
}

State::State(int boardSize, bool* stateData) : State(boardSize)
{
	// copy the data from the pointer
	memcpy(this->data, stateData, this->size * this->size);
}

State::State(const State& other) : data(new bool[other.size * other.size]),
	size(other.size), max_score((other.size * (other.size - 1) / 2)), score(other.score)
{
	// copy the data from the other State
	memcpy(this->data, other.data, this->size * this->size);
}

State::~State()
{
	delete[] data;
}

void State::print() const
{

	// print 1 if there is a queen, 0 if there isn't
	for (int row = 0; row < this->size; row++) {
		for (int col = 0; col < this->size; col++)
			cout << this->get(row, col) ? "1" : "0";

		cout << endl;
	}
}

list<State*>* State::get_children() const
{
	list<State*>* children = new list<State*>();
	for (int row = 0; row < this->size; row++) {

		int queen_col = findQ(row);

		set(row, queen_col, false);

		for (int col = 0; col < this->size; col++) {
			set(row, col, true);

			State *child = new State(*this);
			child->evaluate();
			children->push_back(child);

			set(row, col, false);
		}

		set(row, queen_col, true);
	}

	return children;
}

bool State::isFinal()
{
	return getScore() == 0;
}

int State::getScore()
{
	// don't return a non-evaluated score
	if (this->score == SCORE_NOT_EVALUATED)
		evaluate();

	// ensure bigger score is better
	return this->max_score - this->score;
}

bool State::operator==(const State& other) const
{
	if (this->size != other.size)
		return false;

	for (int i = 0, len = this->size * this->size; i < len; i++)
		if (this->data[i] != other.data[i])
			return false;

	return true;
}

void State::evaluate()
{
	this->score = 0; 
	// find the queen on each row
	for (int row = 0; row < this->size; row++)
	{
		int qx = row, qy = findQ(qx);

		// find the queen on each of the remaining rows
		for (int remaining_row = row + 1; remaining_row < this->size; remaining_row++)
		{
			int cx = remaining_row, cy = findQ(cx);

			// count pair if same column or same diagonal
			if (qy == cy || cx - qx == abs(qy - cy))
				this->score++;
		}
	}
}

ChildrenIterator State::begin() const
{
	return ChildrenIterator(*this);
}

ChildrenIterator State::end() const
{
	ChildrenIterator iter = ChildrenIterator(*this);

	// size*size total squares - size queens on the board
	const int number_of_children = (this->size * this->size) - this->size;

	for (int i = 0; i < number_of_children; i++)
		iter.operator++();
	
	return iter;
}

ChildrenIterator::ChildrenIterator(const State& state)
	: original_state(state), generated_state(nullptr), cx(0), cy(0), qy(state.findQ(0)) { }

ChildrenIterator::~ChildrenIterator() { }

ChildrenIterator& ChildrenIterator::operator++()
{
	// clear cached State
	generated_state = nullptr;

	do {
		// advance column
		cy++;

		// if column is outside of board limits
		if (cy >= original_state.size) {

			// advance row
			cx++;
			
			// if row is outside of board limits
			if (cx == original_state.size) {
				// mark position invalid and break
				cx = cy = ChildrenIterator::INVALID_POSITION;
				break;
			}

			// reset column and queen position
			cy = 0;
			qy = original_state.findQ(cx);
		}
	// skip boards that are the same as parent
	} while (cy == qy);

	return *this;
}

ChildrenIterator ChildrenIterator::operator++(int)
{
	ChildrenIterator temp(*this);
	operator++();
	return temp;
}

bool ChildrenIterator::operator==(const ChildrenIterator& rhs) const
{
	// same state and position
	return original_state == rhs.original_state && cx == rhs.cx && cy == rhs.cy;
}

bool ChildrenIterator::operator!=(const ChildrenIterator& rhs) const
{
	return !(this->operator==(rhs));
}

State* ChildrenIterator::operator*()
{
	// don't return anything when dereferencing end iterator
	if ((cx == ChildrenIterator::INVALID_POSITION) && (cy == ChildrenIterator::INVALID_POSITION))
		return nullptr;

	// return cached State
	if (generated_state != nullptr)
		return generated_state;

	// set row with queen at current position (cx, cy)
	original_state.set(cx, qy, false);
	original_state.set(cx, cy, true);

	// generate new State with queen at new position and cache it
	generated_state = new State(original_state);

	// reset row with queen at original position (cx, qy)
	original_state.set(cx, cy, false);
	original_state.set(cx, qy, true);

	generated_state->evaluate();
	return generated_state;
}

