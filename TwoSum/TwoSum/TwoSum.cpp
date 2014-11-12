/*
The goal of this problem is to implement a variant of the 2-SUM algorithm (covered in the Week 6 lecture on hash table applications).

The file contains 1 million integers, both positive and negative (there might be some repetitions!). This is your array of integers, with the i th row of the file specifying the i th entry of the array.

Your task is to compute the number of target values t in the interval [-10000,10000] (inclusive) such that there are distinct numbers x, y in the input file that satisfy x+y=t. (NOTE: ensuring distinctness requires a one-line addition to the algorithm from lecture.)

OPTIONAL CHALLENGE: If this problem is too easy for you, try implementing your own hash table for it. For example, you could compare performance under the chaining and open addressing approaches to resolving collisions.
*/

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#define LOWER_BOUND -10000
#define UPPER_BOUND 10000
#define OFFSET 2500000
#define SIZE 5000000

long long HashFunc(long long);
int TwoSum(vector<vector<long long>>&);

int main()
{
	ifstream fin("algo1_programming_prob_2sum.txt");
	if(!fin)
	{
		cerr << "Error: unable to open input file." << endl;
		return -1;
	}

	vector<vector<long long>> hash(SIZE);
	long long elem;
	while(fin >> elem)
		hash.at(HashFunc(elem)).push_back(elem);
	
	cout << TwoSum(hash) << endl;

	return 0;
}

long long HashFunc(long long elem)
{
	return (elem / 40001 + OFFSET);
}

int TwoSum(vector<vector<long long>>& hash)
{
	vector<bool> sums(UPPER_BOUND - LOWER_BOUND + 1, false);
	for(long long i = 0; i < hash.size(); ++i)
		for(long long j = 0; j < hash.at(i).size(); ++j)
		{
			long long lowerBoundIdx = HashFunc(LOWER_BOUND - hash.at(i).at(j));
			long long UpperBoundIdx = HashFunc(UPPER_BOUND - hash.at(i).at(j));
			for(long long k = lowerBoundIdx; k <= UpperBoundIdx; ++k)
				for(long long l = 0; l < hash.at(k).size(); ++l)
					if(hash.at(k).at(l) != hash.at(i).at(j))
					{
						long long sum = hash.at(k).at(l) + hash.at(i).at(j);
						if(sum >= LOWER_BOUND && sum <= UPPER_BOUND)
							sums.at(sum - LOWER_BOUND) = true;
					}
		}

	int counter = 0;
	for(int i = 0; i < sums.size(); ++i)
		if(sums.at(i))
			++counter;

	return counter;
}