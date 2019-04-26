#pragma once
#include"AbstractIterator.h"
#include"../Containers/AbstractContainer.h"

/*************************************************************
The GeneralIterator can traverse a container inheriting the
AbstractContainer class. Can be used to traverse all elements
in the container.
*************************************************************/



template<class Item>
class AbstractContainer;

template<class Item>
class GeneralIterator :
	public AbstractIterator<Item>
{
public:
	GeneralIterator(AbstractContainer<Item>* items);
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
GeneralIterator<Item>::GeneralIterator(AbstractContainer<Item>* items)
{
	myItems = items;
}

template<class Item>
void GeneralIterator<Item>::First()
{
	current = 0;
}

template<class Item>
void GeneralIterator<Item>::Next()
{
	current++;
}

template<class Item>
bool GeneralIterator<Item>::IsDone()
{
	return (current >= myItems->numberOfItems());
}

template<class Item>
Item GeneralIterator<Item>::CurrentItem()
{
	return (myItems->getItem(current));
}

template<class Item>
double GeneralIterator<Item>::NumberOfItems()
{
	return myItems->numberOfItems();
}
