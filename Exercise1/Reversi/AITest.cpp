#include <iostream>
#include "Minimax.h"

using namespace std;

int main(void) {

	cout << mini_max(PLAYER::PLAYER1, Board(8), 2).toString() << endl;

	return 0;
}