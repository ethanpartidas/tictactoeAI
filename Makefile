
release: main.o mcts.o mnk.o
	g++ main.o mcts.o mnk.o -o tictactoeAI

debug: main.o mcts.o mnk.o
	g++ -g main.o mcts.o mnk.o -o tictactoeAI

main.o: main.cpp
	g++ -c main.cpp -o main.o

mcts.o: mcts.cpp mcts.hpp
	g++ -c mcts.cpp -o mcts.o

mnk.o: mnk.cpp mnk.hpp
	g++ -c mnk.cpp -o mnk.o