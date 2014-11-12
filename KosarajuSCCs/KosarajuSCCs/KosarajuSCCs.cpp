/*
The file contains the edges of a directed graph. Vertices are labeled as positive integers from 1 to 875714.
Every row indicates an edge, the vertex label in first column is the tail and the vertex label in second column is the head (recall the graph is directed, and the edges are directed from the first column vertex to the second column vertex).
So for example, the 11th row looks liks : "2 47646". This just means that the vertex with label 2 has an outgoing edge to the vertex with label 47646

Your task is to code up the algorithm from the video lectures for computing strongly connected components (SCCs), and to run this algorithm on the given graph.

Output Format: You should output the sizes of the 5 largest SCCs in the given graph, in decreasing order of sizes, separated by commas (avoid any spaces).
So if your algorithm computes the sizes of the five largest SCCs to be 500, 400, 300, 200 and 100, then your answer should be "500,400,300,200,100".
If your algorithm finds less than 5 SCCs, then write 0 for the remaining terms. Thus, if your algorithm computes only 3 SCCs whose sizes are 400, 300, and 100, then your answer should be "400,300,100,0,0".
*/

#include <iostream>
#include <fstream>
#include <vector>

// change the stack reserve size
#pragma comment(linker, "/STACK:102400000")

using namespace std;

#define SIZE 875714

struct VERTEX;
struct EDGE;

void DFS(vector<VERTEX>&, vector<int>&, int, bool);

struct VERTEX
{
	int label;
	bool explored;
	EDGE* edge;
};

struct EDGE
{
	VERTEX* vertex;
	EDGE* next;
};

int main()
{
	ifstream fin("SCC.txt");
	if(!fin)
	{
		cerr << "Error: unable to open input file." << endl;
		return -1;
	}

	vector<VERTEX> graph(SIZE), graphReversed(SIZE);
	for(int i = 1; i <= SIZE; ++i)
	{
		graph.at(i - 1).label = i;
		graph.at(i - 1).explored = false;
		graph.at(i - 1).edge = NULL;

		graphReversed.at(i - 1).label = i;
		graphReversed.at(i - 1).explored = false;
		graphReversed.at(i - 1).edge = NULL;
	}

	int column1, column2;
	while(fin >> column1)
	{
		fin >> column2;

		EDGE* edge = new EDGE;
		edge->vertex = &graph.at(column2 - 1);
		edge->next = graph.at(column1 - 1).edge;
		graph.at(column1 - 1).edge = edge;

		EDGE* edgeReversed = new EDGE;
		edgeReversed->vertex = &graphReversed.at(column1 - 1);
		edgeReversed->next = graphReversed.at(column2 - 1).edge;
		graphReversed.at(column2 - 1).edge = edgeReversed;
	}

	vector<int> graphIndex;
	for(int i = SIZE - 1; i >= 0; --i)
	{
		if(!graphReversed.at(i).explored)
			DFS(graphReversed, graphIndex, i, true);
	}

	vector<int> SCCSize(6, 0);
	while(graphIndex.size())
	{
		int index = graphIndex.back();
		if(!graph.at(index).explored)
		{
			DFS(graph, SCCSize, index, false);
			for(int i = 0; i < 5; ++i)
			{
				if(SCCSize.at(i) < SCCSize.at(5))
				{
					SCCSize.insert(SCCSize.begin() + i, SCCSize.at(5));
					SCCSize.pop_back();
				}
			}
			SCCSize.at(5) = 0;
		}
		graphIndex.pop_back();
	}

	cout << "The Sizes of the 5 Largest SCCs: " << endl;
	for(int i = 0; i < 5; ++i)
	{
		cout << SCCSize.at(i);
		if(i < 4)
			cout << ",";
		else
			cout << endl;
	}

	return 0;
}

void DFS(vector<VERTEX>& graph, vector<int>& result, int index, bool bReversed)
{
	graph.at(index).explored = true;
	for(EDGE* node = graph.at(index).edge; node; node = node->next)
	{
		if(!node->vertex->explored)
			DFS(graph, result, node->vertex->label - 1, bReversed);
	}

	if(bReversed)
		result.push_back(index);
	else
		++result.at(result.size() - 1);
}