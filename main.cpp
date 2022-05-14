#include <iostream>
#include <ctime>

#include "common.hpp"
#include "mnk.hpp"
#include "mcts.hpp"

int main(int argc, char** argv)
{
	if (argc != 5) {
		std::cout << "Enter width, height, in_a_row needed to win,"
			<< " and AI iterations as command line arguments.";
		return 0;
	}

	int width = std::stoi(argv[1]);
	int height = std::stoi(argv[2]);
	int in_a_row = std::stoi(argv[3]);
	int iterations = std::stoi(argv[4]);

	if (width < 1) {
		std::cout << "Error: width too small.";
		return 0;
	}
	if (height < 1) {
		std::cout << "Error: height too small.";
		return 0;
	}
	if (in_a_row < 1) {
		std::cout << "Error: in_a_row too small";
		return 0;
	}
	if (in_a_row > std::max(width, height)) {
		std::cout << "Error: in_a_row too large, impossible to win.";
		return 0;
	}
	if (iterations < 1) {
		std::cout << "Error: iterations too small";
		return 0;
	}

	srand(time(NULL));
	int human = 1 + rand() % 2;
	mnk::State state(width, height, in_a_row);
	state.Print();
	while (true) {
		mnk::Move move;
		if (state.current_player == human) {
			std::cout << "Enter move (x, y): ";
			int x, y;
			std::cin >> x >> y;
			move = mnk::Move(x-1, y-1);
			if (!state.LegalMove(move)) {
				std::cout << "Illegal move, try again.\n";
				continue;
			}
		} else {
			std::cout << "AI is thinking...\n";
			mcts::Node root(&state);
			move = root.Ai(iterations);
			// std::cout << root.w / root.n << '\n';
		}
		state.PlayMove(move);

		state.Print();

		if (state.Won()) {
			if (state.last_player == common::kPlayer1) {
				std::cout << "X has won!\n";
			} else {
				std::cout << "O has won!\n";
			}
			break;
		}
		if (state.Drawn()) {
			std::cout << "It's a draw.\n";
			break;
		}
	}

	return 0;
}