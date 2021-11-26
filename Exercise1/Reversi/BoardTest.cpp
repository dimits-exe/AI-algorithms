#include <iostream>
#include "Board.h"
#include <vector>
#include <string>

using namespace std;

pair<Position, Position> limits_in_x(PLAYER, const vector<vector<PLAYER>>& board, Position);
pair<Position, Position> limits_in_y(PLAYER, const vector<vector<PLAYER>>& board, Position);
pair<Position, Position> limits_in_main_diag(PLAYER, const vector<vector<PLAYER>>& board, Position);
pair<Position, Position> limits_in_sec_diag(PLAYER, const vector<vector<PLAYER>>& board, Position);
pair<Position, Position> limits_everywhere(PLAYER, const vector<vector<PLAYER>>& board, Position, int dx, int dy);
void testBoard();
void testLimits();
string ts(vector<vector<PLAYER>>&);

void p(string str, vector<vector<PLAYER>>& board) {
	cout << str << endl << ts(board) << endl;
}

void pp(string str, pair<Position, Position> lim) {
	cout << str << ": " << lim.first.X() << "-" << lim.first.Y() << " " << lim.second.X() << "-" << lim.second.Y() << endl;
}

void pl(vector<vector<PLAYER>>& board, Position move, PLAYER pl) {
	board[move.Y()][move.X()] = pl;

	cout << "played " << move.X() << "-" << move.Y() << " as player: " << static_cast<char>(pl) << " on board:" << endl;
	p("", board);
}

void ptfl(int x, int y, PLAYER p, vector<vector<PLAYER>>& board) {
	Position move(x, y);
	cout << endl << endl;
	pl(board, move, p);

	pp("xlim", limits_in_x(p, board, move));
	pp("ylim", limits_in_y(p, board, move));
	pp("dia1", limits_in_main_diag(p, board, move));
	pp("dia2", limits_in_sec_diag(p, board, move));
}

int main() {
	testLimits();
	// testBoard();
	return 0;
}

void testBoard() {
	Board board(8);
	cout << board.toString() << endl;

	cout << "Score: " << board.getScore(PLAYER::PLAYER1) << endl;

	Board board2(8);
	cout << "Boards are the same: " << (board.hashcode() == board2.hashcode()) << endl;


	cout << "Invalid move: " << board.isValidMove(PLAYER::PLAYER1, Position(0, 0)) << endl;
	cout << "Valid move: " << board.isValidMove(PLAYER::PLAYER2, Position(2, 3)) << endl;

	board.makeMove(PLAYER::PLAYER2, Position(2, 3));
	cout << board.toString() << endl;


	list<Position> valids = board.getValidMoves(PLAYER::PLAYER1);
	cout << "Valid move count: " << valids.size() << endl;


	for (auto move : board.getValidMoves(PLAYER::PLAYER2))
		cout << move.Y() << "-" << move.X() << " ";
	cout << endl;

}

void testLimits() {
	int DIMENSION = 8;
	vector<vector<PLAYER>> gameBoard;

	PLAYER P1 = PLAYER::PLAYER1, P2 = PLAYER::PLAYER2, PE = PLAYER::EMPTY;

	//construct vectors and initialize all squares to empty
	for (int i = 0; i < DIMENSION; i++) {
		gameBoard.push_back(vector<PLAYER>(DIMENSION));
		for (int j = 0; j < DIMENSION; j++) {
			gameBoard[i][j] = PLAYER::EMPTY;
		}
	}

	//initial board setup
	gameBoard[3][3] = PLAYER::PLAYER1;
	gameBoard[3][4] = PLAYER::PLAYER1;
	gameBoard[3][5] = PLAYER::PLAYER2;
	gameBoard[4][3] = PLAYER::PLAYER1;
	gameBoard[4][4] = PLAYER::PLAYER2;

	p("initial", gameBoard);

	ptfl(2, 2, P2, gameBoard);
	ptfl(3, 2, P1, gameBoard);
	ptfl(4, 2, P2, gameBoard);
	ptfl(5, 2, P1, gameBoard);
	ptfl(2, 3, P2, gameBoard);
	ptfl(2, 4, P1, gameBoard);
	ptfl(2, 5, P2, gameBoard);
	ptfl(3, 5, P1, gameBoard);
	ptfl(4, 5, P2, gameBoard);
	ptfl(5, 5, P1, gameBoard);
	ptfl(6, 1, P2, gameBoard);
	ptfl(7, 0, P1, gameBoard);
	ptfl(5, 1, P2, gameBoard);
	ptfl(4, 1, P2, gameBoard);
	ptfl(4, 0, P1, gameBoard);
	ptfl(5, 0, P2, gameBoard);
	ptfl(6, 0, P1, gameBoard);
	ptfl(6, 2, P1, gameBoard);
	ptfl(3, 0, P1, gameBoard);
	ptfl(3, 1, P1, gameBoard);
	ptfl(2, 1, P1, gameBoard);
	ptfl(2, 0, P2, gameBoard);
	ptfl(7, 7, P1, gameBoard);
}

pair<Position, Position> limits_everywhere(PLAYER p, const vector<vector<PLAYER>>& board, Position last_move, int dx, int dy)
{
	auto get_range = [](int min, int max) {
		return [min, max](int value) {
			return (min <= value) && (value < max);
		};
	};

	int max = board.size();
	auto check_range = get_range(0, max);

	int sx = last_move.X(), sy = last_move.Y(), ex = last_move.X(), ey = last_move.Y();

	bool found_other = false;

	int i = sx - dx;
	int j = sy - dy;
	for (; check_range(i) && check_range(j) && board[j][i] != PLAYER::EMPTY; i -= dx, j -= dy)
	{
		if (board[j][i] == nextTurn(p))
			found_other = true;

		if (board[j][i] == p) {
			if (found_other) {
				sx = i;
				sy = j;
			}
			break;
		}
	}

	found_other = false;

	i = ex + dx;
	j = ey + dy;
	for (; check_range(i) && check_range(j) && board[j][i] != PLAYER::EMPTY; i += dx, j += dy)
	{
		if (board[j][i] == nextTurn(p))
			found_other = true;

		if (board[j][i] == p) {
			if (found_other) {
				ex = i;
				ey = j;
			}
			break;
		}
	}

	return pair<Position, Position>(Position(sx, sy), Position(ex, ey));
}

pair<Position, Position> limits_in_x(PLAYER p, const vector<vector<PLAYER>>& board, Position last_move)
{
	return limits_everywhere(p, board, last_move, 1, 0);
}

pair<Position, Position> limits_in_y(PLAYER p, const vector<vector<PLAYER>>& board, Position last_move)
{
	return limits_everywhere(p, board, last_move, 0, 1);
}

pair<Position, Position> limits_in_main_diag(PLAYER p, const vector<vector<PLAYER>>& board, Position last_move)
{
	return limits_everywhere(p, board, last_move, 1, 1);
}

pair<Position, Position> limits_in_sec_diag(PLAYER p, const vector<vector<PLAYER>>& board, Position last_move)
{
	return limits_everywhere(p, board, last_move, 1, -1);
}

string ts(vector<vector<PLAYER>>& gameBoard) {
	string str(" ");
	int DIMENSION = gameBoard.size();

	//build horizontal header
	for (int i = 0; i < DIMENSION; i++)
		str += to_string(i) + " ";
	str += "\n";

	for (int i = 0; i < DIMENSION; i++) {
		str += to_string(i);
		for (int j = 0; j < DIMENSION; j++) {
			str += static_cast<char>(gameBoard[i][j]); //append player character
			str += " ";
		}
		str += "\n";
	}

	//returning by value is efficient in strings
	return str;
}

