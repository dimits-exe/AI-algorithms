#include "Board.h"

Board::Board(const Board& oldBoard) {
	for (int i = 0; i < DIMENSION; i++)
		for (int j = 0; i < DIMENSION; j++)
			board[i][j] = oldBoard.board[i][j];
}

list<Position> Board::get_valid_moves(PLAYER p) { 
	list<Position> ls;

	//exhaust all options, fuck performance
	for (int i = 0; i < DIMENSION;i++) {
		for (int j = 0; i < DIMENSION;j++) {

			Position move(i, j);
			if (is_valid_move(p, move)) {
				ls.push_back(move);
			}
				
		}
	}

	return ls; //copy the whole list and return it
}

bool Board::is_valid_move(PLAYER p, Position move) {
	if (!range_is_valid(move))
		return false;

	if (board[move.X()][move.Y()] != PLAYER::EMPTY)
		return false;

	if(	Position::is_invalid(limits_in_x(p, move.Y()).first) ||
		Position::is_invalid(limits_in_y(p, move.X()).first) ||
		Position::is_invalid(limits_in_main_diag(p, move).first) ||
		Position::is_invalid(limits_in_sec_diag(p, move).first))
		return false;

	return true;
}

void Board::make_move(PLAYER p, Position move) {
	bool pair_exists = false;

	pair<Position, Position> x_axis = limits_in_x(p, move.Y());
	if (!Position::is_invalid(x_axis.first)) {
		pair_exists == true;
		for (int i = x_axis.first.X(); i < x_axis.second.X(); i++) {
			board[move.Y()][i] = p;
		}
	}
	
	pair<Position, Position> y_axis = limits_in_y(p, move.X());
	if (!Position::is_invalid(y_axis.first)) {
		pair_exists == true;
		for (int i = y_axis.first.Y(); i < y_axis.second.Y(); i++) {
			board[i][move.X()] = p;
		}
	}

	pair<Position, Position> main_diag = limits_in_main_diag(p, move);
	if (!Position::is_invalid(main_diag.first)) {
		pair_exists == true;
		int curr_x = main_diag.first.X();
		int curr_y = main_diag.second.Y();
		while (curr_x <= main_diag.second.X() && curr_y <= main_diag.second.Y()) {
			board[curr_y][curr_x] = p;
			curr_x++;
			curr_y++;
		}
	}

	pair<Position, Position> sec_diag = limits_in_sec_diag(p, move);
	if (!Position::is_invalid(sec_diag.first)) {
		pair_exists == true;
		int curr_x = sec_diag.first.X();
		int curr_y = sec_diag.second.Y();
		while (curr_x <= sec_diag.second.X() && curr_y <= sec_diag.second.Y()) {
			board[curr_y][curr_x] = p;
			curr_x++;
			curr_y--;
		}
	}

	if (!pair_exists)
		throw logic_error("Invalid move " + move);
}


bool Board::range_is_valid(Position move) {
	if (move.X() > DIMENSION || move.Y() > DIMENSION || move.X() < 0 || move.Y() < 0)
		return false;
	else
		return true;
}

//code duplication lololololol

pair<Position, Position> Board::limits_in_x(PLAYER p, int line) {
	int x_start = -1;
	int x_end = -1;

	for (int i = 0; i <= DIMENSION; i++) {
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

pair<Position, Position> Board::limits_in_y(PLAYER p, int row) {
	int y_start = -1;
	int y_end = -1;

	for (int i = 0; i <= DIMENSION; i++) {
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

pair<Position, Position> Board::limits_in_main_diag(PLAYER player, Position pos) {
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
	while (curr_x <= DIMENSION && curr_y <= DIMENSION) {
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

pair<Position, Position> Board::limits_in_sec_diag(PLAYER player, Position pos) {
	int x_start = -1;
	int x_end = -1;
	int y_start = -1;
	int y_end = -1;

	int curr_x;
	int curr_y;

	//find start of diagonals
	if (pos.X() > pos.Y()) {
		curr_x = DIMENSION;
		curr_y -= DIMENSION - pos.X();
	}
	else {
		curr_x -= DIMENSION - pos.Y();
		curr_y = DIMENSION;
	}

	//start scanning
	while (curr_x <= DIMENSION && curr_y <= DIMENSION) {
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