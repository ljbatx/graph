# makefile

CC= gcc
CXX= g++
DEBUG= -O0 -g

all: clean  Directed.exe Flow.exe Undirected.exe AdjListDS.exe DFS.exe BFS.exe Dijkstra.exe Prims.exe MinCut.exe

.cpp.o: DataStruc.h
	$(CXX) $(DEBUG) -c -I. -o $@ $< -std=c++11

Directed.exe: Directed.o DataStruc.o
	$(CXX) -o Directed.exe Directed.o DataStruc.o -std=c++11

Flow.exe: Flow.o DataStruc.o
	$(CXX) -o Flow.exe Flow.o DataStruc.o -std=c++11

Undirected.exe: Undirected.o DataStruc.o
	$(CXX) -o Undirected.exe Undirected.o DataStruc.o -std=c++11

AdjListDS.exe: AdjListDS.o
	$(CXX) -o AdjListDS.exe AdjListDS.o -std=c++11

DFS.exe: DFS.o DataStruc.o
	$(CXX) -o DFS.exe DFS.o DataStruc.o -std=c++11

BFS.exe: BFS.o DataStruc.o
	$(CXX) -o BFS.exe BFS.o DataStruc.o -std=c++11

Prims.exe: Prims.o DataStruc.o
	$(CXX) -o Prims.exe Prims.o DataStruc.o -std=c++11

Dijkstra.exe: Dijkstra.o DataStruc.o
	$(CXX) -o Dijkstra.exe Dijkstra.o DataStruc.o -std=c++11

MinCut.exe: MinCut.o DataStruc.o
	$(CXX) -o MinCut.exe MinCut.o DataStruc.o -std=c++11

.PHONY: clean

clean:
	rm -f *.o *~ \#*\#



