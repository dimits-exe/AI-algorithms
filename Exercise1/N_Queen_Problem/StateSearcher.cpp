#include "StateSearcher.h"

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


