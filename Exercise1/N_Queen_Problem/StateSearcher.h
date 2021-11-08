#pragma once

#include "State.h"

class StateSearcher
{
public:
	static State* HillClimbing(State*);

	static State* HCSideSteps(State*);

	static State* HCFirstChoice(State*);

	static State* HCRandomRestarts(State*);
	
	static State* HCStochastic(State*);
};

