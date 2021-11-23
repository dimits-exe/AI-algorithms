#include "State.h"
#include <time.h>
#include <math.h>

State* State::random(int dimension)
{
	State* state = new State(dimension);
	for (int i = 0; i < dimension; i++) {
		int col = rand() % dimension;
		state->data[i * dimension + col] = 1;
	}
	return state;
}

State::State(int dimension) : data(new bool[dimension * dimension]), dimension(dimension), score(-1), father(nullptr)
{
	memset(this->data, false, this->dimension * this->dimension);
}

State::State(int dimension, bool* data) : State(dimension)
{
	memcpy(this->data, data, this->dimension * this->dimension);
}

State::State(const State& other) : data(new bool[other.dimension * other.dimension]), dimension(other.dimension), score(other.score), father(other.father)
{
	memcpy(this->data, other.data, this->dimension * this->dimension);
}

State::~State()
{
	delete[] data;
}

void State::print()
{
	int dim = this->dimension;

	for (int row = 0, height = dim; row < height; row++) {
		for (int col = 0, width = dim; col < width; col++)
			std::cout << this->data[row * dim + col] ? "1" : "0";

		std::cout << std::endl;
	}
}

std::list<State*>* State::get_children()
{
	int dim = this->dimension;

	std::list<State*>* children = new std::list<State*>();
	for (int row = 0; row < dim; row++) {

		int queen_col;
		for (queen_col = 0; this->data[row * dim + queen_col] == 0; queen_col++)
			;

		this->data[row * dim + queen_col] = 0;

		for (int col = 0; col < dim; col++) {
			this->data[row * dim + col] = 1;

			State *child = new State(*this);
			child->evaluate();
			child->father = this;
			children->push_back(child);

			this->data[row * dim + col] = 0;
		}

		this->data[row * dim + queen_col] = 1;
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
	if (this->score == -1)
		evaluate();

	return (dimension * (dimension - 1) / 2) - score;
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
	int qx, qy;
	for (int row = 0; row < dim; row++)
	{
		qx = row;
		for (qy = 0; this->data[row * dim + qy] == 0; qy++)
			;

		// find the queen on each of the remaining rows
		int cx, cy;
		for (int remaining_row = row + 1; remaining_row < dim; remaining_row++)
		{
			cx = remaining_row;
			for (cy = 0; this->data[cx * dim + cy] == 0; cy++)
				;

			// same column
			if (qy == cy)
				this->score++;

			// same diagonal
			if (cx - qx == abs(qy - cy))
				this->score++;
		}
	}
}

size_t State::HashFunction::operator()(const State& state) const
{
	int hash = 0;
	for (int i = 0; i < state.dimension; i++) {
		int queen_col;
		for (queen_col = 0; state.data[i * state.dimension + queen_col] == 0 && queen_col < state.dimension + 1; queen_col++)
			;

		if (queen_col == state.dimension)
			cout << "rip hash" << endl;

		hash += (int) pow(10, state.dimension - i - 1) * queen_col;
	}

	return hash;
}

ChildrenIterator State::begin()
{
	return ChildrenIterator(*this);
}

ChildrenIterator State::end()
{
	int dim = this->dimension;
	// TODO: don't do this lmao
	ChildrenIterator iter = ChildrenIterator(*this);
	for (int i = 0, count = (dim * dim) - dim; i < count; i++)
		iter.operator++();
	
	return iter;
}

ChildrenIterator::ChildrenIterator(const State& state)
	: original_state(state), generated_state(nullptr), cx(0), cy(0), qy(-1) {

	for (qy = 0; original_state.data[cx * original_state.dimension + qy] == 0; qy++)
		if (qy == original_state.dimension)
			cout << "bad queen at row1 " << cx << endl;
}

ChildrenIterator::~ChildrenIterator()
{
	;
}

ChildrenIterator& ChildrenIterator::operator++()
{
	generated_state = nullptr;
	int dim = original_state.dimension;

	do {
		cy++;
		if (cy == original_state.dimension) {
			cy = 0;
			cx++;
			qy = -1;

			if (cx == original_state.dimension) {
				cx = cy = -1;
				break;
			}

			for (qy = 0; original_state.data[cx * dim + qy] == 0; qy++)
				if (qy == dim)
					cout << "bad queen at row1 " << cx << endl;
		}
	} while (cy == qy);

	// cout << "now at: " << cx << ", " << cy << endl;
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
	if (generated_state != nullptr)
		return generated_state;

	int dim = original_state.dimension;

	original_state.data[cx * dim + qy] = 0;
	original_state.data[cx * dim + cy] = 1;
	generated_state = new State(dim, original_state.data);
	original_state.data[cx * dim + cy] = 0;
	original_state.data[cx * dim + qy] = 1;

	generated_state->evaluate();
	return generated_state;
}
