#include "Board.h"
#include <string> 

using namespace std;

Board::Board(int size):DIMENSION(size) {
	//construct vectors and initialize all squares to empty
	for (int i = 0; i < DIMENSION; i++) {
		this->gameBoard.push_back(vector<PLAYER>(size));
		for (int j = 0; j < DIMENSION; j++) {
			this->gameBoard[i][j] = PLAYER::EMPTY;
		}
	}

	//initial board setup
	Position middleSquare((DIMENSION - 1)/2, (DIMENSION - 1) /2);
	gameBoard[middleSquare.X()][middleSquare.Y()] = PLAYER::PLAYER1;
	gameBoard[middleSquare.X()+1][middleSquare.Y()+1] = PLAYER::PLAYER1;
	gameBoard[middleSquare.X()+1][middleSquare.Y()] = PLAYER::PLAYER2;
	gameBoard[middleSquare.X()][middleSquare.Y()+1] = PLAYER::PLAYER2;
}

Board::Board(const Board& oldBoard): DIMENSION(oldBoard.DIMENSION), gameBoard(oldBoard.gameBoard){}

list<Position> Board::getValidMoves(PLAYER p) const{
	list<Position> ls;

	//exhaust all options, fuck performance
	for (int i = 0; i < DIMENSION;i++) {
		for (int j = 0; j < DIMENSION;j++) {

			Position move(i, j);
			if (isValidMove(p, move)) {
				ls.push_back(move);
			}

		}
	}

	//copy the whole list and return it
	//valid moves should be few enough to make returning by value efficient
	return ls; 
}

bool Board::isValidMove(PLAYER p, Position move) const {
	if (!IsRangeValid(move))
		return false;

	if (gameBoard[move.Y()][move.X()] != PLAYER::EMPTY)
		return false;

	vector<vector<PLAYER>> testBoard(gameBoard);
	testBoard[move.Y()][move.X()] = p;

	if (Position::is_invalid(limits_in_x(p, testBoard, move.Y()).first) &&
		Position::is_invalid(limits_in_y(p, testBoard, move.X()).first) &&
		Position::is_invalid(limits_in_main_diag(p, testBoard, move).first) &&
		Position::is_invalid(limits_in_sec_diag(p, testBoard, move).first)) {
		return false;
	}

	return true;
}

void Board::makeMove(PLAYER p, Position move) {
	bool pair_exists = false;

	gameBoard[move.Y()][move.X()] = p;

	pair<Position, Position> x_axis = limits_in_x(p, gameBoard, move.Y());
	if (!Position::is_invalid(x_axis.first)) { 
		//if a pair was found
		pair_exists = true;
		//flip all pieces to the player's control
		for (int i = x_axis.first.X(); i < x_axis.second.X(); i++) {
			gameBoard[move.Y()][i] = p;
		}
	}

	pair<Position, Position> y_axis = limits_in_y(p, gameBoard, move.X());
	if (!Position::is_invalid(y_axis.first)) {
		//if a pair was found
		pair_exists = true;
		//flip all pieces to the player's control
		for (int i = y_axis.first.Y(); i < y_axis.second.Y(); i++) {
			gameBoard[i][move.X()] = p;
		}
	}

	pair<Position, Position> main_diag = limits_in_main_diag(p, gameBoard, move);
	if (!Position::is_invalid(main_diag.first)) {
		//if a pair was found
		pair_exists = true;
		//flip all pieces to the player's control
		int curr_x = main_diag.first.X();
		int curr_y = main_diag.second.Y();
		while (curr_x <= main_diag.second.X() && curr_y < main_diag.second.Y()) {
			gameBoard[curr_y][curr_x] = p;
			curr_x++;
			curr_y++;
		}
	}

	pair<Position, Position> sec_diag = limits_in_sec_diag(p, gameBoard, move);
	if (!Position::is_invalid(sec_diag.first)) {
		//if a pair was found
		pair_exists = true;
		//flip all pieces to the player's control
		int curr_x = sec_diag.first.X();
		int curr_y = sec_diag.second.Y();
		while (curr_x <= sec_diag.second.X() && curr_y <= sec_diag.second.Y()) {
			gameBoard[curr_y][curr_x] = p;
			curr_x++;
			curr_y--;
		}
	}

	if (!pair_exists) {
		gameBoard[move.Y()][move.X()] = PLAYER::EMPTY;
		throw logic_error("Invalid move " + std::to_string(move.X()) + "-" + std::to_string(move.Y()));
	}
		
}

double Board::hashcode() const {
	string str;

	for (int i = 0; i < DIMENSION; i++) {
		for (int j = 0; j < DIMENSION; j++) {
			str += static_cast<char>(gameBoard[i][j]);
		}
	}
	return hash<std::string>()(str);
}

int Board::getScore(PLAYER p) const {
	if (p == PLAYER::EMPTY)
		throw logic_error("Invalid EMPTY parameter on getScore");

	int pl1Count = 0;
	int pl2Count = 0;

	for (int i = 0; i < DIMENSION; i++) {
		for (int j = 0; j < DIMENSION; j++) {

			switch (gameBoard[i][j]) {
			case PLAYER::PLAYER1:
				pl1Count++;
				break;
			case PLAYER::PLAYER2:
				pl2Count++;
				break;
			}

		}
	}

	if (p == PLAYER::PLAYER1)
		return pl1Count - pl2Count;
	else if (p == PLAYER::PLAYER2)
		return pl2Count - pl1Count;
	else
		return INT_MIN; //unreachable because of if statement at the top
}
			

string Board::toString() const {
	string str;

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

bool Board::IsRangeValid(Position move) const {
	return move.X() < DIMENSION && move.Y() < DIMENSION
		&& move.X() >= 0 && move.Y() >= 0;
}

//code duplication lololololol

pair<Position, Position> Board::limits_in_x(PLAYER p, vector<vector<PLAYER>>& board, int line)  {
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

pair<Position, Position> Board::limits_in_y(PLAYER p, vector<vector<PLAYER>>& board, int row) {
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
pair<Position, Position> Board::limits_in_main_diag(PLAYER player, vector<vector<PLAYER>>& board, Position pos) {
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
pair<Position, Position> Board::limits_in_sec_diag(PLAYER player, vector<vector<PLAYER>>& board, Position pos) {
	int x_start = -1;
	int x_end = -1;
	int y_start = -1;
	int y_end = -1;

	int curr_x;
	int curr_y;

	//find start of diagonals
	if (pos.X() > pos.Y()) {
		curr_x = 0;
		curr_y = pos.Y() + pos.X();
	}
	else {
		curr_x = pos.Y() + pos.X();
		curr_y = 0;
	}

	//start scanning
	while (curr_x < board.size() && curr_y >= 0) {
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

	if (y_end == -1) //if y_start == -1 then always y_end == -1
		return pair<Position, Position>(Position::create_invalid(), Position::create_invalid());
	else
		return pair<Position, Position>(Position(x_start, y_start), Position(x_end, y_end));
}
