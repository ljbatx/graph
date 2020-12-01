/* 
DFS.cpp
COT5405 Project
Laura Battle
November 2020

This file implements Depth First Search on undirected graphs created
by the file Undirected.exe. It returns the number of connected components
in the graph.

This program has two modes, regular and verbose. Regular mode will simply
report the number of connected components. To run DFS in this mode:

DFS.exe file.in

If you would like to see the final state of each vertex in DFS including
their precessor, discovery time, and finishing time, you can run the 
program in verbose mode by providing a third argument like so:

DFS.exe file.in yes

**Please note that verbose mode is disabled when the number of vertices
exceeds 100.

*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <sys/time.h>
#include <DataStruc.h>

struct vertex
{
  int name;
  char color = 'w';
  int pred = NIL;
  int disc = NIL;
  int fin = NIL; //NIL = -1
};

struct pair
{
  int node;
  int time;
};

void Dump(std::vector<vertex> &Graph);
void PrintAnswer(int connectedComponents);

void DFS_Visit(int u, int &time, std::vector<vertex> &Graph, 
               const std::vector<std::vector<edge>> &AdjList)
{
  ++time;
  int ver;
  pair done;
  Graph[u].disc = time;
  Graph[u].color = 'g';
  int n = AdjList[u].size();
  for(int i = 0; i < n; ++i)
  {
    ver = AdjList[u][i].v;    
    if(Graph[ver].color == 'w')
    {
      Graph[ver].pred = u;
      DFS_Visit(ver, time, Graph, AdjList);
    }
  }
  Graph[u].color = 'b';
  ++time;
  Graph[u].fin = time;
}

void DFS(const std::vector<std::vector<edge>> &AdjList, bool verbose, 
         int &connectedComponents, int numVertex, int numEdges)
{
  int v, x;
  std::vector<vertex> Graph(numVertex);
  std::string filename = "dfs.time";
  //START TIME                                                                                       
  timeval start, end;
  double timer;
  StartTimer(start);
  //START TIME 

  for (int i = 0; i < numVertex; ++i)
  {
    Graph[i].name = i;
  }
  int time = 0;
  for(int i = 1; i < numVertex; ++i)
  {
    if(Graph[i].color == 'w')
    {
      ++connectedComponents;
      DFS_Visit(i, time, Graph, AdjList);
    }
  }

  // END TIME                                                                                        
  EndTimer(start, end, timer, (numVertex - 1), numEdges, filename);
  // END TIME 

  if(numVertex > 100)
  {
    //   verbose = false;
   } 
  if(verbose)
  {
    std::cout << "\nAfter DFS is called on undirected graph:";
    Dump(Graph);
  }
}

int main(int argc, char** argv)
{
  std::string filename;
  bool verbose = false;
  int numVertex, numEdges;
  int connectedComponents = 0;
  if (argc == 2)
    filename = argv[1];
  else if (argc == 3)
  {
    verbose = true;
    filename = argv[1];
  }
  else
  {
    std::cerr << "\nTo run this program you need one input file:\n"
	      << "eg:\n\n\t\t DFS.exe default.in\n\n";
    exit(EXIT_FAILURE);
  }

  std::vector<edge> Edges;
  std::vector<std::vector<edge>> AdjList;
  edge empty = {0, 0};
  Edges.push_back(empty);
  AdjList.push_back(Edges);

  DataStruc(filename, AdjList, Edges, empty, numVertex, numEdges); 

  DFS(AdjList, verbose, connectedComponents, numVertex, numEdges);
  PrintAnswer(connectedComponents);
  return 0;
}

void PrintAnswer(int connectedComponents)
{
  std::cout << "\nDFS complete. This graph has " << connectedComponents
	    << " connected component(s)\n\n";

}

void Dump(std::vector<vertex> &Graph)
{
  std::cout << "\n\nDump of Graph:\n";
  int size = Graph.size();
  for (int i = 1; i < size; ++i)
  {
    std::cout << "vertex: " << Graph[i].name
	      << "\tcolor: " << Graph[i].color
	      << "\tprevious: " << Graph[i].pred
	      << "\tdiscovery: " << Graph[i].disc
	      << "\tfinish: " << Graph[i].fin
	      << "\n";
  }

}
