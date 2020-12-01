/*                                                                                                                 
Flow.cpp                                                                                                       
Erdos-Renyi random directed graph generator - no self loops or reverse edges                               
COT 5405 Project                                                                                                   
Laura Battle                                                                                                       
November 2020                                                                                                      
                                                                                                                   
The purpose of this file is to create edge lists of directed graphs with positive edge weights 
and no reverse edges or self-loops for use with graph algorithms.                  
                                                                                                                   
This graph generator will default to a p value of 0.1, 20 nodes, and a max weight of 100.                          
You have the option of running it with the default values like so:                                                 
                                                                                                
Flow.exe                                                                                                       
                                                                                                                   
which will output the values in a file called "flow.in" or you may alter the values. If                        
you want a p value of 0.05, 50 nodes, and a max weight of 300 you will need to provide a                           
filename for output and enter the command like so:                                                                 
                                                                                                                   
Flow.exe 0.05 50 300 filename.in                                                                               
                                                                                                                   
And your results will be in filename.in located in the same directory.                                             
                                                                                                                   
This graph generator considers n! possible edges, so no self loops are possible. If the
distribution generator decides at iteration 1, 5 to insert an edge, then there is a 
second generator that decides with a 50/50 probability in which direction the edge will
be inserted. Weight is later assigned. This means that this list is also out of order
and QuickSort is used at the end to sort the edge list by both first and second vertex.
The QuickSort algorithm is located in the DataStruc.cpp file.
                                                                                                                   
This generates graphs appropriate to use with the executable MinCut.exe also                        
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
void Chance(int &start, int &end, std::default_random_engine &generator);


int main(int argc, char** argv)
{
  // default values
  float probability = 0.1;
  int nodes = 20;
  int weight = 1000;
  int numEdges = 0;
  std::string filename = "flow.in";
  std::vector<edgeRead> EdgeList;

  if (argc == 1)
  {
    std::cout << "\n\nWelcome, creating edge list with default values for a flow graph, flow.in" 
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
  SortMe(numEdges, EdgeList);
  CreateFile(EdgeList, numEdges, filename);

  return 0;
}

int GenerateGraph(std::vector<edgeRead> &EdgeList, float prob, int numNodes)
{
  edgeRead enter;
  int numEdges = 0;
  int s, e;
  std::default_random_engine generator;
  std::bernoulli_distribution distribution(prob);
  for (int start = 1; start <= numNodes; ++start)
  {
    for (int end = start + 1; end <= numNodes; ++end)
    {
      if(distribution(generator))
      {
	s = start;
	e = end;
        Chance(s, e, generator); 
        ++numEdges;
	enter = {s, e, ZERO};
	EdgeList.push_back(enter);
      }
    }
  }
  return numEdges;
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

  for (int i = 1; i < (numEdges - 1); ++i)
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

void Chance(int &start, int &end, std::default_random_engine &generator)
{
  int swap;
  float equalizer = 0.5;
  std::bernoulli_distribution equalize(equalizer);
  if(equalize(generator))
  {
    swap = end;
    end = start;
    start = swap;
  }
}


