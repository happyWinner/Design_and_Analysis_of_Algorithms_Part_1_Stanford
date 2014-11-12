/*
The goal of this problem is to implement the "Median Maintenance" algorithm (covered in the Week 5 lecture on heap applications).

The text file contains a list of the integers from 1 to 10000 in unsorted order; you should treat this as a stream of numbers, arriving one by one.
Letting Xi denote the i th number of the file, the k th median Mk is defined as the median of the numbers X1, ... , Xk.
(So, if k is odd, then Mk is ((k+1)/2) th smallest number among X1, ... , Xk; if k is even, then Mk is the (k/2) th smallest number among X1, ... , Xk.)

You should output the sum of these 10000 medians, modulo 10000 (i.e., only the last 4 digits). That is, you should compute (M1+ M2 + M3 + ... + M10000) mod 10000.

OPTIONAL EXERCISE: Compare the performance achieved by heap-based and search-tree-based implementations of the algorithm.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include "MaxHeap.h"
#include "MinHeap.h"

using namespace std;

int MedianMaintenance(MaxHeap<int>&, MinHeap<int>&, int);

int main()
{
	ifstream fin("Median.txt");
	if(!fin)
	{
		cerr << "Error: unable to open input file." << endl;
		return -1;
	}

	MaxHeap<int> heapLow;
	MinHeap<int> heapHigh;
	int sumOfMedian = 0;
	int elem;
	while(fin >> elem)
		sumOfMedian += MedianMaintenance(heapLow, heapHigh, elem);

	cout << sumOfMedian % 10000 << endl; 

	return 0;
}

int MedianMaintenance(MaxHeap<int>& heapLow, MinHeap<int>& heapHigh, int elem)
{
	if(heapLow.size() == heapHigh.size())
	{
		if(heapLow.size())
		{
			if(elem > heapHigh.get_min())
			{
				heapHigh.insert(elem);
				heapLow.insert(heapHigh.extract_min());
			}
			else
				heapLow.insert(elem);
		}
		else
			heapLow.insert(elem);
	}
	else
	{
		if(elem < heapLow.get_max())
		{
			heapLow.insert(elem);
			heapHigh.insert(heapLow.extract_max());
		}
		else
			heapHigh.insert(elem);
	}

	return heapLow.get_max();
}