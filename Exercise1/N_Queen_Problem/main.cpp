
#include "State.h"
#include "StateSearcher.h"

using namespace std;

int main() {
	//int size = 5;
	//bool* data = new bool[size * size]{
	//	0, 0, 0, 1, 0,
	//	0, 0, 0, 1, 0,
	//	0, 0, 1, 0, 0,
	//	0, 1, 0, 0, 0,
	//	0, 0, 1, 0, 0,
	//};
	//bool* data1 = new bool[size * size]{
	//	1, 0, 0, 0, 0,
	//	0, 0, 0, 1, 0,
	//	0, 0, 0, 0, 1,
	//	0, 1, 0, 0, 0,
	//	0, 0, 1, 0, 0,
	//};

	//State state = State(size, data);
	//State s2 = State(size, data);
	//State s3 = State(size, data1);
	//state.evaluate();
	//s2.evaluate();
	//s3.evaluate();

	//vector<State*> *v = state.get_children();
	//bool final = state.isFinal();
	//int score = state.getScore();
	//bool same1 = state == s2;
	//bool same2 = state == s3;
	//size_t hash = State::HashFunction()(state);
	//size_t hash3 = State::HashFunction()(s3);

	//state.print();
	//cout << (*v).size() << endl << final << endl <<
	//	score << endl << same1 << endl << same2 << endl <<
	//	hash << endl << hash3 << endl;

	//State* s = State::random(4);
	//s->print();
	//cout << endl;

	srand((unsigned)time(NULL));

	auto res = [](string name, State* s) {
		cout << name;
		printf(": %d\n", s->getScore());
		s->print();
		delete s;
	};

	int times = 1;
	while (times--) {
		int dim = 50;
		State* s1 = State::random(dim);
		s1->print();

		State* f1 = StateSearcher::HillClimbing(s1);
		State* f2 = StateSearcher::HCFirstChoice(s1);
		State* f3 = StateSearcher::HCRandomRestarts(dim, 3);
		State* f4 = StateSearcher::HCStochastic(s1);

		res("start", s1);
		res("HC", f1);
		res("HCFC", f2);
		res("HCRR", f3);
		res("HCSt", f4);
	}
	times++;
}


