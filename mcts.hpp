#ifndef MCTS_HPP
#define MCTS_HPP

#include "mnk.hpp"
#include "common.hpp"
#include <vector>

namespace game = mnk;
/* game must implement:

struct move
{
	move();
}

struct state
{
	common::player last_player
	common::player current_player

	std::vector<move> legal_moves();
	void play_move(move move);
	void play_random_move();
	void undo_move();
	bool won(); // true if player_1 or player_2 has won
	bool drawn(); // true if game is drawn, can assume won() is false
}

*/

namespace mcts {
	
	struct node
	{
		game::move last_move = game::move();
		double w = 0;
		int n = 0;
		bool is_leaf = true;
		std::vector<node*> children;

		node();
		node(game::move last_move_);
		~node();

		node *play_move(game::move move);
		void expand(game::state *state);
		double UCT(int N);
		node *max_child();
	};

	common::player rollout(game::state *state);
	common::player iterate(game::state *state, node *node);
	node *AI(game::state *state, node *node, int iterations);
}

#endif