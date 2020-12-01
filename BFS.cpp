/* 
BFS.cpp
COT5405 Project
Laura Battle
November 2020

This file implements Breadth-First Search on a directed, weighted
graph like the type created by Directed.exe. It returns the hop
distances for each vertex to the source provided at the command line.
The source defaults to 1 if no source is provided or if the source
provided is not contained in the Graph input in the file.

This program has two modes, regular and verbose. Regular mode will
output a list to the console of each vertex and its hop distance in
a list unless the number of nodes is greater than 100, in which case
it will output the list to a file "bfs.out" in the same directory.

To run the program in verbose mode you can either accept the default:

BFS.exe file.in

or provide a source:

BFS.exe file.in 5

To run the program in verbose mode, you can provide a 4th argument:

BFS.exe file.in 1 yes

Verbose mode will output the final state of each node including its
predecessor node. Please note that verbose mode is disabled on Graphs
with more than 100 nodes.

*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <queue>
#include <string>
#include <sys/time.h>
#include <DataStruc.h>

struct vertex
{
  int name;
  char color = 'w';
  int dist = NIL;  // -1
  int pred = NIL; // -1
};

void Dump(std::vector<vertex> &Graph);
void PrintAnswer(std::vector<vertex> &Graph, int s);
void StartTimer(timeval &start);
void EndTimer(const timeval start, timeval end, double time, int nodes, int edges);


void BFS(const std::vector<std::vector<edge>> &AdjList, bool verbose, int &s, 
         int numVertex, int numEdges)
{
  std::string filename = "bfs.time";
  std::vector<int> empty;
  if(s >= numVertex)
  {
    std::cerr << "\n**Error, argument greater than number of nodes"
	      << "\nSource reset to default of s = 1\n";
    s = 1;
  }
  std::vector<vertex> Graph(numVertex);

  //START TIME                                                                                       
  timeval start, end;
  double time;
  StartTimer(start);
  //START TIME

  for (int i = 0; i < numVertex; ++i)
  {
    Graph[i].name = i;
  }
  Graph[s].color = 'g';
  Graph[s].dist = 0;
  int u;
  int n;
  int ver;
  std::queue<int> Queue;
  Queue.push(s);
  while(!Queue.empty())
  {
    u = Queue.front();
    Queue.pop();
    n = AdjList[u].size();
    for(int i = 0; i < n; ++i)
    {
      ver = AdjList[u][i].v;
      if(Graph[ver].color == 'w')
      {
	Graph[ver].color = 'g';
	Graph[ver].dist = Graph[u].dist + 1;
	Graph[ver].pred = u;
	Queue.push(ver);
      }
    }
    Graph[u].color = 'b';
  }

  // END TIME                                                                                        
  EndTimer(start, end, time, (numVertex - 1), numEdges, filename);
  // END TIME 

  if(numVertex > 100)
    verbose = false;

  if(verbose)
  {
    Dump(Graph);
  }

  PrintAnswer(Graph, s);
}

int main(int argc, char** argv)
{
  std::string filename;
  bool verbose = false;
  int source = 1;
  int numVertex, numEdges;
  if (argc == 2)
  {
    filename = argv[1];
    std::cout << "\n\nWelcome to BFS, source defaults to 1. Consult README to alter source.\n\n";
  }
  else if (argc == 3)
  {
    filename = argv[1];
    sscanf(argv[2], "%d", &source);
  }
  else if (argc == 4)
    {
      filename = argv[1];
      sscanf(argv[2], "%d", &source);
      verbose = true;
    }
  else
  {
    std::cerr << "\nTo run this program you need one input file:\n"
	      << "eg:\n\n\t\t BFS.exe default.in\n\n";
    exit(EXIT_FAILURE);
  }

  std::vector<edge> Edges;
  std::vector<std::vector<edge>> AdjList;
  edge empty = {0, 0};
  Edges.push_back(empty);
  AdjList.push_back(Edges);

  DataStruc(filename, AdjList, Edges, empty, numVertex, numEdges); 

  BFS(AdjList, verbose, source, numVertex, numEdges);
  return 0;
}

void PrintAnswer(std::vector<vertex> &Graph, int source)
{
  std::cout << "\nBFS complete.";
  int size = Graph.size();
  if (size < 101)
  {
    std::cout << "\n Hop distances from s = " << source << ":"
	      << "\n\nVertex\tHop Distance";
  for (int i = 1; i < size; ++i)
  {
    std::cout << "\n  " << Graph[i].name << "\t";
    if(Graph[i].dist == -1)
      std::cout << "infinity";
    else
      std::cout << Graph[i].dist;
  }
  std::cout << "\n\n";
  }
  else
  {
    std::cout << "\n\n\tRedirected to file, 'bfs.out,' -1 represents infinity\n\n";
    std::ofstream fout;
    fout.open("bfs.out");
    fout << "Vertex\tHop Distance";
    for (int i = 1; i < size; ++i)
    {
      fout << "\n  " << Graph[i].name << "\t" << Graph[i].dist;
    }
    fout.close();
  }
}

void Dump(std::vector<vertex> &Graph)
{
  std::cout << "\n\nDump of Graph:\n";
  int size = Graph.size();
  for (int i = 1; i < size; ++i)
  {
    std::cout << "vertex: " << Graph[i].name
	      << "\tcolor: " << Graph[i].color
	      << "\tdistance: ";
    if(Graph[i].dist == NIL)
      std::cout << "infinity";
    else
      std::cout << Graph[i].dist << "     ";

    std::cout << "\tpredecessor: ";
    if(Graph[i].pred == NIL)
      std::cout << "nil";
    else
      std::cout << Graph[i].pred;
    std::cout  << "\n";
  }
}
