#ifndef MNK_HPP
#define MNK_HPP

#include "common.hpp"
#include <vector>
#include <set>

namespace mnk {
	struct move
	{
		int x;
		int y;
		move();
		move(int x_, int y_);
		bool operator==(const move& other);
		bool operator<(const move& other) const;
	};

	struct state
	{
		int width;
		int height;
		int in_a_row;
		common::player **board;
		common::player current_player = common::player_1;
		common::player last_player = common::player_2;
		std::vector<move> move_history;

		state(int width_, int height_, int in_a_row_);
		~state();

		void flip_player();
		bool valid_move(move move); // true if x and y are within bounds of board
		bool legal_move(move move); // true if valid and cell is empty
		std::vector<move> legal_moves();
		void play_move(move move);
		void play_random_move();
		void undo_move();
		void print();
		bool won();
		bool drawn();
	};
}

#endif