#ifndef MCTS_HPP
#define MCTS_HPP

#include <vector>

#include "common.hpp"
#include "mnk.hpp"

namespace game = mnk;
/* game must implement:

struct Move
{
	Move();
}

struct State
{
	common::Player last_player
	common::Player current_player

	std::vector<Move> LegalMoves();
	void PlayMove(Move move);
	void PlayRandomMove();
	void UndoMove();
	bool Won();  // true if kPlayer1 or kPlayer2 has won
	bool Drawn();  // true if game is drawn, can assume won() is false
}

*/

namespace mcts {

struct Node {
	game::Move last_move = game::Move();
	double w = 0;
	int n = 0;
	bool is_leaf = true;
	std::vector<Node> children;
	static game::State* state;

	Node(game::State* state_);
	Node(game::Move last_move_) : last_move(last_move_) {};

	void Expand();
	double Uct(int N);
	Node& MaxChild();

	static common::Player Rollout();
	common::Player Iterate();
	game::Move Ai(int iterations);
};

}  // namespace mcts

#endif  // MCTS_HPP