#include <iostream>
#include "Board.h"
#include <vector>

using namespace std;

pair<Position, Position> limits_in_x(PLAYER p, vector<vector<PLAYER>>& board, int line);
pair<Position, Position> limits_in_y(PLAYER p, vector<vector<PLAYER>>& board, int row);
pair<Position, Position> limits_in_main_diag(PLAYER player, vector<vector<PLAYER>>& board, Position pos);
pair<Position, Position> limits_in_sec_diag(PLAYER player, vector<vector<PLAYER>>& board, Position pos);
string toString(vector<vector<PLAYER>>&);

int main() {
	//testLimits();
	testBoard();
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

	//construct vectors and initialize all squares to empty
	for (int i = 0; i < DIMENSION; i++) {
		gameBoard.push_back(vector<PLAYER>(DIMENSION));
		for (int j = 0; j < DIMENSION; j++) {
			gameBoard[i][j] = PLAYER::EMPTY;
		}
	}

	//initial board setup
	Position middleSquare((DIMENSION - 1) / 2, (DIMENSION - 1) / 2);
	gameBoard[middleSquare.X()][middleSquare.Y()] = PLAYER::PLAYER1;
	gameBoard[middleSquare.X() + 1][middleSquare.Y() + 1] = PLAYER::PLAYER1;
	gameBoard[middleSquare.X() + 1][middleSquare.Y()] = PLAYER::PLAYER2;
	gameBoard[middleSquare.X()][middleSquare.Y() + 1] = PLAYER::PLAYER2;

	//test x
	gameBoard[3][5] = PLAYER::PLAYER1;
	cout << toString(gameBoard) << endl;

	pair<Position, Position> x_limit = limits_in_x(PLAYER::PLAYER1, gameBoard, 3);
	cout << x_limit.first.Y() << "-" << x_limit.first.X() << " " << x_limit.second.Y() << "-" << x_limit.second.X() << endl;

	gameBoard[3][5] = PLAYER::EMPTY;

	//test y
	gameBoard[2][3] = PLAYER::PLAYER2;
	cout << toString(gameBoard) << endl;

	pair<Position, Position> y_limit = limits_in_y(PLAYER::PLAYER2, gameBoard, 3);
	cout << y_limit.first.Y() << "-" << y_limit.first.X() << " " << y_limit.second.Y() << "-" << y_limit.second.X() << endl;

	gameBoard[2][3] = PLAYER::EMPTY;

	//main diag
	gameBoard[2][2] = PLAYER::PLAYER2;
	gameBoard[5][5] = PLAYER::PLAYER2;
	cout << toString(gameBoard) << endl;

	pair<Position, Position> m_diag_limit = limits_in_main_diag(PLAYER::PLAYER2, gameBoard, Position(5, 5));
	cout << m_diag_limit.first.Y() << "-" << m_diag_limit.first.X() << " " << m_diag_limit.second.Y() << "-" << m_diag_limit.second.X() << endl;

	gameBoard[2][2] = PLAYER::EMPTY;
	gameBoard[5][5] = PLAYER::EMPTY;

	//SEC diag
	gameBoard[5][2] = PLAYER::PLAYER1;
	gameBoard[2][5] = PLAYER::PLAYER1;
	cout << toString(gameBoard) << endl;

	pair<Position, Position> sex_diag_limit = limits_in_sec_diag(PLAYER::PLAYER1, gameBoard, Position(5, 2));
	cout << sex_diag_limit.first.Y() << "-" << sex_diag_limit.first.X() << " " << sex_diag_limit.second.Y() << "-" << sex_diag_limit.second.X() << endl;

	gameBoard[5][2] = PLAYER::EMPTY;
	gameBoard[2][5] = PLAYER::EMPTY;
}


pair<Position, Position> limits_in_x(PLAYER p, vector<vector<PLAYER>>& board, int line) {
	int x_start = -1;
	int x_end = -1;

	for (int i = 0; i < board.size(); i++) {
		if (board[line][i] == p) {

			if (x_start == -1) {
				x_start = i;
			}
			else {
				x_end = i;
			}

		}
	}

	if (x_end == -1) //if x_start == -1 then always x_end == -1
		return pair<Position, Position>(Position::create_invalid(), Position::create_invalid());
	else
		return pair<Position, Position>(Position(x_start, line), Position(x_end, line));
}

pair<Position, Position> limits_in_y(PLAYER p, vector<vector<PLAYER>>& board, int row) {
	int y_start = -1;
	int y_end = -1;

	for (int i = 0; i < board.size(); i++) {
		if (board[i][row] == p) {

			if (y_start == -1) {
				y_start = i;
			}
			else {
				y_end = i;
			}

		}
	}

	if (y_end == -1) //if y_start == -1 then always y_end == -1
		return pair<Position, Position>(Position::create_invalid(), Position::create_invalid());
	else
		return pair<Position, Position>(Position(row, y_start), Position(row, y_end));
}

//TODO FIX STARTING VALUES
pair<Position, Position> limits_in_main_diag(PLAYER player, vector<vector<PLAYER>>& board, Position pos) {
	int x_start = -1;
	int x_end = -1;
	int y_start = -1;
	int y_end = -1;

	int curr_x;
	int curr_y;

	//find start of diagonals
	if (pos.X() > pos.Y()) {
		curr_y = 0;
		curr_x = pos.X() - pos.Y();		
	}
	else {
		curr_x = 0;
		curr_y = pos.Y() - pos.X();
	}
	cout << "start of main diag: " << curr_y << "-" << curr_x << endl;

	//start scanning
	while (curr_x < board.size() && curr_y < board.size()) {
		if (board[curr_y][curr_x] == player) {

			if (x_start == -1) {
				x_start = curr_x;
				y_start = curr_y;
			}
			else {
				x_end = curr_x;
				y_end = curr_y;
			}

		}
		curr_x++;
		curr_y++;
	}

	if (y_end == -1) //if y_start == -1 then always y_end == -1
		return pair<Position, Position>(Position::create_invalid(), Position::create_invalid());
	else
		return pair<Position, Position>(Position(x_start, y_start), Position(x_end, y_end));
}

//TODO FIX STARTING VALUES
pair<Position, Position> limits_in_sec_diag(PLAYER player, vector<vector<PLAYER>>& board, Position pos) {
	int x_start = -1;
	int x_end = -1;
	int y_start = -1;
	int y_end = -1;

	int curr_x = pos.X();
	int curr_y = pos.Y();

	//seek start of diagonal
	while (curr_x > 0 && curr_y < board.size()) {
		curr_x--;
		curr_y++;
	}

	cout << "start of sec diag: " << curr_y << "-" << curr_x << endl;

	//start scanning
	while (curr_x < board.size() && curr_y >= 0) {
		cout << "square " << curr_y << "-" << curr_x << ((board[curr_y][curr_x] == player) ? " is" : " is not") << " player controlled" << endl;
		if (board[curr_y][curr_x] == player) {

			if (x_start == -1) {
				x_start = curr_x;
				y_start = curr_y;
			}
			else {
				x_end = curr_x;
				y_end = curr_y;
			}

		}
		curr_x++;
		curr_y--;
	}

	cout << "end of sec diag: " << y_end << "-" << x_end << endl;

	if (y_end == -1) //if y_start == -1 then always y_end == -1
		return pair<Position, Position>(Position::create_invalid(), Position::create_invalid());
	else
		return pair<Position, Position>(Position(x_start, y_start), Position(x_end, y_end));
}

string toString(vector<vector<PLAYER>>& gameBoard) {
	string str;
	int DIMENSION = gameBoard.size();

	for (int i = 0; i < DIMENSION; i++) {
		for (int j = 0; j < DIMENSION; j++) {
			str += static_cast<char>(gameBoard[i][j]); //append player character
			str += " ";
		}
		str += "\n";
	}

	//returning by value is efficient in strings
	return str;
}
