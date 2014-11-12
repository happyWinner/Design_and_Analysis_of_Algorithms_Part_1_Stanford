/*
The file contains an adjacency list representation of an undirected weighted graph with 200 vertices labeled 1 to 200. Each row consists of the node tuples that are adjacent to that particular vertex along with the length of that edge.
For example, the 6th row has 6 as the first entry indicating that this row corresponds to the vertex labeled 6. The next entry of this row "141,8200" indicates that there is an edge between vertex 6 and vertex 141 that has length 8200. The rest of the pairs of this row indicate the other vertices adjacent to vertex 6 and the lengths of the corresponding edges.

Your task is to run Dijkstra's shortest-path algorithm on this graph, using 1 (the first vertex) as the source vertex, and to compute the shortest-path distances between 1 and every other vertex of the graph.
If there is no path between a vertex v and vertex 1, we'll define the shortest-path distance between 1 and v to be 1000000.

You should report the shortest-path distances to the following ten vertices, in order: 7,37,59,82,99,115,133,165,188,197. You should encode the distances as a comma-separated string of integers.
So if you find that all ten of these vertices except 115 are at distance 1000 away from vertex 1 and 115 is 2000 distance away, then your answer should be 1000,1000,1000,1000,1000,2000,1000,1000,1000,1000.

IMPLEMENTATION NOTES: This graph is small enough that the straightforward O(mn) time implementation of Dijkstra's algorithm should work fine.
OPTIONAL: For those of you seeking an additional challenge, try implementing the heap-based version. Note this requires a heap that supports deletions, and you'll probably need to maintain some kind of mapping between vertices and their positions in the heap.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "MinHeap.h"

using namespace std;

#define SIZE 200
#define INFINITY 1000000

int outputLabels[] = {7, 37, 59, 82, 99, 115, 133, 165, 188, 197};

struct VERTEX;
struct EDGE;

void DijkstraShortestPath(vector<VERTEX>&, int);

struct VERTEX
{
	int label;
	long minDistance;
	EDGE* edge;
};

struct EDGE
{
	long distance;
	VERTEX* vertex;
	EDGE* next;
};

struct HeapElem
{
	int label;
	long distance;

	HeapElem(int newLabel, long newDistance) : label(newLabel), distance(newDistance) {};

	bool operator < (HeapElem other)
	{
		return distance < other.distance;
	}

	bool operator <= (HeapElem other)
	{
		return distance <= other.distance;
	}

	bool operator > (HeapElem other)
	{
		return distance > other.distance;
	}

	bool operator >= (HeapElem other)
	{
		return distance >= other.distance;
	}
};

int main()
{
	ifstream fin("dijkstraData.txt");
	if(!fin)
	{
		cerr << "Error: unable to open input file." << endl;
		return -1;
	}

	vector<VERTEX> graph(SIZE);
	while(fin.peek() != EOF)
	{
		string line;
		getline(fin, line);
		stringstream str(line);

		int label;
		str >> label;
		graph.at(label - 1).label = label;
		graph.at(label - 1).minDistance = INFINITY;
		graph.at(label - 1).edge = NULL;

		int adjLabel;
		char comma;
		long distance;
		while(str >> adjLabel >> comma >> distance)
		{
			EDGE* edge = new EDGE;
			edge->distance = distance;
			edge->vertex = &(graph.at(adjLabel - 1));
			edge->next = graph.at(label - 1).edge;
			graph.at(label - 1).edge = edge;
		}
	}
	fin.close();

	DijkstraShortestPath(graph, 1);

	for(int i = 0; i < 10; ++i)
	{
		cout << graph.at(outputLabels[i] - 1).minDistance;
		if(i != 9)
			cout << ",";
		else
			cout << endl;
	}

	return 0;
}

void DijkstraShortestPath(vector<VERTEX>& graph, int srcLabel)
{
	graph.at(srcLabel - 1).minDistance = 0;
	MinHeap<HeapElem> heap;
	for(EDGE* node = graph.at(srcLabel - 1).edge; node; node = node->next)
	{
		HeapElem* elem = new HeapElem(node->vertex->label, node->distance);
		heap.insert(*elem);
	}

	while(heap.size())
	{
		HeapElem minElem = heap.extract_min();
		if(graph.at(minElem.label - 1).minDistance == INFINITY)
		{
			graph.at(minElem.label - 1).minDistance = minElem.distance;
			for(EDGE* node = graph.at(minElem.label - 1).edge; node; node = node->next)
			{
				HeapElem* elem = new HeapElem(node->vertex->label, node->distance + minElem.distance);
				heap.insert(*elem);
			}
		}
	}

	return;
}