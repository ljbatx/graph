/*                                                                                                                 
Unirected.cpp                                                                                                       
Erdos-Renyi random undirected graph generator                                                                        
COT 5405 Project                                                                                                   
Laura Battle                                                                                                       
November 2020                                                                                                      
                                                                                                                   
The purpose of this file is to create edge lists of undirected graphs with positive edge weights                     
for use with graph algorithms.                                                                                     
                                                                                                                   
This graph generator will default to a p value of 0.1, 20 nodes, and a max weight of 100.                          
You have the option of running it with the default values like so:                                                 
                                                                                                                   
Undirected.exe                                                                                                       
                                                                                                                   
which will output the values in a file called "undirected.in" or you may alter the values. If                        
you want a p value of 0.05, 50 nodes, and a max weight of 300 you will need to provide a                           
filename for output and enter the command like so:                                                                 
                                                                                                                   
Undirected.exe 0.05 50 300 filename.in                                                                               
                                                                                                                   
And your results will be in filename.in located in the same directory.                                             
                                                                                                                   
This graph generator considers n! possible edges so there are no self-loops. Edge weights are
expected in listings, meaning that if there is a listing for:
1 5 40
meaning there is an edge from 1 to 5 with a weight of 40, then you can also expect to see
a listing like this:
5 1 40

As these insertions happen simulaneously, after the edge list is built, it is sorted
using quicksort to organize the edge list in ascending order both for the
starting vertex and finishing vertex. This quicksort is located in DataStruc.cpp.

This generates graphs appropriate to use with the executables Prims.exe and DFS.exe also
built with the makefile in this directory                    
                                                                                                                   
*/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <random>
#include <string>
#include <DataStruc.h>


int  GenerateGraph(std::vector<edgeRead> &EdgeList, float prob, int numNodes);
void AddWeight(std::vector<edgeRead> &EdgeList, int max, int numEdges);
void CreateFile(std::vector<edgeRead> &EdgeList, int numEdges, std::string filename);
void InputError();
void Mirror(std::vector<edgeRead> &EdgeList, int &numEdges);

int main(int argc, char** argv)
{
  // default values
  float probability = 0.1;
  int nodes = 20;
  int weight = 100;
  int numEdges = 0;
  std::string filename = "undirected.in";

  std::vector<edgeRead> EdgeList;

  if (argc == 1)
  {
    std::cout << "\n\nWelcome, creating edge list with default values, undirected.in" 
              << "\nPlease consult README.org for instructions on how to alter values.\n\n";
  }
  else if (argc == 5)
  {
    filename.clear();
    sscanf(argv[1], "%f", &probability);
    sscanf(argv[2], "%d", &nodes);
    sscanf(argv[3], "%d", &weight);
    filename = argv[4];
    if (probability <= ZERO || nodes <= ZERO || weight <= ZERO)
      InputError();
    if((probability >= ONE) || (nodes > MAXNODE) || (weight > MAXWEIGHT))
       InputError();
    std::cout << "\n\nWelcome, creating edge list with values:\n\t p = "
	      << probability << "\n\t nodes = " << nodes << "\n\t max weight = "
	      << weight << "\n\nFile will be placed in current directory as "
	      << filename << "\n\n";
  }
  else
    InputError();

  numEdges = GenerateGraph(EdgeList, probability, nodes);
  AddWeight(EdgeList, weight, numEdges);
  Mirror(EdgeList, numEdges);
  SortMe(numEdges, EdgeList);
  CreateFile(EdgeList, numEdges, filename);

  return 0;
}

int GenerateGraph(std::vector<edgeRead> &EdgeList, float prob, int numNodes)
{
  edgeRead enter;
  int numEdges = 0;
  std::default_random_engine generator;
  std::bernoulli_distribution distribution(prob);
  for (int start = 1; start <= numNodes; ++start)
  {
    for (int end = start + 1; end <= numNodes; ++end)
    {
           
      if(distribution(generator))
      {
	++numEdges;
	enter = {start, end, ZERO};
	EdgeList.push_back(enter);
      }
    }
  }

  return numEdges;
}

void Mirror(std::vector<edgeRead> &EdgeList, int &numEdges)
{
  edgeRead reverse;
  int s, f, w;
  for (int i = 0; i < numEdges; ++i)
  {
    s = EdgeList[i].f;
    f = EdgeList[i].s;
    w = EdgeList[i].w;
    reverse = {s, f, w};
    EdgeList.push_back(reverse);
  }
  numEdges = numEdges + numEdges;
}

void AddWeight(std::vector<edgeRead> &EdgeList, int max, int numEdges)
{
  srand(time(0));
  int floor = ONE;
  int ceiling = max;
  for(int i = 0; i < numEdges; ++i)
  {
    EdgeList[i].w = ((rand() % (ceiling - floor + 1)) + floor);
  }
}

void CreateFile(std::vector<edgeRead> &EdgeList, int numEdges, std::string filename)
{
  std::ofstream fout;
  fout.open(filename);
  if (!fout)
  {
    std::cerr << "\n\t***Error: file failed to open\n";
    exit (EXIT_FAILURE);
  }

  for (int i = 0; i < (numEdges - 1); ++i)
  {
    fout << EdgeList[i].s << " "
	 << EdgeList[i].f << " "
	 << EdgeList[i].w << "\n";
  }

  fout << EdgeList[numEdges - 1].s << " "
       << EdgeList[numEdges - 1].f << " "
       << EdgeList[numEdges - 1].w;

  fout.close();
}

void InputError()
{
  std::cerr << "\n\nInput error. Please consult README for information on how to\n"
            << "provide command line arguments. Alternatively, provide no\n"
	    << "arguments for default values.\n";
  exit(EXIT_FAILURE);
}
/*
void SortMe(int numEdges, std::vector<edge> &EdgeList)
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

void QuickSortF(std::vector<edge> &EdgeList, int p, int r)
{
  int q;
  if (p < r)
    {
      q = PartitionF(EdgeList, p, r);
      QuickSortF(EdgeList, p, (q - 1));
      QuickSortF(EdgeList, (q + 1), r);
    }
}

void QuickSortS(std::vector<edge> &EdgeList, int p, int r)
{
  int q;
  if (p < r)
    {
      q = PartitionS(EdgeList, p, r);
      QuickSortS(EdgeList, p, (q - 1));
      QuickSortS(EdgeList, (q + 1), r);
    }
}

int PartitionS(std::vector<edge> &EdgeList, int p, int r)
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

int PartitionF(std::vector<edge> &EdgeList, int p, int r)
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

void Swap(std::vector<edge> &EdgeList, int i, int j)
{
  edge swap;
  swap = EdgeList[j];
  EdgeList[j] = EdgeList[i];
  EdgeList[i] = swap;
}

*/
