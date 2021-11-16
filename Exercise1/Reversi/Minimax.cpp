#include <limits>
#include <math.h>
#include <iostream>
#include "Minimax.h"

using namespace std;

Board mini_max(PLAYER turn, const Board& starting_board, int max_depth) {
	unordered_set <ReversiState, ReversiState::HashFunction> state_set;
	ReversiState start(turn, starting_board);

	return max_value(state_set, start, 0, 0, max_depth).getBoard();
}

ReversiState min_value(unordered_set <ReversiState, ReversiState::HashFunction>& closed_set, ReversiState& state, int a, int b, int TTL) {
	cout << state.getBoard().toString() << " with value of " << state.getValue() << endl;

	if (state.isFinal() || TTL < 0 || closed_set.count(state) == 1)
		return state;

	closed_set.insert(state);
	int value = INT_MAX;
	ReversiState* min_value_state = nullptr;

	for (ReversiState& child : state.getChildren()) {
		int max_v = max_value(closed_set, child, a, b, TTL-1).getValue();
		if (value < max_v) {
			value = max_v;
			min_value_state = &child;
		}

		if (value <= a)
			return child;
		b = min(b, value);
	}

	if (min_value_state == nullptr)
		return state;
	else
		return *min_value_state;
}

ReversiState max_value(unordered_set <ReversiState, ReversiState::HashFunction>& closed_set, ReversiState& state, int a, int b, int TTL) {
	if (state.isFinal() || TTL < 0 || closed_set.count(state) == 1)
		return state;

	closed_set.insert(state);
	int value = INT_MIN;
	ReversiState* max_value_state = nullptr;

	for (ReversiState& child : state.getChildren()) {
		int min_v = min_value(closed_set, child, a, b, TTL - 1).getValue();
		if (value > min_v) {
			value = min_v;
			max_value_state = &child;
		}
		value = max(value, min_value(closed_set, child, a, b, TTL-1).getValue());
		if (value >= b)
			return child;
		a = max(a, value);
	}

	if (max_value_state == nullptr)
		return state;
	else
		return *max_value_state;
}