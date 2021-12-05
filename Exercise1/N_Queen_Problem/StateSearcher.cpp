#include "StateSearcher.h"
#include <list>

using namespace std;

State *StateSearcher::HillClimbing(State *initial_state)
{
	// the parent state of this generation that has the overall biggest score so far
	State *largest_overall = new State(*initial_state);

	bool exists_larger = true;
	while (exists_larger)
	{
		State* largest_child = nullptr;

		// find largest_child
		for (auto iter = largest_overall->begin(), end = largest_overall->end(); iter != end; iter++)
		{
			State* generated_child = *iter;
			// initialise largest_child with the first generated child
			if (largest_child == nullptr)
			{
				largest_child = generated_child;
			}
			else
			{
				// check if generated child is better
				if (generated_child->getScore() > largest_child->getScore())
				{
					delete largest_child;
					largest_child = generated_child;
				}
				else
				{
					delete generated_child;
				}
			}
		}

		// check if largest child has a better score than the largest_overall
		exists_larger = largest_child->getScore() > largest_overall->getScore();
		if (exists_larger)
		{
			delete largest_overall;
			largest_overall = largest_child;
		}
	}

	return largest_overall;
}

State* StateSearcher::HCFirstChoice(State *initial_state)
{
	// the parent state of this generation that has the overall biggest score so far
	State *largest_overall = new State(*initial_state);

	bool exists_larger = true;
	while (exists_larger)
	{
		exists_larger = false;

		for (auto iter = largest_overall->begin(), end = largest_overall->end(); iter != end; iter++)
		{
			// stop at the first child with a better score than largest_overall
			if ((*iter)->getScore() > largest_overall->getScore())
			{
				delete largest_overall;
				largest_overall = *iter;
				exists_larger = true;
				break;
			}
			// dismiss the other children that have a worse score than largest_overall
			else
			{
				delete* iter;
			}
		}
	}

	return largest_overall;
}

State* StateSearcher::HCRandomRestarts(int dimension, int retries)
{
	// the state that has the current highest score among all reruns of HillClimbing
	State* largest_overall = StateSearcher::HillClimbing(State::random(dimension));

	// run HillClimbing a total of 'retries' times
	for (int i = 1; i < retries; i++)
	{
		State* state = StateSearcher::HillClimbing(State::random(dimension));

		// keep the state with the biggest score
		if (state->getScore() > largest_overall->getScore())
		{
			delete largest_overall;
			largest_overall = state;
		}
		else
		{
			delete state;
		}
	}

	return largest_overall;
}

State* StateSearcher::HCStochastic(State *initial_state)
{
	// the parent state of this generation that has the overall biggest score so far
	State* largest_overall = new State(*initial_state);

	list<State*> bigger_children;

	bool exists_larger = true;
	while (exists_larger)
	{
		list<State*>* children = largest_overall->get_children();

		// separate the children into bigger and smaller than the largest_overall
		for (State *child : *children)
		{
			if (child->getScore() > largest_overall->getScore())
				bigger_children.push_back(child);
			else
				delete child;
		}

		// cleanup
		children->clear();
		delete children;

		// check if there are any bigger children
		exists_larger = !bigger_children.empty();

		if (exists_larger)
		{
			// find sum of score of bigger chilren
			int total_score = 0;
			for (auto iter = bigger_children.begin(); iter != bigger_children.end(); iter++)
				total_score += (*iter)->getScore();

			// randomly pick a child (among the bigger ones) with a weighted probability according to its score
			int target = rand() % total_score;
			int running_total = 0;
			// bigger_children.sort();

			// find the state whose score, when added to the running total, brings it above the target
			// states with a bigger score have a higher chance of being the state that achieves this by
			// computing the cumulative density function stopping when its value surpasses the target
			for (auto iter = bigger_children.begin(); iter != bigger_children.end(); iter++)
			{
				running_total += (*iter)->getScore();
				if (running_total > target)
				{
					delete largest_overall;
					largest_overall = *iter;
					break;
				}
			}

			// cleanup
			for (State* bigger_child : bigger_children)
				if (bigger_child != largest_overall)
					delete bigger_child;

			bigger_children.clear();
		}
	}

	return largest_overall;
}

