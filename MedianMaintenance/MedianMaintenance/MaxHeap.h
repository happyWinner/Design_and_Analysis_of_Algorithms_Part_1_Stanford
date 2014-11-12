#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <vector>

#endif

template<typename Comparable>
class MaxHeap
{
private:
	std::vector<Comparable> elem;

public:
	int size();
	void insert(Comparable);
	void erase(int);
	void heapify(std::vector<Comparable>);
	Comparable extract_max();
	Comparable get_max();
};

template<typename Comparable>
int MaxHeap<Comparable>::size()
{
	return elem.size();
}

template<typename Comparable>
void MaxHeap<Comparable>::insert(Comparable newElem)
{
	elem.push_back(newElem);

	// bubble up until heap property is restored
	int elemIdx = elem.size() - 1;	
	int parentIdx = (elemIdx - (elemIdx % 2 ? 1 : 2)) / 2;
	while(parentIdx >= 0 && elem.at(parentIdx) < elem.at(elemIdx))
	{
		std::swap(elem.at(parentIdx), elem.at(elemIdx));
		elemIdx = parentIdx;
		parentIdx = (elemIdx - (elemIdx % 2 ? 1 : 2)) / 2;
	}

	return;
}

template<typename Comparable>
void MaxHeap<Comparable>::erase(int elemIdx)
{
	std::swap(elem.at(elemIdx), elem.at(elem.size() - 1));
	elem.pop_back();

	// bubble down until heap property is restored
	int leftChildIdx  = 2 * elemIdx + 1;
	int rightChildIdx = 2 * elemIdx + 2;
	bool bContinue = true;
	while(bContinue)
	{
		// two child node
		if(rightChildIdx < elem.size())
		{
			if(elem.at(rightChildIdx) > elem.at(elemIdx) && elem.at(rightChildIdx) >= elem.at(leftChildIdx))
			{
				std::swap(elem.at(elemIdx), elem.at(rightChildIdx));
				elemIdx = rightChildIdx;
				leftChildIdx  = 2 * elemIdx + 1;
				rightChildIdx = 2 * elemIdx + 2;
			}
			else if(elem.at(leftChildIdx) > elem.at(elemIdx) && elem.at(leftChildIdx) >= elem.at(rightChildIdx))
			{
				std::swap(elem.at(elemIdx), elem.at(leftChildIdx));
				elemIdx = leftChildIdx;
				leftChildIdx  = 2 * elemIdx + 1;
				rightChildIdx = 2 * elemIdx + 2;
			}
			else
				bContinue = false;
		}
		
		// one child node
		if(leftChildIdx == elem.size() - 1)
		{
			if(elem.at(leftChildIdx) > elem.at(elemIdx))
				std::swap(elem.at(elemIdx), elem.at(leftChildIdx));
			bContinue = false;
		}

		// no child node
		if(leftChildIdx >= elem.size())
			bContinue = false;
	}

	return;
}

template<typename Comparable>
void MaxHeap<Comparable>::heapify(std::vector<Comparable> newElem)
{
	while(newElem.size())
	{
		insert(newElem.back());
		newElem.pop_back();
	}
	return;
}

template<typename Comparable>
Comparable MaxHeap<Comparable>::extract_max()
{
	Comparable max = elem.at(0);
	erase(0);
	return max;
}

template<typename Comparable>
Comparable MaxHeap<Comparable>::get_max()
{
	return elem.at(0);
}