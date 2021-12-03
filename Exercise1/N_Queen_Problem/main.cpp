#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING

#include <cstring>

#include "State.h"
#include "StateSearcher.h"

using namespace std;

int main(int argc, char** argv) {

	// initialize the random seed that is used by some HC algorithms
	srand((unsigned)time(NULL));

	// parse commandline arguments
	if (argc < 2) {
		cout << "Usage: n_queens <size> [options]" << endl << endl
			<< "Where options include:" << endl
			<<"\t -hc for HillClimbing" << endl
			<<"\t -fc for FirstChoice" << endl
			<<"\t -rr for RandomRestarts with 5 restarts (hardcoded)" << endl
			<<"\t -st for Stochastic" << endl;
		return -1;
	}

	int size = atoi(argv[1]);

	if (size < 3) {
		cout << "Invalid size! It must be an integer greater than or equal to 3" << endl;
		return -1;
	}

	bool hc = true, fc = true, rr = true, st = true;
	if (argc > 2) {
		hc = fc = rr = st = false;

		for (int i = 2; i < argc; i++) {
			// cout << "'" << argv[i] << "'" << endl;
			hc |= strcmp(argv[i], "-hc") == 0;
			fc |= strcmp(argv[i], "-fc") == 0;
			rr |= strcmp(argv[i], "-rr") == 0;
			st |= strcmp(argv[i], "-st") == 0;
		}
	}
	// cout << hc << fc << rr << st << endl;

	// run the algorithms according to commandline args
	State* start = nullptr, * hc_state = nullptr, * fc_state = nullptr, * rr_state = nullptr, * st_state = nullptr;

	start = State::random(size);

	if (hc)
		hc_state = StateSearcher::HillClimbing(start);
	if (fc)
		fc_state = StateSearcher::HCFirstChoice(start);
	if (rr)
		rr_state = StateSearcher::HCRandomRestarts(size, 5);
	if (st)
		st_state = StateSearcher::HCStochastic(start);

	// declare `results` function for printing results and cleanup
	auto results = [](string name, State* s) {
		cout << name << " Score=" << s->getScore() << ". There are "
			<< s->max_score - s->getScore() << " pairs of queens that attack each other." << endl;
		s->print();
		delete s;
	};

	// print results according to commandline args
	results("Starting state:", start);
	if (hc)
		results("HC:  ", hc_state);
	if (fc)
		results("HCFC:", fc_state);
	if (rr)
		results("HCRR:", rr_state);
	if (st)
		results("HCSt:", st_state);

	cout << endl;
}

