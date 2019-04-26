#pragma once
#include"../Iterators/AbstractIterator.h"

/********************************************************
This is an abstract class specifying the main interface
for interaction with the containers, aka its sub-classes.
**********************************************************/

template<class Item>
class AbstractContainer
{
public:
	virtual AbstractIterator<Item>* createIterator() = 0;
	virtual void addItem(Item a) = 0;
	virtual void remove(Item a) = 0;
	virtual long numberOfItems() = 0;
	virtual Item getItem(long index) = 0;
};

