#include "Minimax.h"
#include <math.h>

using namespace std;

Info::Info(int maxDepth):MAX_DEPTH(maxDepth), move_made(Position::create_invalid()), best_value(INT_MIN) {}

//I'm so proud of my boy, Im naming him Stockfish Jr
Position mini_max(PLAYER turn, const Board starting_board, int max_depth) {
	unordered_set <ReversiState, ReversiState::HashFunction> state_set;
	ReversiState start(turn, turn, starting_board);
	Info context(max_depth);

	min_value(context, state_set, start, INT_MIN, INT_MAX, max_depth);

	return context.move_made;
}

int min_value(Info& context, unordered_set <ReversiState, ReversiState::HashFunction>& closed_set, 
	const ReversiState& state, int a, int b, int TTL) {

	//if the game is predicted to end or if the state was already examined or the recursion has to end
	if (state.isFinal() || TTL < 0 || closed_set.count(state) == 1)
		return state.getValue();

	closed_set.insert(state);
	int value = INT_MAX;
	const ReversiState* min_value_state = nullptr;  //will point to the state with the min value

	const list<ReversiState>& children = state.getChildren();

	if (children.size() == 0) {
		return max_value(context, closed_set, state, a, b, TTL - 2);
	}

	//examine all children and find the least fit one
	for (const ReversiState& child : children) {
		int max_v = max_value(context, closed_set, child, a, b, TTL - 2);
		if (value > max_v) {
			value = max_v;
			min_value_state = &child;
		}

		if (value <= a) {
			return child.getValue();
		}

		b = min(b, value);
	}

	if (min_value_state == nullptr) {
		return state.getValue();
	}
	else {
		return min_value_state->getValue();
	}
		
}

int max_value(Info& context, unordered_set <ReversiState, ReversiState::HashFunction>& closed_set, 
	const ReversiState& state, int a, int b, int TTL) {

	Position AI_move;
	if (context.MAX_DEPTH == TTL + 2) {				//if directly below the root of the search tree
		AI_move = state.getLastMove();				//save the move made
	}

	//if the game is predicted to end or if the state was already examined or the recursion has to end
	if (state.isFinal() || TTL < 0 || closed_set.count(state) == 1) {
		int value = state.getValue();
		store_move_if_best(context, AI_move, value, TTL);
		return value;
	}

	closed_set.insert(state);
	int value = INT_MIN;
	const ReversiState* max_value_state = nullptr; //will point to the state with the max value
	const list<ReversiState>& children = state.getChildren();

	if (children.size() == 0) {
		return min_value(context,closed_set, state, a, b, TTL);
	}

	//examine all children and find the fittest one
	for (const ReversiState& child : children) {
		int min_v = min_value(context, closed_set, child, a, b, TTL);
		if (value < min_v) {
			value = min_v;
			max_value_state = &child;
		}

		if (value >= b) {
			store_move_if_best(context, AI_move, value, TTL);
			return child.getValue();
		}

		a = max(a, value);
	}

	if (max_value_state == nullptr) {
		return state.getValue();
	}
	else {
		store_move_if_best(context, AI_move, value, TTL);
		return max_value_state->getValue();
	}

}

void store_move_if_best(Info& context, Position& AI_move, int move_value, int curr_depth) {
	if (context.MAX_DEPTH == curr_depth + 2 && move_value > context.best_value) {
		context.move_made = AI_move;
		context.best_value = move_value;
	}
		
}
