#pragma once
#include<vector>
#include <algorithm>
#include"AbstractContainer.h"
#include"../Iterators/GeneralIterator.h"
#include"../Iterators/AvailableTasksIterator.h"


/************************************************************
This is a container, a sub-set of "AbstractContainer" class. 

Using a vector as base container, elements are easily accessed.

The iterator AvailableTaskIterator can be created if Task
objects are being stored. 
************************************************************/

template<class Item>
class AvailableTasksIterator;

template<class Item>
class GeneralIterator;

template<class Item>
class AbstractIterator;



template<class Item>
class Set :public AbstractContainer < Item >
{
private:
	std::vector<Item> myItems;
public:
	void addItem(Item a);
	long numberOfItems();
	void remove(Item a);
	Item getItem(long index);
	AbstractIterator<Item>* createIterator();
	AbstractIterator<Item>* createAvailableTasksIterator();
	void addItems(Set<Item> items);
};


template<class Item>
AbstractIterator<Item>* Set<Item>::createIterator()
{
	return new GeneralIterator<Item>(this);
}

template<class Item>
void Set<Item>::addItem(Item a)
{
	myItems.push_back(a);
}

template<class Item>
long Set<Item>::numberOfItems()
{
	return myItems.size();
}

template<class Item>
void Set<Item>::remove(Item a)
{
	typename std::vector<Item>::iterator itemIterator;
	itemIterator = std::find(myItems.begin(), myItems.end(), a);
	myItems.erase(itemIterator);
}

template<class Item>
Item Set<Item>::getItem(long index)
{
	return myItems.at(index);
}

template<class Item>
AbstractIterator<Item>* Set<Item>::createAvailableTasksIterator()
{
	return new AvailableTasksIterator<Item>(this);
}


template<class Item>
void Set<Item>::addItems(Set<Item> items)
{
	for (int i = 0; i < items.numberOfItems(); i++)
	{
		myItems.push_back(items.getItem(i));
	}
}
