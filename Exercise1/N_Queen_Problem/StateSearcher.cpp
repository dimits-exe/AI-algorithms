#include "StateSearcher.h"
#include <time.h>
#include <list>

State *StateSearcher::HillClimbing(State *initial_state)
{
	State *current = initial_state;

	bool exists_larger = true;
	while (exists_larger) {

		exists_larger = false;
		State *largest = current;

		for (auto iter = current->begin(), end = current->end(); iter != end; iter++) {
			if ((*iter)->getScore() > largest->getScore()) {
				delete largest;
				largest = *iter;
				exists_larger = true;
			}
			else {
				delete *iter;
			}
		}
		current = largest;
	}

	return current;
}

State* StateSearcher::HCSideSteps(State* initial_state, int side_steps)
{
	State *current = initial_state;

	bool exists_larger = true;
	while (exists_larger) {

		std::list<State*> neighbours;

		exists_larger = false;
		State *largest = current;

		for (auto iter = current->begin(), end = current->end(); iter != end; iter++) {
			if ((*iter)->getScore() > largest->getScore()) {
				delete largest;
				largest = *iter;
				exists_larger = true;
			}
			else {
				delete *iter;
			}
		}
		current = largest;
	}

	return current;
}

State* StateSearcher::HCFirstChoice(State *initial_state)
{
	State *current = initial_state;

	bool exists_larger = true;
	while (exists_larger) {

		exists_larger = false;
		State *largest = current;

		for (auto iter = current->begin(), end = current->end(); iter != end; iter++) {
			if ((*iter)->getScore() > largest->getScore()) {
				delete largest;
				largest = *iter;
				exists_larger = true;
				break;
			}
			else {
				delete* iter;
			}
		}
		current = largest;
	}

	return current;
}

State* StateSearcher::HCRandomRestarts(int dim, int retries)
{
	State* largest;
	for (int i = 0; i < retries; i++) {
		State* state = StateSearcher::HillClimbing(State::random(dim));
		if (state->getScore() > largest->getScore())
			largest = state;
	}
	return largest;
}

State* StateSearcher::HCStochastic(State *initial_state)
{
	srand((unsigned)time(NULL));

	State* current = initial_state;
	std::list<State*> bigger_children;

	bool exists_larger = true;
	while (exists_larger) {

		std::list<State*> *children = current->get_children();

		for (State *s : *children) {
			if (s->getScore() > current->getScore())
				bigger_children.push_back(s);
			else
				delete s;
		}

		exists_larger = !bigger_children.empty();

		bigger_children.sort();
		int total_score = 0;
		for (auto iter = bigger_children.begin(); iter != bigger_children.end(); iter++)
			total_score += (*iter)->getScore();

		int target = rand() % total_score;
		int running_score = 0;
		for (auto iter = bigger_children.begin(); iter != bigger_children.end(); iter++) {
			State* temp = *iter;
			running_score += temp->getScore();
			if (running_score > target) {
				current = temp;
				break;
			}
		}

		for (State* s : bigger_children)
			if (s != current)
				delete s;
		bigger_children.clear();
	}

	return current;
}

