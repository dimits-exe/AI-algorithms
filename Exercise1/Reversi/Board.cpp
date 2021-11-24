#include "Board.h"
#include <string> 
#include <iostream>

using namespace std;

Board::Board(int size) : DIMENSION(size) {
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

Board::Board(const Board& oldBoard) { 
	*this = oldBoard; 
}

void Board::operator=(const Board& other) {
	this->DIMENSION = other.DIMENSION;
	this->gameBoard = other.gameBoard;
	this->lastMovePlayed = other.lastMovePlayed;
}

Position Board::getLastMove() const {
	return lastMovePlayed;
}

list<Position> Board::getValidMoves(PLAYER p) const{
	list<Position> ls;

	//exhaust all options, fuck performance
	for (int i = 0; i < DIMENSION; i++) {
		for (int j = 0; j < DIMENSION; j++) {

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

	bool x_cor = true;
	bool y_cor = true;
	bool diag_1_cor = true;
	bool diag_2_cor = true;

	pair<Position, Position> move_x = limits_in_x(p, testBoard, move);
	if (move_x.first.X() == move_x.second.X())
		x_cor = false;

	pair<Position, Position> move_y = limits_in_y(p, testBoard, move);
	if (move_y.first.Y() == move_y.second.Y())
		y_cor = false;

	pair<Position, Position> move_diag_1 = limits_in_main_diag(p, testBoard, move);
	if (move_diag_1.first.X() == move_diag_1.second.X())
		diag_1_cor = false;

	pair<Position, Position> move_diag_2 = limits_in_sec_diag(p, testBoard, move);
	if (move_diag_2.first.X() == move_diag_2.second.X())
		diag_2_cor = false;

	return x_cor || y_cor || diag_1_cor || diag_2_cor;
}

void Board::makeMove(PLAYER p, Position move) {
	bool pair_exists = false;
	PLAYER prevValue = gameBoard[move.Y()][move.X()];
	
	if(prevValue != PLAYER::EMPTY)
		throw logic_error("The selected square isn't empty");

	gameBoard[move.Y()][move.X()] = p;

	//check pairs on the x axis
	pair<Position, Position> x_axis = limits_in_x(p, gameBoard, move);

	//flip all pieces to the player's control
	for (int i = x_axis.first.X(); i < x_axis.second.X(); i++) {
		gameBoard[move.Y()][i] = p;
		pair_exists = true;
	}
	
	//check pairs on the y axis
	pair<Position, Position> y_axis = limits_in_y(p, gameBoard, move);
	//flip all pieces to the player's control
	for (int i = y_axis.first.Y(); i < y_axis.second.Y(); i++) {
		gameBoard[i][move.X()] = p;
		pair_exists = true;
	}
	
	//check pairs on the left-to-right diagonal 
	pair<Position, Position> main_diag = limits_in_main_diag(p, gameBoard, move);
	//flip all pieces to the player's control
	int curr_x = main_diag.first.X();
	int curr_y = main_diag.first.Y();
	while (curr_x <= main_diag.second.X() && curr_y < main_diag.second.Y()) {
		gameBoard[curr_y][curr_x] = p;
		curr_x++;
		curr_y++;
		pair_exists = true;
	}
	
	
	//check pairs on the right_to_left diagonal 
	pair<Position, Position> sec_diag = limits_in_sec_diag(p, gameBoard, move);
	curr_x = sec_diag.first.X();
	curr_y = sec_diag.second.Y();
	while (curr_x <= sec_diag.second.X() && curr_y <= sec_diag.second.Y()
		&& curr_y >= 0) {
		gameBoard[curr_y][curr_x] = p;
		curr_x++;
		curr_y--;
		pair_exists = true;
	}
	
	//if (!pair_exists) {
		//gameBoard[move.Y()][move.X()] = prevValue;
		//throw logic_error("Invalid move x=" + std::to_string(move.X()) + " y=" + std::to_string(move.Y()) + " in board \n" + toString());
	//}
	//else {
		//lastMovePlayed = move;
	//}
	lastMovePlayed = move;
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

	for (int i = 0; i < this->DIMENSION; i++) {
		for (int j = 0; j < this->DIMENSION; j++) {

			if (gameBoard[i][j] == PLAYER::PLAYER1) {
				pl1Count++;
			}
			else if (gameBoard[i][j] == PLAYER::PLAYER2) {
				pl2Count++;
			}

		}
	}

	if (p == PLAYER::PLAYER1)
		return pl1Count - pl2Count;
	else if (p == PLAYER::PLAYER2)
		return pl2Count - pl1Count;
}
			

string Board::toString() const {
	string str (" ");

	//build horizontal header
	for (int i = 1; i <= DIMENSION; i++)
		str += to_string(i) + " ";
	str += "\n";

	for (int i = 0; i < DIMENSION; i++) {
		str += to_string(i+1);
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

auto get_range(int min, int max) {
	return [min, max](int value) {
		return (min <= value) && (value < max);
	};
}

pair<Position, Position> Board::limits_everywhere(PLAYER p, const vector<vector<PLAYER>>& board, Position last_move, int dx, int dy)
{
	int max = board.size();
	auto check_range = get_range(0, max);

	int sx = last_move.X(), sy = last_move.Y(), ex = last_move.X(), ey = last_move.Y();

	if (check_range(sx - dx) && check_range(sy - dy))
		// if (sx - dx >= 0 && sy - dy >= 0)
	{
		int i = sx - dx;
		int j = sy - dy;
		for (; check_range(i) && check_range(j) && board[j][i] != PLAYER::EMPTY; i -= dx, j -= dy)
		{
			if (board[j][i] == p) {
				sx = i;
				sy = j;
				break;
			}
		}
	}

	if (check_range(ex + dx) && check_range(ey + dy))
		// if (ex + dx < max && ey + dy < max)
	{
		int i = ex + dx;
		int j = ey + dy;
		for (; check_range(i) && check_range(j) && board[j][i] != PLAYER::EMPTY; i += dx, j += dy)
		{
			if (board[j][i] == p) {
				ex = i;
				ey = j;
				break;
			}
		}
	}

	return pair<Position, Position>(Position(sx, sy), Position(ex, ey));
}
pair<Position, Position>  Board::limits_in_x(PLAYER p, const vector<vector<PLAYER>>& board, Position last_move)
{
	return limits_everywhere(p, board, last_move, 1, 0);
}

pair<Position, Position>  Board::limits_in_y(PLAYER p, const vector<vector<PLAYER>>& board, Position last_move)
{
	return limits_everywhere(p, board, last_move, 0, 1);
}

pair<Position, Position>  Board::limits_in_main_diag(PLAYER p, const vector<vector<PLAYER>>& board, Position last_move)
{
	return limits_everywhere(p, board, last_move, 1, 1);
}

pair<Position, Position>  Board::limits_in_sec_diag(PLAYER p, const vector<vector<PLAYER>>& board, Position last_move)
{
	return limits_everywhere(p, board, last_move, 1, -1);
}
