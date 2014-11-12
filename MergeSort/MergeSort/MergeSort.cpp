/*
This file contains all of the 100,000 integers between 1 and 100,000 (inclusive) in some order, with no integer repeated.
Your task is to compute the number of inversions in the file given, where the i th row of the file indicates the i th entry of an array.
Because of the large size of this array, you should implement the fast divide-and-conquer algorithm covered in the video lectures.
*/

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

unsigned long SortAndCount(vector<int>&, vector<int>&, int, int);
unsigned long MergeAndCountSplitInv(vector<int>&, vector<int>&, int, int);

int main()
{
	ifstream fin("IntegerArray.txt");
	if(!fin)
	{
		cerr << "Error: unable to open input file." << endl;
		return -1;
	}

	vector<int> originalNumbers;
	int temp;
	while(fin >> temp)
		originalNumbers.push_back(temp);
	int length = originalNumbers.size();
	vector<int> sortedNumbers(length);
	fin.close();

	cout << "number of inversions: " << SortAndCount(originalNumbers, sortedNumbers, 0, length - 1) << endl;
	
	return 0;
}

unsigned long SortAndCount(vector<int>& originalNumbers, vector<int>& sortedNumbers, int start, int end)
{
	if(start == end)
		return 0;
	
	unsigned long leftInv = SortAndCount(originalNumbers, sortedNumbers, start, (start + end) / 2);
	unsigned long rightInv = SortAndCount(originalNumbers, sortedNumbers, (start + end) / 2 + 1, end);
	unsigned long splitInv = MergeAndCountSplitInv(originalNumbers, sortedNumbers, start, end);

	return leftInv + rightInv + splitInv;
}

unsigned long MergeAndCountSplitInv(vector<int>& originalNumbers, vector<int>& sortedNumbers, int start, int end)
{
	int middle = (start + end) / 2;
	int leftIndex = start;
	int rightIndex = middle + 1;
	unsigned long splitInv = 0;

	int sortedIndex;
	for(sortedIndex = start; sortedIndex <= end && leftIndex <= middle && rightIndex <= end; sortedIndex++)
	{
		if(originalNumbers.at(leftIndex) <= originalNumbers.at(rightIndex))
		{
			sortedNumbers.at(sortedIndex) = originalNumbers.at(leftIndex);
			leftIndex++;
		}
		else
		{
			sortedNumbers.at(sortedIndex) = originalNumbers.at(rightIndex);
			rightIndex++;
			splitInv += middle - leftIndex + 1;
		}
	}

	while(leftIndex <= middle)
	{
		sortedNumbers.at(sortedIndex) = originalNumbers.at(leftIndex);
		leftIndex++;
		sortedIndex++;
	}

	while(rightIndex <= end)
	{
		sortedNumbers.at(sortedIndex) = originalNumbers.at(rightIndex);
		rightIndex++;
		sortedIndex++;
	}

	for(sortedIndex = start; sortedIndex <= end; sortedIndex++)
		originalNumbers.at(sortedIndex) = sortedNumbers.at(sortedIndex);

	return splitInv;
}