#pragma once

#include "State.h"

class StateSearcher
{
public:
	static State* HillClimbing(State*);

	static State* HCSideSteps(State*, int);

	static State* HCFirstChoice(State*);

	static State* HCRandomRestarts(int dimension, int retries);
	
	static State* HCStochastic(State*);
};

