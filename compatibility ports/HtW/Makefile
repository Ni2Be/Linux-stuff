#Makefile
CXX=g++
CXXFLAGS=-std=c++14
RM=rm -f

htw: HtW_Game.o Labyrinth.o main.cpp
	$(CXX) $(CXXFLAGS) -o htw HtW_Game.o Labyrinth.o main.cpp
HtW_Game.o: HtW_Game.cpp Labyrinth.o
	$(CXX) -c $(CXXFLAGS) -o HtW_Game.o Labyrinth.o HtW_Game.cpp
Labyrinth.o: Labyrinth.cpp
	$(CXX) -c $(CXXFLAGS) -o Labyrinth.o Labyrinth.cpp

clean:
	$(RM) htw HtW_Game.o Labyrinth.o 
