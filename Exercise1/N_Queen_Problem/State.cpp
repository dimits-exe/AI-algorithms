#include "State.h"

State::State(int dimension) : data(new bool[dimension * dimension]), dimension(dimension), score(0), father(nullptr) {}

State::State(const State& other) : data(new bool[dimension * dimension]), dimension(other.dimension), score(other.score), father(other.father)
{
	memcpy(this->data, other.data, this->dimension * this->dimension);
}

State::~State()
{
	delete[] data;
	delete father;
}

void State::print()
{
	for (int i = 0, height = this->dimension; i < height; i++) {
		for (int j = 0, width = this->dimension; j < width; j++) {
			std::cout << this->data[i * dimension + j] ? "1" : "0";
		}
	}
}

std::vector<State>* State::get_children()
{
	std::vector<State>* children = new std::vector<State>();
	for (int i = 0; i < this->dimension; i++) {

		int queen_col;
		for (queen_col = 0; this->data[i * this->dimension + queen_col] == 0; queen_col++)
			;

		this->data[i * this->dimension + queen_col] = 0;

		for (int j = 0; j < this->dimension; j++) {
			this->data[i * this->dimension + queen_col] = 1;
			State child = State(*this);
			child.evaluate();
			this->data[i * this->dimension + queen_col] = 0;

			children->push_back(child);
		}
	}

	return children;
}

bool State::isFinal()
{
	return getScore() == 0;
}

int State::getScore()
{
	return score;
}

bool State::operator==(const State& other) const
{
	if (this->dimension != other.dimension)
		return false;

	for (int i = 0, const len = this->dimension * this->dimension; i < len; i++)
		if (this->data[i] != other.data[i])
			return false;
	return true;
}

void State::evaluate()
{
	this->score = 0;
	for (int i = 0; i < this->dimension; i++)
	{
		int queen_col;
		for (queen_col = 0; this->data[i * this->dimension + queen_col] == 0; queen_col++)
			;

		for (int j = i + 1; j < this->dimension; j++)
		{
			for (int k = 0; k < this->dimension; k++)
			{
				if (this->data[j * this->dimension + k] == 1)
				{
					if (queen_col == k || abs(queen_col - i) == abs(k - j))
						score++;
				}
			}
		}
	}
}

size_t State::HashFunction::operator()(const State& state) const
{
	return size_t();
}

