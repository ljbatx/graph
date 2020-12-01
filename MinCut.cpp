/* 
MinCut.cpp
COT5405 Project
Laura Battle
November 2020

This file implements the Edmonds-Karp version of the Ford-Fulkerson
method (BFS) to find the minimum cut between a source and a sink in a
given network. It should work on weighted, directed graphs with no
self-loops or parallel edges. The hockey puck example flow network
on pg. 710 of CLRS is provided as "exflow.in" with 
Vancouver = 1, Edmondton = 2, Calgary = 3, Saskatoon = 4
Regina = 5, and Winnipeg = 6

This program has two modes, regular and verbose. Regular mode will
provide you with the minimum cut of the source and sink given by the
user. It should be entered like so:

MinCut.exe exflow.in 1 6

The source and the sink will default to 1 and 2 respectively if you
do not provide arguments or if your arguments are beyond the range
of the vertices in the provided file.

To run the program in verbose mode, provide a fifth argument like so:

MinCut.exe exflow.in 1 6 yes

And the program will output each residual graph it creates in the while
loop of the FordFulkerson() function.

**Please note verbose mode is disabled on graphs with over 100 nodes

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
};

//void Dump(std::vector<int> &Path); // for development
//void Dump(std::vector<vertex> &Graph);
void Dump(std::vector<std::vector<int>> &Residual);

void PrintAnswer(int answer, int source, int sink);
bool BFS(const std::vector<std::vector<int>> &Residual,
	 std::vector<int> &Path, int s, int t);

int FordFulkerson(const std::vector<std::vector<edge>> &AdjList, int s, int t, bool verbose,
		  int numVertex, int numEdges)
{
  std::string filename = "minCut.time";
  int maxFlow = 0;
  std::vector<int> Path(numVertex, 0);
  std::vector<int> Cleanse = Path;
  int size, v, flow, temp;
  std::vector<std::vector<int>> Residual(numVertex, Cleanse);

  //START TIME                                                                                       
  timeval start, end;
  double time;
  StartTimer(start);
  //START TIME                                                                                       


  for (int i = 1; i < numVertex; ++i)
  {  
    size = AdjList[i].size();
    for(int j = 0; j < size; ++j)
    {
      temp = AdjList[i][j].v;
      Residual[i][temp] = AdjList[i][j].w;
    }
  }
  while(BFS(Residual, Path, s, t))
  {
    flow = TINY_INFINITY;
    for(int u = t; u != s; u = Path[u])
    {
      v = Path[u];
      if(flow > Residual[v][u])
      {
	flow = Residual[v][u];
      }
    }
    for(int u = t; u != s; u = Path[u])
    {
      v = Path[u];
      Residual[u][v] = Residual[u][v] + flow;
      Residual[v][u] = Residual[v][u] - flow;
    }
    if(numVertex > 100)
      verbose = false;
    if (verbose)
      Dump(Residual);
    maxFlow = maxFlow + flow;
    Path = Cleanse;
  }

  // END TIME                                                                                        
  EndTimer(start, end, time, (numVertex - 1), numEdges, filename);
  // END TIME                                                                                        

  return maxFlow;
}

bool BFS(const std::vector<std::vector<int>> &Residual, 
          std::vector<int> &Path, int s, int t)
{
  int numVertex = Residual.size();
  bool pathExists = false;
  std::vector<vertex> Graph(numVertex);
  for (int i = 0; i < numVertex; ++i)
  {
    Graph[i].name = i;
  }
  Graph[s].color = 'g';
  int u;
  int n;
  int ver;
  std::queue<int> Queue;
  Queue.push(s);
  Path[s] = -1;
  while(!Queue.empty())
  {
    u = Queue.front();
    Queue.pop();
    n = Residual[u].size();
    for(int i = 1; i < n; ++i)
    {
      ver = Residual[u][i];
      if(ver > 0 && Graph[i].color == 'w')
      {
	Graph[i].color = 'g';
	Path[i] = u;
	Queue.push(i);
      }
    }
    Graph[u].color = 'b';
  }
  if(Graph[t].color == 'b')
    pathExists = true;
  return pathExists;
}

int main(int argc, char** argv)
{
  int answer;
  std::string filename;
  bool verbose = false;
  int numVertex, numEdges;
  int source = 1;
  int sink = 2;
  if (argc == 2)
  {
    filename = argv[1];
    std::cout << "\n\nWelcome to MinCut, source defaults to 1, sink to 2.\n"
	      << "Consult README to alter source and sink.\n\n";
  }
  else if (argc == 4)
  {
    filename = argv[1];
    sscanf(argv[2], "%d", &source);
    sscanf(argv[3], "%d", &sink);
  }
  else if (argc == 5)
  {
    filename = argv[1];
    sscanf(argv[2], "%d", &source);
    sscanf(argv[3], "%d", &sink);
    verbose = true;
  }
  else
  {
    std::cerr << "\nTo run this program you need one input file:\n"
	      << "eg:\n\n\t\tMinCut.exe default.in\n\n"
              << "\nOr you need an input file, a source, and a sink:\n"
              << "eg:\n\n\t\tMinCut.exe file.in 1 8";
    exit(EXIT_FAILURE);
  }

  std::vector<edge> Edges;
  std::vector<std::vector<edge>> AdjList;
  edge empty = {0, 0};
  Edges.push_back(empty);
  AdjList.push_back(Edges);
  DataStruc(filename, AdjList, Edges, empty, numVertex, numEdges);

  if(source >= numVertex || sink >= numVertex)
  {
    std::cerr << "\n**Error, argument greater than number of nodes"
	      << "\nSource reset to default of s = 1 and sink to t = 2\n";
    source = 1;
    sink = 2;
  }
  else if (source == sink)
  {
    std::cerr << "\n**Error, argument for source and sink are equal"
	      << "\nSource reset to default of s = 1 and sink to t = 2\n";
    source = 1;
    sink = 2;
  }

  answer = FordFulkerson(AdjList, source, sink, verbose, numVertex, numEdges);
  PrintAnswer(answer, source, sink);
  return 0;
}

void PrintAnswer(int answer, int source, int sink)
{
  if (answer > 0)
  {  
    std::cout << "\nFord-Fulkerson/Edmonds-Karp complete."
	      << "\nMinimum Cut with s = " << source
	      << " and t = " << sink << " is " << answer << "\n\n";
  }
  else
  {
    std::cout << "\nFord-Fulkerson/Edmonds-Karp complete. "
	      << "The source (" <<  source << ") "
	      << "does not flow to the sink (" << sink << ")\n\n";
  }
}
/*
void Dump(std::vector<vertex> &Graph)
{
  std::cout << "\n\nDump of Graph:\n";
  int size = Graph.size();
  for (int i = 1; i < size; ++i)
  {
    std::cout << "vertex: " << Graph[i].name
	      << "\tcolor: " << Graph[i].color << '\n';
  }
}
*/

void Dump(std::vector<std::vector<int>> &Residual)
{
  std::cout << "\n\nDump of Residual Graph:\n";
  int size = Residual.size();
  int size2;
  for (int i = 1; i < size; ++i)
  {
    size2 = Residual[i].size();
    for (int j = 1; j < size2; ++j)
    {
      if(Residual[i][j] > 0)
      {
        std::cout << "\nResidual[" << i << "][" << j
                << "] = " << Residual[i][j];
      }
    }
  }
  std::cout << '\n';
}

/*
void Dump(std::vector<int> &Path)
{
  std::cout << "\nPath Vector is: ";
  int n = Path.size();
  for(int i = 1; i < n; ++i)
  {
    std::cout << "\nPath[" << i << "] = " << Path[i];
  }
  std::cout << '\n';
}
*/
