#include "mnk.hpp"

#include <iostream>

mnk::State::State(int width_, int height_, int in_a_row_) {
	width = width_;
	height = height_;
	in_a_row = in_a_row_;
	board = std::vector<std::vector<common::Player>>(width,
		std::vector<common::Player>(height, common::kNeither));
}

void mnk::State::FlipPlayer() {
	common::Player temp = last_player;
	last_player = current_player;
	current_player = temp;
}

bool mnk::State::ValidMove(Move move) {
	return move.x >= 0 && move.x < width &&
		move.y >=0 && move.y < height;
}

bool mnk::State::LegalMove(Move move) {
	return ValidMove(move) && board[move.x][move.y] == common::kNeither;
}

std::vector<mnk::Move> mnk::State::LegalMoves() {
	std::vector<Move> LegalMoves;
	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
			if (board[x][y] == common::kNeither) {
				LegalMoves.emplace_back(x, y);
			}
		}
	}
	return LegalMoves;
}

void mnk::State::PlayMove(Move move) {
	board[move.x][move.y] = current_player;
	move_history.push_back(move);
	FlipPlayer();
}

void mnk::State::PlayRandomMove() {
	while (true) {
		Move move(rand() % width, rand() % height);
		if (LegalMove(move)) {
			PlayMove(move);
			break;
		}
	}
}

void mnk::State::UndoMove() {
	Move last_move = move_history.back();
	board[last_move.x][last_move.y] = common::kNeither;
	move_history.pop_back();
	FlipPlayer();
}

void mnk::State::Print() {
	std::cout << '\n';
	for (int y = height - 1; y >= 0; --y) {
		for (int x = 0; x < width; ++x) {
			if (board[x][y] == common::kPlayer1) {
				std::cout << 'X';
			} else if (board[x][y] == common::kPlayer2) {
				std::cout << 'O';
			} else {
				std::cout << '_';
			}
			
			if (x < width - 1) {
				std::cout << '|';
			} else {
				std::cout << '\n';
			}
		}
	}
}

bool mnk::State::Won() {
	if (move_history.size() == 0) {
		return false;
	}
	Move last_move = move_history.back();
	
	auto CountRay = [&](int delta_x, int delta_y) {
		int count = 0;
		for (int x = last_move.x, y = last_move.y;
				ValidMove(Move(x, y)) && board[x][y] == last_player;
				x += delta_x, y += delta_y) {
			++count;
		}
		return count;
	};

	return CountRay(1, 0) + CountRay(-1, 0) > in_a_row ||
		CountRay(0, 1) + CountRay(0, -1) > in_a_row ||
		CountRay(1, 1) + CountRay(-1, -1) > in_a_row ||
		CountRay(1, -1) + CountRay(-1, 1) > in_a_row;
}

bool mnk::State::Drawn() {
	return move_history.size() == width * height;
}