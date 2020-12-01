/*
AdjListDS.cpp
COT5405 Project
Laura Battle
November 2020

This file is self-contained from all others. It is an executable version of the DataStruc.cpp
file that is used by the sorting algorithms. It is for debugging purposes, and for viewing
what the adjacency list does. It has additional functionality. It will also display the
transpose of a graph. (This functionality was built to do strongly connected components for
directed graphs with DFS, but I ultimately just made an ER Generator for Undirected Graphs
instead. I commented out this functionality in the DataStruc.cpp but haven't deleted it in
case I decide to work with this further in the future.

In order to run this program type the name of the executable and the file you want to put
into the Adjacency List Data Structure like so:

AdjListDS.exe file.in

If you would like to see the transpose of the graph add a third argument like so:

AdjListDS.exe file.in yes

When the adjacency list is displayed it will look like this:

1 -> 2 (16) -> 3 (13)
2 -> 4 (12)
3 -> 2 (4) -> 5 (14)
4 -> 3 (9) -> 6 (20)
5 -> 4 (7) -> 6 (4)
6

This is interpreted as vertex 1 points to vertices 2 and 3 with edge weights 16 and 13
respectively, etc.


*/
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>

 struct edge
 {
   int v; // destination vertex                    
   int w; // edge weight                                       
 };

void Dump(std::vector<std::vector<edge>> &AdjList);
void FindLastVertex(std::vector<std::vector<edge>> &AdjList, std::vector<edge> Edges);

int main(int argc, char** argv)
{
  std::vector<edge> Edges;
  std::vector<std::vector<edge>> AdjList;
  edge empty = {0, 0};
  Edges.push_back(empty);
  AdjList.push_back(Edges);
  bool DFS = false;

  std::string filename;
  if (argc == 2)
  {
    filename = argv[1];
  }
  else if (argc == 1)
  {
    filename = "default.in";
  }
  else if (argc == 3)
  {
    filename = argv[1];
    DFS = true;
  }

  else
  {
    std::cerr << "\n\n**Error, too many command line arguments\n\n";
    exit(EXIT_FAILURE);
  }
  std::ifstream fin;
  fin.open(filename);
  if(!fin)
  {
    std::cerr << "\n\n**Error opening " << filename << "\n\n";
    exit(EXIT_FAILURE);
  }
  int curVertex = 0;
  int start;
  int finish;
  int weight;
  edge add;
  do
  {
    fin >> start;
    fin >> finish;
    fin >> weight;
    add = {finish, weight};
    if (start == curVertex)
    {
      AdjList[curVertex].push_back(add);
    }
    else if (start == curVertex + 1)
    {
      curVertex = start;
      AdjList.push_back(Edges);
      AdjList[curVertex][0].v = finish;
      AdjList[curVertex][0].w = weight;
    }
    else
    {
      while(start != curVertex)
      {
	++curVertex;
	AdjList.push_back(Edges);
      }
      AdjList.push_back(Edges);
      AdjList[curVertex][0].v = finish;
      AdjList[curVertex][0].w = weight;
    }

  }while (!fin.eof());

  fin.close();

  int n = AdjList.size() - 1;
  int test = 0;
  int lastEdge = AdjList[n][0].v;
  while(test == lastEdge)
  {
    AdjList[n].pop_back();
    --n;
    AdjList.pop_back();
    lastEdge = AdjList[n][0].v;
  }
  FindLastVertex(AdjList, Edges);
  int m = AdjList.size();
  for (int i = 0; i < m; ++i)
  {
    if(AdjList[i][0].v == test)
      AdjList[i].pop_back();
  }

  Dump(AdjList);
  if(DFS)
  {
    int i, j, weight, node, list;
    int size = AdjList.size();
    std::vector<std::vector<edge>> AdjDFS(size);
    for(i = 1; i < size; ++i)
    {
      list = AdjList[i].size();
      for(j = 0; j < list; ++j)
      {
	node = AdjList[i][j].v;
	weight = AdjList[i][j].w;
	add = {i, weight};
	AdjDFS[node].push_back(add);
      }
    }
    std::cout << "\tTranspose of Graph is below.";
    Dump(AdjDFS);
  }

  return 0;
}

void Dump(std::vector<std::vector<edge>> &AdjList)
{
  std::cout << "\n\nAdjacency List:";
  int n = AdjList.size();
  int m;
  for(int i = 1; i < n; ++i)
  {
    m = AdjList[i].size();
    std::cout << "\n" << i;
    for(int j = 0; j < m; ++j)
    {
      std::cout << " -> " << AdjList[i][j].v << " ("
                << AdjList[i][j].w << ")";
    }
  }
  std::cout << "\n\nend of list\n\n";
}

void FindLastVertex(std::vector<std::vector<edge>> &AdjList, std::vector<edge> Edges)
{
  int outer = AdjList.size() - 1;
  int max = outer;
  int inner;
  for(int i = 0; i <= outer; ++i)
  {
    inner = AdjList[i].size();
    for (int j = 0; j < inner; ++j)
    {
      if(max < AdjList[i][j].v)
	max = AdjList[i][j].v;
    }
  }
  while (max > outer)
  {
    ++outer;
    AdjList.push_back(Edges);
  }
}
