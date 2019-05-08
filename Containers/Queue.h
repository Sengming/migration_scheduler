#pragma once
#include<list>
#include"AbstractContainer.h"
#include"../Iterators/GeneralIterator.h"
#include"../Iterators/SortedQueueIterator.h"


/*****************************************************************
 This is a container, a sub-set of "AbstractContainer" class. 

 This Queue Container can be sorted, if the template Item 
 can be compared as in the funtion "compareItems". 

 The function "emptyQueue()" clears the content of the container.
******************************************************************/


template<class Item>
class AbstractIterator;

template<class Item>
class SortedQueueIterator;

template<class Item>
class GeneralIterator;


template<class Item>
bool compareItems(const Item& lhs, const Item& rhs)
{
	return (*lhs<*rhs);
}


template<class Item>
class Queue :
	public AbstractContainer<Item>
{
private:
	std::list<Item> myItems;
public:
	AbstractIterator<Item>* createIterator();
	void addItem(Item a);
	long numberOfItems();
	bool remove(Item a);
	Item getItem(long index);
	void sortQueue();
	void emptyQueue();
};


template<class Item>
void Queue<Item>::emptyQueue()
{
	myItems.clear();
}

template<class Item>
AbstractIterator<Item>* Queue<Item>::createIterator()
{
	return new GeneralIterator<Item>(this);
}

template<class Item>
void Queue<Item>::addItem(Item a)
{
	myItems.push_back(a);
}

template<class Item>
long Queue<Item>::numberOfItems()
{
	return myItems.size();
}

template<class Item>
bool Queue<Item>::remove(Item a)
{
	bool retVal = true;
	myItems.remove(a);
	return retVal;
}

template<class Item>
Item Queue<Item>::getItem(long index)
{
	typename std::list<Item>::iterator it = myItems.begin();
	std::advance(it, index);
	return *it;
}

template<class Item>
void Queue<Item>::sortQueue()
{
	myItems.sort(compareItems<Item>);
}
