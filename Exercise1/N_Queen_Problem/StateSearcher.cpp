#include "StateSearcher.h"
#include <list>

using namespace std;

State *StateSearcher::HillClimbing(State *initial_state)
{
	cout << "starting HC" << endl;
	State *largest_overall = new State(*initial_state);

	bool exists_larger = true;
	while (exists_larger) {

		State* largest_child = nullptr;

		for (auto iter = largest_overall->begin(), end = largest_overall->end(); iter != end; iter++) {

			State* generated_child = *iter;
			// initialise largest_child with the first generated child
			if (largest_child == nullptr)
			{
				largest_child = generated_child;
			}
			else
			{
				// check if generated child is better
				if (generated_child->getScore() > largest_child->getScore()) {
					delete largest_child;
					largest_child = generated_child;
				}
				else {
					delete generated_child;
				}
			}
		}

		exists_larger = largest_child->getScore() > largest_overall->getScore();
		if (exists_larger) {
			delete largest_overall;
			largest_overall = new State(*largest_child);
			delete largest_child;
		}
	}

	return largest_overall;
}

State* StateSearcher::HCSideSteps(State* initial_state, int side_steps)
{
	// lmao doesn't work
	State *current = initial_state;
	std::cout << side_steps << std::endl;

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
	cout << "starting HCFC" << endl;
	State *current = new State(*initial_state);

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

State* StateSearcher::HCRandomRestarts(int dimension, int retries)
{
	cout << "starting HCRR" << endl;
	State* largest = StateSearcher::HillClimbing(State::random(dimension));

	for (int i = 1; i < retries; i++) {
		State* state = StateSearcher::HillClimbing(State::random(dimension));
		if (state->getScore() > largest->getScore()) {
			delete largest;
			largest = state;
		}
		else {
			delete state;
		}
	}
	return largest;
}

State* StateSearcher::HCStochastic(State *initial_state)
{
	cout << "starting HCSt" << endl;
	State* current = new State(*initial_state);
	std::list<State*> bigger_children;
	std::list<State*>* children;

	bool exists_larger = true;
	while (exists_larger) {

		children = current->get_children();

		for (State *child : *children) {
			if (child->getScore() > current->getScore())
				bigger_children.push_back(child);
			else
				delete child;
		}

		children->clear();
		delete children;

		exists_larger = !bigger_children.empty();

		if (exists_larger) {
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
					delete current;
					current = temp;
					break;
				}
			}

			for (State* bigger_child : bigger_children)
				if (bigger_child != current)
					delete bigger_child;
			bigger_children.clear();
		}
	}

	return current;
}

