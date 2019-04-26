#pragma once
#include "AbstractIterator.h"
#include"../Containers/AbstractContainer.h"
#include"../Containers/Queue.h"

template<class Item>
class AbstractContainer;

template<class Item>
class Queue;

template<class Item>
class SortedQueueIterator :
	public AbstractIterator < Item >
{
public:
	SortedQueueIterator(AbstractContainer<Item>* items);
	void First();
	void Next();
	bool IsDone();
	Item CurrentItem();
	double NumberOfItems();
private:
	AbstractContainer<Item>* myItems;
	long current;
};


template<class Item>
SortedQueueIterator<Item>::SortedQueueIterator(AbstractContainer<Item>* items)
{
	myItems = items;
	First();
}


template<class Item>
void SortedQueueIterator<Item>::First()
{
	current = 0;
}


template<class Item>
void SortedQueueIterator<Item>::Next()
{
	current++;
}

template<class Item>
bool SortedQueueIterator<Item>::IsDone()
{
	return (current >= myItems->numberOfItems());
}

template<class Item>
Item SortedQueueIterator<Item>::CurrentItem()
{
	myItems->sortQueue();
	return (myItems->getItem(current));
}

template<class Item>
double SortedQueueIterator<Item>::NumberOfItems()
{
	return myItems->numberOfItems();
}