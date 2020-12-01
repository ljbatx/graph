/* 
Dijkstra.cpp
COT5405 Project
Laura Battle
November 2020

This file implements Dijkstra's shortest path algorithm on directed, weighted
graphs. It will output the shortest path distance from the given source for
each vertex in a list. For outputs of size 100 or less the output goes to the
console. For larger outputs, the list outputs to a file named "dijkstra.out"

You can generate directed, weighted graphs using Directed.exe

Source, if not given, will default to 1:

Dijkstra.exe input.in

If you want to change the source, you may:

Dijkstra.exe input.in 3

If your input source node is larger than the number of nodes in the given file
it will default back to 1.

If you would like to see the predecessor nodes you can enter verbose mode
by adding a 4th argument like so:

Dijkstra.exe input.in 1 yes

But please note that verbose mode is disabled on graph with over 100 nodes.

Runtimes will be appended to "dijkstra.time" in format:
nodes edges time

*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <deque>
#include <string>
#include <sys/time.h>
#include <DataStruc.h>

struct vertex
{
  int name;
  int dist = TINY_INFINITY;  //2.1 billion ish
  int pred = NIL;  // -1
};

void Dump(std::vector<vertex> &Graph);
void PrintAnswer(std::vector<vertex> &Graph, int s);
//void Print(vertex &v);              // for development
void RestoreMinHeap(std::deque<vertex*> &Queue);
int ExtractMin(std::deque<vertex*> &Queue);
int Parent(int index);

void Dijkstra(const std::vector<std::vector<edge>> &AdjList, bool verbose, int &s,
	      int numVertex, int numEdges)
{
  std::string filename = "dijkstra.time";
  vertex u, v;
  vertex * ptr;
  int size, w, q, x;
  if(s >= numVertex)
  {
    std::cerr << "\n**Error, argument greater than number of nodes"
	      << "\nSource reset to default of s = 1\n";
    s = 1;
  }
  std::vector<vertex> Graph(numVertex);
  std::deque<vertex*> Queue;

  //START TIME
  timeval start, end;
  double time;
  StartTimer(start);
  //START TIME

  Graph[s].dist = 0;
  Graph[s].name = s;
  for (int i = 1; i < numVertex; ++i)
  {
    Graph[i].name = i;
  }
  
  ptr = &Graph[s];
  Queue.push_back(ptr);

  while (!Queue.empty())
  {
  
    x = ExtractMin(Queue);
    u = Graph[x];
    size = AdjList[u.name].size();
    for(int i = 0; i < size; ++i)
    {
      v = Graph[AdjList[u.name][i].v];
      w = AdjList[u.name][i].w;
      if(v.dist > (u.dist + w))
      {
	v.dist = u.dist + w;
	v.pred = u.name;
	Graph[v.name] = v;
	ptr = &Graph[v.name];
	Queue.push_back(ptr);
	RestoreMinHeap(Queue);
      } 
    }
  }

  // END TIME
  EndTimer(start, end, time, (numVertex - 1), numEdges, filename);
  // END TIME

  if (numVertex > 100)
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
  int numEdges, numVertex;
  if (argc == 2)
  {
    filename = argv[1];
    std::cout << "\n\nWelcome to Dijkstra, source defaults to 1. Consult README to alter source.\n\n";
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
  Dijkstra(AdjList, verbose, source, numVertex, numEdges);
  return 0;
}

void PrintAnswer(std::vector<vertex> &Graph, int source)
{
  std::cout << "\nDijkstra complete. ";
  int size = Graph.size();
  if (size < 101)
  {
    std::cout << "Shortest path distances from s = " << source << ":"
            << "\n\nVertex\tShortest Path Distance";
    for (int i = 1; i < size; ++i)
    {
      std::cout << "\n  " << Graph[i].name << "\t";
      if(Graph[i].dist == TINY_INFINITY)
        std::cout << "infinity";
      else
        std::cout << Graph[i].dist;
    }
    std::cout << "\n\n";
  }
  else
  {
    std::cout << "\n\nRedirected to file, 'dijkstra.out,' infinity represented as 2147483647\n\n";
    std::ofstream fout;
    fout.open("dijkstra.out");
    if(!fout)
    {
	std::cerr << "\n\t **Error: file failed to open\n";
        exit (EXIT_FAILURE);
    }
    fout << "Shortest path distances from s = " << source << ":"
	 << "\n\nVertex\tShortest Path Distance";
    for (int i = 1; i < size; ++i)
    {
      fout << "\n  " << Graph[i].name << "\t" << Graph[i].dist;
    }
    fout.close();
  }
}

int ExtractMin(std::deque<vertex*> &Queue)
{
  int max = Queue[0]->name;
  Queue.pop_front();
  return max;
}

void RestoreMinHeap(std::deque<vertex*> &Queue)
{
  vertex * exchange;
  int index = Queue.size() - 1;
  int key = Queue[index]->dist;
  while (index > 0 && (Queue[Parent(index)] < Queue[index]))
  {
    exchange = Queue[index];
    Queue[index] = Queue[Parent(index)];
    Queue[Parent(index)] = exchange;
    index = Parent(index);
  }
}

int Parent(int index)
{
  return (index / 2);
}

void Dump(std::vector<vertex> &Graph)
{
  std::cout << "\n\nDump of Graph:\n";
  int size = Graph.size();
  for (int i = 1; i < size; ++i)
  {
    std::cout << "vertex: " << Graph[i].name
	      << "\tdistance: ";
    if(Graph[i].dist == 2147483600)
      std::cout << "infinity";
    else
      std::cout << Graph[i].dist << "     ";
    std::cout << "\tpredecessor: ";
    if(Graph[i].pred == -1)
      std::cout << "nil";
    else
      std::cout << Graph[i].pred;

    std::cout << "\n";
  }
}

/* for development

void Print(vertex &v)
{
  std::cout << "\nvertex: " << v.name
	    << "\tdistance: " << v.dist
	    << "\tpred: " << v.pred
	    << "\tinSet: " << v.inSet;
}
//*/
