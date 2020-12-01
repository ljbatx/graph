/*
DataStruc.cpp
COT 5405
Laura Battle
November 2020

This file takes the edge lists in the directory and puts them into
an Adjacency List data structure. An executable version of this
program resides in this directory under "AdjListDS.cpp".

This file also contains a few utility functions such as QuickSort
used by Undirected.cpp and Flow.cpp. It also contains a Dump()
function for the runtime timers used by all of the graph algorithms

I used code from a stackoverflow thread for the timers:
https://stackoverflow.com/questions/2125219/how-to-get-the-running-of-time-of-my-program-with-gettimeofday

The quicksort is adapted from a quicksort that I wrote for 
COP4531 in Spring 2020.
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <sys/time.h>
#include <bits/stdc++.h> // for std::setprecision()
#include <DataStruc.h>



void DataStruc(std::string filename, std::vector<std::vector<edge>> &AdjList,
	       std::vector<edge> &Edges, edge &empty,
	       int &numVertex, int &numEdges)
{
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
  numEdges = 0;
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
  //FindLastVertex function here
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
  int m = AdjList.size();
  for (int i = 0; i < m; ++i)
  {
    if(AdjList[i][0].v == test)
      AdjList[i].pop_back();
  }
  numVertex = AdjList.size();
  for (int i = 0; i < numVertex; ++i)
    numEdges = numEdges + AdjList[i].size();
  /*
  if(DFS)
  {
    int i, j, weight, node, list;
    AdjDFS.resize(numVertex);
    for(i = 1; i < numVertex; ++i)
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
  }
  */
}


void StartTimer(timeval &start)
{
  gettimeofday(&start, NULL);
  std::ios_base::sync_with_stdio(false);
}

void EndTimer(const timeval start, timeval end, double time, int nodes, int edges, std::string filename)                   
{                                                                                                    
  gettimeofday(&end, NULL);                                                                          
  time = (end.tv_sec - start.tv_sec) * 1e6;                                                          
  time = (time + (end.tv_usec - start.tv_usec)) * 1e-6;                                              
  std::ofstream runTime;                                                                             
  runTime.open(filename, std::ios::app); // append mode                                       
  if(!runTime)                                                                                       
    std::cout << "Error opening file, time was " << time;                                            
  else                                                                                               
  {                                                                                                  
    runTime << nodes << '\t' << edges << '\t';                                                       
    runTime << std::fixed << time << std::setprecision(6) << '\n';                                   
    runTime.close();                                                                                 
  }                                                                                                  
}


void SortMe(int numEdges, std::vector<edgeRead> &EdgeList)
{
  QuickSortS(EdgeList, 0, (numEdges - 1));
  int node, ceiling;
  for(int i = 0; i < numEdges; ++i)
    {
      ceiling = 0;
      node = EdgeList[i].s;
      for(int j = (i + 1); j < numEdges; ++j)
        {
          if(EdgeList[j].s == node)
            ++ceiling;
        }
      QuickSortF(EdgeList, i, (i + ceiling));
    }
}

void QuickSortF(std::vector<edgeRead> &EdgeList, int p, int r)
{
  int q;
  if (p < r)
    {
      q = PartitionF(EdgeList, p, r);
      QuickSortF(EdgeList, p, (q - 1));
      QuickSortF(EdgeList, (q + 1), r);
    }
}

void QuickSortS(std::vector<edgeRead> &EdgeList, int p, int r)
{
  int q;
  if (p < r)
    {
      q = PartitionS(EdgeList, p, r);
      QuickSortS(EdgeList, p, (q - 1));
      QuickSortS(EdgeList, (q + 1), r);
    }
}

int PartitionS(std::vector<edgeRead> &EdgeList, int p, int r)
{
  int q = EdgeList[r].s;
  int i = p - 1;
  for(int j = p; j <= (r - 1); ++j)
    {
      if(EdgeList[j].s < q)
        {
          ++i;
          Swap(EdgeList, i , j);
        }
    }
  ++i;
  Swap(EdgeList, i, r);
  return i;
}
int PartitionF(std::vector<edgeRead> &EdgeList, int p, int r)
{
  int q = EdgeList[r].f;
  int i = p - 1;
  for(int j = p; j <= (r - 1); ++j)
    {
      if(EdgeList[j].f < q)
        {
          ++i;
          Swap(EdgeList, i , j);
        }
    }
  ++i;
  Swap(EdgeList, i, r);
  return i;
}

void Swap(std::vector<edgeRead> &EdgeList, int i, int j)
{
  edgeRead swap;
  swap = EdgeList[j];
  EdgeList[j] = EdgeList[i];
  EdgeList[i] = swap;
}



