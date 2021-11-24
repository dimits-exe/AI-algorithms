#include "Minimax.h"
#include <limits>
#include <math.h>
#include <iostream> //delete this


using namespace std;

Position mini_max(PLAYER turn, const Board& starting_board, int max_depth) {
	unordered_set <ReversiState, ReversiState::HashFunction> state_set;
	ReversiState start(turn, turn, starting_board);
	Info context(starting_board, max_depth+1);

	max_value(context, state_set, start, INT_MIN, INT_MAX, max_depth+1);

	return Position(context.x_of_move, context.y_of_move);
}

int min_value(Info& context, unordered_set <ReversiState, ReversiState::HashFunction>& closed_set, 
	const ReversiState& state, int a, int b, int TTL) {
	//cout << "Min " << static_cast<char>(state.turn) << endl;

	//if the game is predicted to end or if the state was already examined or the recursion has to end
	if (state.isFinal() || TTL < 0 || closed_set.count(state) == 1)
		return state.getValue();

	closed_set.insert(state);
	int value = INT_MAX;
	const ReversiState* min_value_state = nullptr;  //will point to the state with the min value

	const list<ReversiState>& children = state.getChildren();

	if (children.size() == 0) {
		//cout << "No possible moves " << '\n' << state.getBoard().toString() << endl;
		return max_value(context, closed_set, state, a, b, TTL - 2);
	}

	//examine all children and find the least fit one
	for (const ReversiState& child : children) {
		int max_v = max_value(context, closed_set, child, a, b, TTL - 2);
		//cout << "\t\t Needed value less than " << value << " value got: " << max_v << endl;
		if (value > max_v) {
			value = max_v;
			min_value_state = &child;
		}

		if (value <= a) {
			//cout << "Pruned " << '\n' << state.getBoard().toString() << endl;
			return child.getValue();
		}
			

		b = min(b, value);
	}

	if (min_value_state == nullptr) {
		throw logic_error("No suitable state found");
	}
	else {
		//cout << "Normal return from min " << '\n' << state.getBoard().toString() << " with value " << state.getValue() << endl;
		return min_value_state->getValue();
	}
		
}

int max_value(Info& context, unordered_set <ReversiState, ReversiState::HashFunction>& closed_set, 
	const ReversiState& state, int a, int b, int TTL) {
	//cout << "Max " << static_cast<char>(state.turn) << endl;

	//cout << "Max depth: " << context.MAX_DEPTH << " TTL: " << TTL << endl;
	if (context.MAX_DEPTH == TTL + 2) { //if directly below the root of the search tree
		const Position& move_made = state.getLastMove();
		//cout << state.getBoard().toString() << endl;
		//cout << "X = " << move_made.X() << "Y = " << move_made.Y() << endl;
		context.x_of_move = move_made.X();
		context.y_of_move = move_made.Y();
	}

	//if the game is predicted to end or if the state was already examined or the recursion has to end
	if (state.isFinal() || TTL < 0 || closed_set.count(state) == 1)
		return state.getValue();

	closed_set.insert(state);
	int value = INT_MIN;
	const ReversiState* max_value_state = nullptr; //will point to the state with the max value


	const list<ReversiState>& children = state.getChildren();

	if (children.size() == 0) {
		//cout << "No possible moves " << '\n' << state.getBoard().toString() << endl;
		return min_value(context,closed_set, state, a, b, TTL);
	}

	//examine all children and find the fittest one
	for (const ReversiState& child : children) {
		int min_v = min_value(context, closed_set, child, a, b, TTL);
		//cout << "\t\t Needed value bigger than " << value << " value got: " << min_v << endl;
		if (value < min_v) {
			value = min_v;
			max_value_state = &child;
		}

		if (value >= b) {
			//cout << "Pruned " << '\n' << state.getBoard().toString() << endl;
			return child.getValue();
		}


		a = max(a, value);
	}

	if (max_value_state == nullptr) {
		throw logic_error("No suitable state found");
	}
	else {
		//cout << "Normal return from max " << '\n' << state.getBoard().toString() << " with value " << state.getValue() << endl;
		return max_value_state->getValue();
	}

}
