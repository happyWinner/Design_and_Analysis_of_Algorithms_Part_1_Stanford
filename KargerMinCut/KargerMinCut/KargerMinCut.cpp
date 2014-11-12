/*
The file contains the adjacency list representation of a simple undirected graph. There are 200 vertices labeled 1 to 200.
The first column in the file represents the vertex label, and the particular row (other entries except the first column) tells all the vertices that the vertex is adjacent to.
So for example, the 6th row looks like : "6 155 56 52 120 ......". This just means that the vertex with label 6 is adjacent to (i.e., shares an edge with) the vertices with labels 155,56,52,120,......,etc

Your task is to code up and run the randomized contraction algorithm for the min cut problem and use it on the above graph to compute the min cut.
HINT: Note that you'll have to figure out an implementation of edge contractions. Initially, you might want to do this naively, creating a new graph from the old every time there's an edge contraction. But you should also think about more efficient implementations.
WARNING: As per the video lectures, please make sure to run the algorithm many times with different random seeds, and remember the smallest cut that you ever find.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <ctime>

using namespace std;

#define SIZE 200
#define TRIAL_NUMBER SIZE * SIZE

struct VERTEX;
struct EDGE;

int KargerMinCut(vector<VERTEX>&);

struct VERTEX
{
	int label;
	EDGE* edge;
};

struct EDGE
{
	VERTEX* vertex;
	EDGE* next;
};

int main()
{
	srand((int)time(0));
	int minCuts = -1;
	for(int i = 1; i <= TRIAL_NUMBER; i++)
	{
		ifstream fin("kargerMinCut.txt");
		if(!fin)
		{
			cerr << "Error: unable to open input file." << endl;
			return -1;
		}
		
		vector<VERTEX> vertices(SIZE);
		while(!fin.eof())
		{
			string line;
			getline(fin, line);
			stringstream str(line);
			int label, adjLabel;
			str >> label;
			VERTEX* vertex = &(vertices.at(label - 1));
			vertex->label = label;
			EDGE head;
			EDGE* node = &head;
			while(str >> adjLabel)
			{
				EDGE* edge = new EDGE;
				edge->vertex = &(vertices.at(adjLabel - 1));
				edge->next = NULL;
				node->next = edge;
				node = node->next;
			}
			vertex->edge = head.next;
		}
		fin.close();
		
		int cut = KargerMinCut(vertices);
		if(cut < minCuts || minCuts < 0)
			minCuts = cut;

		vertices.~vector();

		if(!(i % 100))
			cout << "Minimum Cuts after " << i << " Trials: " << minCuts << endl;
	}

	return 0;
}

int KargerMinCut(vector<VERTEX>& vertices)
{
	vector<int> indexes;
	for(int i = 0; i < SIZE; ++i)
		indexes.push_back(i);
	
	// random contraction
	while(indexes.size() > 2)
	{
		// pick an edge
		int selectedIdx = rand() % indexes.size();
		int vertexIdx = indexes.at(selectedIdx);
		VERTEX* vertex1 = &(vertices.at(vertexIdx));
		EDGE* node = vertex1->edge;
		int random = rand() & indexes.size();
		for(int i = 1; i < random; i++)
		{
			node = node->next;
			if(!node)
				node = vertex1->edge;
		}
		VERTEX* vertex2 = node->vertex;		

		// remove self-loops in vertex1 & modify related edges
		EDGE head;
		head.next = vertex1->edge;
		vertex1->edge = &head;
		EDGE* previous;
		for(previous = &head, node = head.next; node; node = node->next)
		{
			if(node->vertex == vertex2)
				previous->next = node->next;
			else
			{
				bool bFound = false;
				for(EDGE* temp = node->vertex->edge; temp && !bFound; temp = temp->next)
				{
					if(temp->vertex == vertex1)
					{
						temp->vertex = vertex2;
						bFound = true;
					}
				}
				previous = previous->next;
			}
		}
		vertex1->edge = head.next;

		// remove self-loops in vertex2
		head.next = vertex2->edge;
		vertex2->edge = &head;
		for(previous = &head, node = head.next; node; node = node->next)
		{
			if(node->vertex == vertex1)
				previous->next = node->next;
			else
				previous = previous->next;
		}
		vertex2->edge = head.next;

		// merge two vertices into a single vertex
		if(vertex2->edge)
			previous->next = vertex1->edge;
		else
			vertex2->edge = vertex1->edge;
		vertex1->label = 0;
		vertex1->edge = NULL;
		indexes.erase(indexes.begin() + selectedIdx);
	}

	// calculate the minimum cuts
	int minCuts = 0;
	for(EDGE* edge = vertices.at(indexes.at(0)).edge; edge; edge = edge->next)
		++minCuts;

	return minCuts;
}