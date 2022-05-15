#include "common.hpp"
#include "mcts.hpp"

#include <cmath>

game::State* mcts::Node::state;

mcts::Node::Node(game::State* state_) {
	state = state_;
}

void mcts::Node::Expand() {
	std::vector<game::Move> legal_moves = state->LegalMoves();
	children.reserve(legal_moves.size());
	for (game::Move move : legal_moves) {
		children.emplace_back(move);
		is_leaf = false;
	}
}

double mcts::Node::Uct(int N) {
	return (n - w) / (n + 1) + sqrt(2 * log(N + 1) / (n + 1));
}

mcts::Node& mcts::Node::MaxChild() {
	int max_i;
	double max_uct = 0;
	for (int i = 0; i < children.size(); ++i) {
		double child_uct = children[i].Uct(n);
		if (child_uct >= max_uct) {
			max_i = i;
			max_uct = child_uct;
		}
	}
	return children[max_i];
}

common::Player mcts::Node::Rollout() {
	int moves_played = 0;
	while (!state->GameOver()) {
		state->PlayRandomMove();
		++moves_played;
	}
	common::Player winner = state->winner;
	for (int i = 0; i < moves_played; ++i) {
		state->UndoMove();
	}
	return winner;
}

common::Player mcts::Node::Iterate() {
	common::Player winner;
	if (is_leaf) {
		winner = Rollout();
		if (!state->GameOver()) {
			Expand();
		}
	} else {
		Node& next_node = MaxChild();
		state->PlayMove(next_node.last_move);
		winner = next_node.Iterate();
		state->UndoMove();
	}
	if (winner == state->current_player) {
		w += 1;
	} else if (winner == common::kNeither) {
		w += 0.5;
	}
	++n;
	return winner;
}

game::Move mcts::Node::Ai(int iterations) {
	for (int i = 0; i < iterations; ++i) {
		Iterate();
	}
	int max_i = 0;
	for (int i = 1; i < children.size(); ++i) {
		if (children[i].n > children[max_i].n) {
			max_i = i;
		}
	}
	return children[max_i].last_move;
}