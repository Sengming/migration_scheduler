#pragma once
#include "AbstractIterator.h"
#include"../Containers/AbstractContainer.h"

/****************************************************
The AvailableTaskIterator is an iterator for a Set 
containing Task object pointers. 

This iterator will only point to tasks available for
execution, that is: tasks with the states:
READY, RUNNING or WAITING.
*****************************************************/

template<class Item>
class AbstractContainer;

template<class Item>
class AvailableTasksIterator :
	public AbstractIterator < Item >
{
public:
	AvailableTasksIterator(AbstractContainer<Item> *items);
	AvailableTasksIterator(){};

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
AvailableTasksIterator<Item>::AvailableTasksIterator(AbstractContainer<Item> *items)
{
	myItems = items;
}

template<class Item>
void AvailableTasksIterator<Item>::First()
{
	current = 0;
	while (!IsDone())
	{
		int tempstate = myItems->getItem(current)->getState();
		if (tempstate == READY || tempstate == RUNNING || tempstate == WAITING)
		{
			return;
		}
		current++;
	}
}

template<class Item>
void AvailableTasksIterator<Item>::Next()
{
	current++;
	while (!IsDone())
	{
		int tempstate = myItems->getItem(current)->getState();
		if (tempstate == READY || tempstate == RUNNING || tempstate == WAITING)
		{
			return;
		}
		current++;
	}
}

template<class Item>
bool AvailableTasksIterator<Item>::IsDone()
{
	return (current >= myItems->numberOfItems());
}

template<class Item>
Item AvailableTasksIterator<Item>::CurrentItem()
{
	if (current == myItems->numberOfItems())
	{
		First();
	}
	return (myItems->getItem(current));
}

template<class Item>
double AvailableTasksIterator<Item>::NumberOfItems()
{
	return myItems->numberOfItems();
}
