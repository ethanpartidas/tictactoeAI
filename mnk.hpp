#ifndef MNK_HPP
#define MNK_HPP

#include <vector>

#include "common.hpp"

namespace mnk {

struct Move {
	int x = -1;
	int y = -1;
	constexpr Move() {};
	Move(int x_, int y_) : x(x_), y(y_) {};
};

struct State {
	int width;
	int height;
	int in_a_row;
	common::Player **board;
	common::Player current_player = common::kPlayer1;
	common::Player last_player = common::kPlayer2;
	std::vector<Move> move_history;

	State(int width_, int height_, int in_a_row_);
	~State();

	void FlipPlayer();
	bool ValidMove(Move move); // true if x and y are within bounds of board
	bool LegalMove(Move move); // true if valid and cell is empty
	std::vector<Move> LegalMoves();
	void PlayMove(Move move);
	void PlayRandomMove();
	void UndoMove();
	void Print();
	bool Won();
	bool Drawn();
};

}  // namespace mnk

#endif  // MNK_HPP