#pragma once
#include <unordered_set>
#include "ReversiState.h"

/*
* It's clear now that this should have been a class to hide all the implementation from users of the file.
* It works fine for this project though.
*/

/// <summary>
/// A struct used to hold the state of the algorithm while it's being executed.
/// Holds the max_depth value so the algorithm knows how far in the recursion it's in
/// and the move_made as a return value for the mini_max function.
/// </summary>
struct Info {
	Position move_made;
	int best_value;
	const int MAX_DEPTH;

	Info(int max_depth);
};

/// <summary>
/// Runs the board through an AI algorithm to determine the best move.
/// </summary>
/// <param name="turn">The player controlled by the computer.</param>
/// <param name="start">The current state of the board</param>
/// <param name="max_depth">The maximum search depth for the algorithm.</param>
/// <returns>A new board with the best move marked.</returns>
Position mini_max(PLAYER turn, const Board start, int max_depth);

/// <summary>
/// The 'min' half of the alpha-beta pruning algorithm.
/// </summary>
/// <param name="context">An object holding the state of the algorithm.</param>
/// <param name="closed_set">A set holding all the already computed states.</param>
/// <param name="state">The state to be expanded and examined.</param>
/// <param name="a">The parameter of the node's max value</param>
/// <param name="b">The parameter of the node's min value</param>
/// <param name="TTL">How many more calls will be made before the algorithm terminates.</param>
/// <returns>The state with the current best outcome for the computer.</returns>
int min_value(Info& context, std::unordered_set <ReversiState, ReversiState::HashFunction>& closed_set,
	const ReversiState& state, int a, int b, int TTL);

/// <summary>
/// The 'max' half of the alpha-beta pruning algorithm.
/// </summary>
/// <param name="context">An object holding the state of the algorithm.</param>
/// <param name="closed_set">A set holding all the already computed states.</param>
/// <param name="state">The state to be expanded and examined.</param>
/// <param name="a">The parameter of the node's max value</param>
/// <param name="b">The parameter of the node's min value</param>
/// <param name="TTL">How many more calls will be made before the algorithm terminates.</param>
/// <returns>The state with the current best outcome for the computer.</returns>
int max_value(Info& context, std::unordered_set <ReversiState, ReversiState::HashFunction>& closed_set,
	const ReversiState& state, int a, int b, int TTL);

/// <summary>
/// Stores the move if it's better than the currently best one in the Info object.
/// </summary>
/// <param name="context">An object holding the state of the algorithm.</param>
/// <param name="AI_move">The attempted move by the AI.</param>
/// <param name="move_value">The value of the board after the supplied move.</param>
/// <param name="curr_depth">The current depth of the DFS search.</param>
void store_move_if_best(Info& context, Position& AI_move, int move_value, int curr_depth);
