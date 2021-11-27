#pragma once
#include <unordered_set>
#include "ReversiState.h"


/// <summary>
/// A struct used to hold the state of the algorithm while it's being executed.
/// Holds the max_depth value so the algorithm knows how far in the recursion it's in
/// and the move_made as a return value for the mini_max function.
/// </summary>
struct Info {
	Position move_made;
	const int MAX_DEPTH;

	Info(int maxDepth);
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
/// <param name="closed_set">A set holding all the already computed states.</param>
/// <param name="state">The state to be expanded and examined.</param>
/// <param name="a">The parameter of the node's max value</param>
/// <param name="b">The parameter of the node's min value</param>
/// <param name="TTL">How many more calls will be made before the algorithm terminates.</param>
/// <returns>The state with the current best outcome for the computer.</returns>
static int min_value(Info& context, std::unordered_set <ReversiState, ReversiState::HashFunction>& closed_set,
	const ReversiState& state, int a, int b, int TTL);

/// <summary>
/// The 'max' half of the alpha-beta pruning algorithm.
/// </summary>
/// <param name="closed_set">A set holding all the already computed states.</param>
/// <param name="state">The state to be expanded and examined.</param>
/// <param name="a">The parameter of the node's max value</param>
/// <param name="b">The parameter of the node's min value</param>
/// <param name="TTL">How many more calls will be made before the algorithm terminates.</param>
/// <returns>The state with the current best outcome for the computer.</returns>
static int max_value(Info& context, std::unordered_set <ReversiState, ReversiState::HashFunction>& closed_set,
	const ReversiState& state, int a, int b, int TTL);
