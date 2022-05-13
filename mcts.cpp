#include "mcts.hpp"
#include <cmath>
#include <cstdlib>

mcts::node::node()
{
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
	node *next_node;
	for (node *child: children)
		if (child->last_move == move)
			next_node = child;
		else
			delete child;
	return next_node;
}

void mcts::node::expand(game::state *state)
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

common::player mcts::rollout(game::state *state)
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

common::player mcts::iterate(game::state *state, node *node)
{
	common::player winner;
	if (node->is_leaf)
	{
		winner = rollout(state);
		if (!state->won())
			node->expand(state);
	}
	else
	{
		mcts::node *next_node = node->max_child();
		state->play_move(next_node->last_move);
		winner = iterate(state, next_node);
		state->undo_move();
	}
	if (winner == state->current_player)
		node->w += 1;
	if (winner == common::neither)
		node->w += 0.5;
	node->n++;
	return winner;
}

mcts::node *mcts::AI(game::state *state, node *node, int iterations)
{
	for (int i = 0; i < iterations; i++)
		iterate(state, node);
	mcts::node *max_child = node->children[0];
	for (mcts::node *child: node->children)
		if (child->n > max_child->n)
			max_child = child;
	return max_child;
}