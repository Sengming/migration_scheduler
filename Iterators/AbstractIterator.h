#pragma once


/********************************************************
This is an abstract class specifying the main interface
for interaction with the iterators, aka its sub-classes.
**********************************************************/

template<class Item>
class AbstractIterator
{
public:
	virtual void First() = 0;
	virtual void Next() = 0;
	virtual bool IsDone() = 0;
	virtual Item CurrentItem() = 0;
	virtual double NumberOfItems() = 0;
protected:
	AbstractIterator(){};
	~AbstractIterator(){};
};

