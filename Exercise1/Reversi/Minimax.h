#pragma once
#include <unordered_set>
#include "ReversiState.h"

Board mini_max(PLAYER turn, const Board& start, int max_depth);

static ReversiState min_value(std::unordered_set <ReversiState, ReversiState::HashFunction>& closed_set, 
	ReversiState& state, int a, int b, int TTL);

static ReversiState max_value(std::unordered_set <ReversiState, ReversiState::HashFunction>& closed_set,
	ReversiState& state, int a, int b, int TTL);