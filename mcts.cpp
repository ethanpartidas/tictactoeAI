#include "mcts.hpp"

#include <cmath>

game::state *mcts::node::state;

mcts::node::node(game::state *state_)
{
	state = state_;
}

mcts::node::node(game::move last_move_)
{
	last_move = last_move_;
}

void mcts::node::expand()
{
	for (game::move move: state->legal_moves())
	{
		children.emplace_back(move);
		is_leaf = false;
	}
}

double mcts::node::UCT(int N)
{
	return (n - w) / (n + 1) + sqrt(2 * log(N + 1) / (n + 1));
}

mcts::node& mcts::node::max_child()
{
	int max_i;
	double max_UCT = 0;
	for (int i = 0; i < children.size(); ++i)
	{
		double child_UCT = children[i].UCT(n);
		if (child_UCT >= max_UCT)
		{
			max_i = i;
			max_UCT = child_UCT;
		}
	}
	return children[max_i];
}

common::player mcts::node::rollout()
{
	int moves_played = 0;
	common::player winner = common::neither;
	while (true)
	{
		if (state->won())
		{
			winner = state->last_player;
			break;
		}
		if (state->drawn())
			break;
		state->play_random_move();
		++moves_played;
	}
	for (int i = 0; i < moves_played; ++i)
		state->undo_move();
	return winner;
}

common::player mcts::node::iterate()
{
	common::player winner;
	if (is_leaf)
	{
		winner = rollout();
		if (!state->won())
			expand();
	}
	else
	{
		node& next_node = max_child();
		state->play_move(next_node.last_move);
		winner = next_node.iterate();
		state->undo_move();
	}
	if (winner == state->current_player)
		w += 1;
	if (winner == common::neither)
		w += 0.5;
	++n;
	return winner;
}

game::move mcts::node::AI(int iterations)
{
	for (int i = 0; i < iterations; ++i)
		iterate();
	int max_i = 0;
	for (int i = 1; i < children.size(); ++i)
		if (children[i].n > children[max_i].n)
			max_i = i;
	return children[max_i].last_move;
}