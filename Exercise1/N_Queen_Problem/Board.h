
class Board {

public:
	Board(int dimension);

	bool operator() (int x, int y);
	void operator() (int x, int y, bool v);

private:
	int dimension;
	bool* data;
};