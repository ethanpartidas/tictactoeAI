
release: main.cpp mcts.cpp mcts.hpp mnk.cpp mnk.hpp
	g++ -O3 main.cpp mcts.cpp mnk.cpp -o tictactoeAI

debug: main.cpp mcts.cpp mcts.hpp mnk.cpp mnk.hpp
	g++ -g main.cpp mcts.cpp mnk.cpp -o tictactoeAI