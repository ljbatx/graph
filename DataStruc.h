/*
DataStruc.h
COT 5405 Project
Laura Battle
November 2020

This is a header file that accompanies DataStruc.cpp which is
used by all other files in this directory except AdjListDS.cpp

The DataStruc.cpp file contains more information about its 
contents
*/

#ifndef DATASTRUC_H
#define DATASTRUC_H

#include <cstdlib>
#include <vector>
#include <queue>
#include <string>
#include <sys/time.h>

#define MAXNODE   1000000
#define ZERO      0
#define MAXWEIGHT 100000
#define ONE       1
#define TINY_INFINITY 2147483600
#define NIL -1

struct edgeRead
{
  int s;   //starting vertex                                                                                       
  int f;   //ending vertex (finish)                                                                                
  int w;   //weight of vertex                                                                                      
};

struct edge
{
  int v; // destination vertex                                                          
  int w; // edge weight                                                                  
};

void SortMe(int numEdges, std::vector<edgeRead> &EdgeList);
void QuickSortF(std::vector<edgeRead> &EdgeList, int p, int r);
void QuickSortS(std::vector<edgeRead> &EdgeList, int p, int r);
int PartitionS(std::vector<edgeRead> &EdgeList, int p, int r);
int PartitionF(std::vector<edgeRead> &EdgeList, int p, int r);
void Swap(std::vector<edgeRead> &EdgeList, int i, int j);

void DataStruc(std::string filename, std::vector<std::vector<edge>> &AdjList,
	       std::vector<edge> &Edges, edge &empty, int &numVertex, int &numEdges);

void StartTimer(timeval &start);
void EndTimer(const timeval start, timeval end, double time, int nodes, int edges, std::string filename);
void Dump(std::queue<int> &Queue);  // for development


#endif

