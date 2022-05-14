#include "mnk.hpp"

#include <iostream>

mnk::state::state(int width_, int height_, int in_a_row_)
{
	width = width_;
	height = height_;
	in_a_row = in_a_row_;
	board = new common::player*[width];
	for (int x = 0; x < width; ++x)
		board[x] = new common::player[height]{common::neither};
}
	
mnk::state::~state()
{
	for (int x = 0; x < width; ++x)
		delete[] board[x];
	delete[] board;
}

void mnk::state::flip_player()
{
	common::player temp = last_player;
	last_player = current_player;
	current_player = temp;
}

bool mnk::state::valid_move(move move)
{
	return move.x >= 0 && move.x < width
		&& move.y >=0 && move.y < height;
}

bool mnk::state::legal_move(move move)
{
	return valid_move(move)
		&& board[move.x][move.y] == common::neither;
}

std::vector<mnk::move> mnk::state::legal_moves()
{
	std::vector<move> legal_moves;
	for (int x = 0; x < width; ++x)
		for (int y = 0; y < height; ++y)
			if (board[x][y] == common::neither)
				legal_moves.emplace_back(x, y);
	return legal_moves;
}

void mnk::state::play_move(move move)
{
	board[move.x][move.y] = current_player;
	move_history.push_back(move);
	flip_player();
}

void mnk::state::play_random_move()
{
	while (true)
	{
		mnk::move move = mnk::move(rand() % width, rand() % height);
		if (legal_move(move))
		{
			play_move(move);
			break;
		}
	}
}

void mnk::state::undo_move()
{
	move last_move = move_history.back();
	board[last_move.x][last_move.y] = common::neither;
	move_history.pop_back();
	flip_player();
}

void mnk::state::print()
{
	std::cout << '\n';
	for (int y = height - 1; y >= 0; --y)
	{
		for (int x = 0; x < width; ++x)
		{
			if (board[x][y] == common::player_1)
				std::cout << 'X';
			else if (board[x][y] == common::player_2)
				std::cout << 'O';
			else
				std::cout << '_';
			
			if (x < width - 1)
				std::cout << '|';
			else
				std::cout << '\n';
		}
	}
}

bool mnk::state::won()
{
	if (move_history.size() == 0)
		return false;
	
	move last_move = move_history.back();
	
	// horizontal check
	int count = 0;
	for (int x = last_move.x, y = last_move.y;
		valid_move(move(x, y)) && board[x][y] == last_player;
		++x)
		++count;
	for (int x = last_move.x, y = last_move.y;
		valid_move(move(x, y)) && board[x][y] == last_player;
		--x)
		++count;
	if (count > in_a_row)
		return true;

	// vertical check
	count = 0;
	for (int x = last_move.x, y = last_move.y;
		valid_move(move(x, y)) && board[x][y] == last_player;
		++y)
		++count;
	for (int x = last_move.x, y = last_move.y;
		valid_move(move(x, y)) && board[x][y] == last_player;
		--y)
		++count;
	if (count > in_a_row)
		return true;

	// diagonal check
	count = 0;
	for (int x = last_move.x, y = last_move.y;
		valid_move(move(x, y)) && board[x][y] == last_player;
		++x, ++y)
		++count;
	for (int x = last_move.x, y = last_move.y;
		valid_move(move(x, y)) && board[x][y] == last_player;
		--x, --y)
		++count;
	if (count > in_a_row)
		return true;

	// diagonal check
	count = 0;
	for (int x = last_move.x, y = last_move.y;
		valid_move(move(x, y)) && board[x][y] == last_player;
		++x, --y)
		++count;
	for (int x = last_move.x, y = last_move.y;
		valid_move(move(x, y)) && board[x][y] == last_player;
		--x, ++y)
		++count;
	if (count > in_a_row)
		return true;

	return false;
}

bool mnk::state::drawn()
{
	return move_history.size() == width * height;
}