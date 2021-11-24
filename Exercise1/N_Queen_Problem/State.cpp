#include "State.h"
#include <time.h>
#include <math.h>

using namespace std;

State* State::random(int dimension)
{
	// assumes srand has been called
	State* state = new State(dimension);
	for (int row = 0; row < state->dimension; row++) {
		// assumes one queen per row
		int col = rand() % state->dimension;
		state->set(row, col, true);
	}

	return state;
}

State::State(int dimension) : data(new bool[dimension * dimension]), dimension(dimension),
	score(SCORE_NOT_EVALUATED), father(nullptr), max_score(dimension * (dimension - 1) / 2) 
{
	// clear the data
	memset(this->data, false, this->dimension * this->dimension);
}

State::State(int dimension, bool* data) : State(dimension)
{
	// copy the data from the pointer
	memcpy(this->data, data, this->dimension * this->dimension);
}

State::State(const State& other) : data(new bool[other.dimension * other.dimension]),
	dimension(other.dimension), score(other.score), father(other.father),
	max_score((other.dimension* (other.dimension - 1) / 2) - score)
{
	// copy the data from the other State
	memcpy(this->data, other.data, this->dimension * this->dimension);
}

State::~State()
{
	delete[] data;
}

void State::print()
{
	int dim = this->dimension;

	// print 1 if there is a queen, 0 if there isn't
	for (int row = 0; row < dim; row++) {
		for (int col = 0; col < dim; col++)
			cout << this->get(row, col) ? "1" : "0";

		cout << endl;
	}
}

list<State*>* State::get_children()
{
	int dim = this->dimension;

	list<State*>* children = new list<State*>();
	for (int row = 0; row < dim; row++) {

		int queen_col = findQ(row);

		set(row, queen_col, false);

		for (int col = 0; col < dim; col++) {
			set(row, col, true);

			State *child = new State(*this);
			child->evaluate();
			child->father = this;
			children->push_back(child);

			set(row, col, false);
		}

		set(row, queen_col, true);
	}

	return children;
}

State* State::get_father()
{
	return father;
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
	if (this->dimension != other.dimension)
		return false;

	for (int i = 0, len = this->dimension * this->dimension; i < len; i++)
		if (this->data[i] != other.data[i])
			return false;

	return true;
}

void State::evaluate()
{
	int dim = this->dimension;
	
	this->score = 0;

	// find the queen on each row
	for (int row = 0; row < dim; row++)
	{
		int qx = row, qy = findQ(qx);

		// find the queen on each of the remaining rows
		for (int remaining_row = row + 1; remaining_row < dim; remaining_row++)
		{
			int cx = remaining_row, cy = findQ(cx);

			// count pair if same column or same diagonal
			if (qy == cy || cx - qx == abs(qy - cy))
				this->score++;
		}
	}
}

ChildrenIterator State::begin()
{
	return ChildrenIterator(*this);
}

ChildrenIterator State::end()
{
	int dim = this->dimension;
	ChildrenIterator iter = ChildrenIterator(*this);

	// dim*dim squares - dim queens
	for (int i = 0, count = (dim * dim) - dim; i < count; i++)
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
		if (cy >= original_state.dimension) {

			// advance row
			cx++;
			
			// if row is outside of board limits
			if (cx == original_state.dimension) {
				// mark position invalid and break
				cx = cy = -1;
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
	generated_state = nullptr;
	ChildrenIterator temp(*this);
	operator++();
	return temp;
}

bool ChildrenIterator::operator==(const ChildrenIterator& rhs)
{
	return original_state == rhs.original_state && cx == rhs.cx && cy == rhs.cy;
}

bool ChildrenIterator::operator!=(const ChildrenIterator& rhs)
{
	return !this->operator==(rhs);
}

State* ChildrenIterator::operator*()
{
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

