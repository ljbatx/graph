/* 
Prims.cpp
COT5405 Project
Laura Battle
November 2020

This file implements minimum spanning tree of an undirected weighted graph using
Prim's algorithm. The file provided "testprim.in" contains the edge list 
for the graph on page 635 of CLRS. (Letter nodes are represented with a = 1,
b = 2, etc.)

This program has two modes, regular and verbose. Regular mode will provide a
minimum spanning tree representation. With or without a user given node.
For example for a default root of r = 1, you can run the program like so:

Prims.exe testprim.in

To run it with a different node:

Prims.exe testprim.in 3

And lastly, to run the program in verbose mode, you simply add a 4th argument
and the program will output each node's predecessor, key and tree level
in a list format. The command for that would look like this:

Prims.exe testprim.in 3 yes

If you enter a root value that isn't contained in the file given, the root
will default back to 1.

If you enter in a file that has over 100 nodes, the output will be put into
a file called "prims.out."

**Please note verbose mode is disabled on files with over 100 nodes

Lastly, figuring out how to print the m-ary tree was difficult. I used
the following website to help me print out the tree to the console:
https://www.geeksforgeeks.org/print-n-ary-tree-graphically/

*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <deque>
#include <string>
#include <sys/time.h>
#include <DataStruc.h>

struct vertexPrim
{
  int name; 
  int pred = NIL; // -1
  int key = TINY_INFINITY; // 2.147 billion
  int level = NIL; // -1
  bool inQ = true;
};

void Dump(std::vector<vertexPrim> &Graph);
void PrintAnswer(std::vector<vertexPrim> &Graph, int r, int max);
int  GetMin(std::vector<vertexPrim> &Graph, int numVertex);
void RestoreMinHeap(std::deque<vertexPrim*> &Queue);
int ExtractMin(std::deque<vertexPrim*> &Queue);
int Parent(int index);

void Prims(const std::vector<std::vector<edge>> &AdjList, bool verbose, int &r,
	   int numVertex, int numEdges)
{
  std::string filename = "prims.time";
  int size, w, max, q;
  vertexPrim* ptr;
  vertexPrim u, v;
  max = 0;
  if(r >= numVertex)
  {
    std::cerr << "\n**Error, argument greater than number of nodes"
	      << "\nRoot reset to default of r = 1\n";
    r = 1;
  }
  std::vector<vertexPrim> Graph(numVertex);
  std::deque<vertexPrim*> Queue;  

  //START TIME                                                                                       
  timeval start, end;
  double time;
  StartTimer(start);
  //START TIME                                                                                       

  Graph[r].key = 0;
  Graph[r].level = 0;

  for (int i = 1; i < numVertex; ++i)
  {
    Graph[i].name = i;
  }

  ptr = &Graph[r];
  Queue.push_back(ptr);

  while(!Queue.empty())
  {
    q = ExtractMin(Queue);
    Graph[q].inQ = false;
    u = Graph[q];
    size = AdjList[u.name].size();
    for (int i = 0; i < size; ++i)
    {
      v = Graph[AdjList[u.name][i].v];
      w = AdjList[u.name][i].w;
      if((w < v.key) && v.inQ)
      {
	v.pred = u.name;
	v.key = w;
	v.level = (u.level + 1);
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
                                                                                      
  for (int i = 0; i < numVertex; ++i)
    if(Graph[i].level > max)
      max = Graph[i].level;

  if (numVertex > 100)
    verbose = false;
  if(verbose)
  {
    Dump(Graph);
  }

  PrintAnswer(Graph, r, max);
}

int main(int argc, char** argv)
{
  std::string filename;
  bool verbose = false;
  int numVertex, numEdges;
  int root = 1;
  if (argc == 2)
  {
    filename = argv[1];
    std::cout << "\n\nWelcome to Prim's, root defaults to 1. Consult README to alter root.\n\n";
  }
  else if (argc == 3)
  {
    filename = argv[1];
    sscanf(argv[2], "%d", &root);
  }
  else if (argc == 4)
    {
      filename = argv[1];
      sscanf(argv[2], "%d", &root);
      verbose = true;
    }
  else
  {
    std::cerr << "\nTo run this program you need one input file:\n"
	      << "eg:\n\n\t\t Prims.exe default.in\n\n";
    exit(EXIT_FAILURE);
  }

  std::vector<edge> Edges;
  std::vector<std::vector<edge>> AdjList;
  edge empty = {0, 0};
  Edges.push_back(empty);
  AdjList.push_back(Edges);

  DataStruc(filename, AdjList, Edges, empty, numVertex, numEdges); 

  Prims(AdjList, verbose, root, numVertex, numEdges);
  return 0;
}

void Trace(std::vector<vertexPrim> &Graph, int prev, int size)
{
  for(int i = 0; i < size; ++i)
  {
    if(Graph[i].pred == prev)
    {
      std::cout << '\n';
      for(int j = 0; j < Graph[i].level; ++j)
      {
	std::cout << "--";
      }
      std::cout << Graph[i].name << " (" << Graph[i].key << ")";
      Trace(Graph, Graph[i].name, size);
    }
  }
}

void Trace(std::vector<vertexPrim> &Graph, int prev, int size,
           std::ofstream &fout)
{
  for(int i = 0; i < size; ++i)
    {
      if(Graph[i].pred == prev)
	{
	  fout << '\n';
	  for(int j = 0; j < Graph[i].level; ++j)
	  {
	    fout << "--";
	  }
	  fout << Graph[i].name << " (" << Graph[i].key << ")";
	  Trace(Graph, Graph[i].name, size, fout);
	}
    }
}

int ExtractMin(std::deque<vertexPrim*> &Queue)
{
  int max = Queue[0]->name;
  Queue.pop_front();
  return max;
}

void RestoreMinHeap(std::deque<vertexPrim*> &Queue)
{
  vertexPrim * exchange;
  int index = Queue.size() - 1;
  int key = Queue[index]->key;
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

void PrintAnswer(std::vector<vertexPrim> &Graph, int root, int max)
{
  std::cout << "\nPrim's complete. Minimum Spanning Tree with root, r = " << root << ":\n";
  int size = Graph.size();
  if (size > 101)
  {
    std::cout << "\n\n\tRedirected to file 'prims.out'\n\n";
    std::ofstream fout;
    fout.open("prims.out");
    fout << root;
    Trace(Graph, root, size, fout);
  }
  else
  {
    std::cout << '\n' << root;
    Trace(Graph,root, size);
    std::cout << "\n\n";
  }
}

void Dump(std::vector<vertexPrim> &Graph)
{
  std::cout << "\n\nDump of Graph:\n";
  int size = Graph.size();
  for (int i = 1; i < size; ++i)
  {
    std::cout << "vertex: " << Graph[i].name;
    std::cout << "\tpredecessor: ";
    if(Graph[i].pred == NIL)
      std::cout << "nil";
    else
      std::cout << Graph[i].pred << "    ";
    std::cout << "\tkey:  ";
    if(Graph[i].key == TINY_INFINITY)
      std::cout << "infinity";
    else
      std::cout << Graph[i].key << "    ";
    std::cout << "\tlevel: ";
    if(Graph[i].level == NIL)
      std::cout << "not connected to root";
    else
      std::cout << Graph[i].level;
    std::cout << '\n';
  }
}

int GetMin(std::vector<vertexPrim> &Graph, int numVertex)
{
  int min = Graph[0].key;
  int index = 0;
  for (int i = 1; i < numVertex; ++i)
  {
    if (Graph[i].inQ && Graph[i].key < min)
    {
      min = Graph[i].key;
      index = i;
    }
  }
  return index;
}
