#include "mcts.hpp"
#include <cmath>
#include <cstdlib>

game::state *mcts::node::state;

mcts::node::node(game::state *state_)
{
	state = state_;
}

mcts::node::node(game::move last_move_)
{
	last_move = last_move_;
}

mcts::node::~node()
{
	for (auto it = children.begin(); it != children.end(); ++it)
		delete *it;
}

mcts::node *mcts::node::play_move(game::move move)
{
	if (is_leaf)
		expand();
	node *next_node;
	for (node *child: children)
		if (child->last_move == move)
			next_node = child;
		else
			delete child;
	return next_node;
}

void mcts::node::expand()
{
	for (game::move m: state->legal_moves())
	{
		children.push_back(new node(m));
		is_leaf = false;
	}
}

double mcts::node::UCT(int N)
{
	return (n - w) / (n + 1) + sqrt(2 * log(N + 1) / (n + 1));
}

mcts::node *mcts::node::max_child()
{
	node *max_child = children[0];
	for (node *child: children)
		if (child->UCT(n) > max_child->UCT(n))
			max_child = child;
	return max_child;
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
		moves_played++;
	}
	for (int i = 0; i < moves_played; i++)
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
		mcts::node *next_node = max_child();
		state->play_move(next_node->last_move);
		winner = next_node->iterate();
		state->undo_move();
	}
	if (winner == state->current_player)
		w += 1;
	if (winner == common::neither)
		w += 0.5;
	n++;
	return winner;
}

game::move mcts::node::AI(int iterations)
{
	for (int i = 0; i < iterations; i++)
		iterate();
	node *max_child = children[0];
	for (node *child: children)
		if (child->n > max_child->n)
			max_child = child;
	return max_child->last_move;
}