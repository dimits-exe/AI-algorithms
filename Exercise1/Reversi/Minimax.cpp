#include "Minimax.h"
#include <limits>
#include <math.h>
#include <iostream> //delete this


using namespace std;

Board mini_max(PLAYER turn, const Board& starting_board, int max_depth) {
	unordered_set <ReversiState, ReversiState::HashFunction> state_set;
	ReversiState start(turn, starting_board);

	return max_value(state_set, start, INT_MIN, INT_MAX, max_depth).getBoard();
}

ReversiState min_value(unordered_set <ReversiState, ReversiState::HashFunction>& closed_set, const ReversiState& state, int a, int b, int TTL) {

	//if the game is predicted to end or if the state was already examined or the recursion has to end
	if (state.isFinal() || TTL < 0 || closed_set.count(state) == 1)
		return state;

	closed_set.insert(state);
	int value = INT_MAX;
	const ReversiState* min_value_state = nullptr;  //will point to the state with the min value

	const list<ReversiState>& children = state.getChildren();

	if (children.size() == 0) {
		//cout << "No possible moves " << '\n' << state.getBoard().toString() << endl;
		return max_value(closed_set, state, a, b, TTL - 1);
	}

	//examine all children and find the least fit one
	for (const ReversiState& child : children) {
		int max_v = max_value(closed_set, child, a, b, TTL-1).getValue();
		//cout << "\t\t Needed value less than " << value << " value got: " << max_v << endl;
		if (value > max_v) {
			value = max_v;
			min_value_state = &child;
		}

		if (value <= a) {
			//cout << "Pruned " << '\n' << state.getBoard().toString() << endl;
			return child;
		}
			

		b = min(b, value);
	}

	if (min_value_state == nullptr) {
		throw logic_error("No suitable state found");
	}
	else {
		//cout << "Normal return from min " << '\n' << state.getBoard().toString() << " with value " << state.getValue() << endl;
		return *min_value_state;
	}
		
}

ReversiState max_value(unordered_set <ReversiState, ReversiState::HashFunction>& closed_set, const ReversiState& state, int a, int b, int TTL) {

	//if the game is predicted to end or if the state was already examined or the recursion has to end
	if (state.isFinal() || TTL < 0 || closed_set.count(state) == 1)
		return state;

	closed_set.insert(state);
	int value = INT_MIN;
	const ReversiState* max_value_state = nullptr; //will point to the state with the max value


	const list<ReversiState>& children = state.getChildren();

	if (children.size() == 0) {
		//cout << "No possible moves " << '\n' << state.getBoard().toString() << endl;
		return max_value(closed_set, state, a, b, TTL - 1);
	}

	//examine all children and find the fittest one
	for (const ReversiState& child : children) {
		int min_v = min_value(closed_set, child, a, b, TTL - 1).getValue();
		//cout << "\t\t Needed value bigger than " << value << " value got: " << min_v << endl;
		if (value < min_v) {
			value = min_v;
			max_value_state = &child;
		}

		if (value >= b) {
			//cout << "Pruned " << '\n' << state.getBoard().toString() << endl;
			return child;
		}
			

		a = max(a, value);
	}

	if (max_value_state == nullptr){
		throw logic_error("No suitable state found");
	}
	else {
		//cout << "Normal return from max " << '\n' << state.getBoard().toString() << " with value " << state.getValue() << endl;
		return *max_value_state;
	}
		
}
