#include "Event.h"


Event::Event(int tempEvent, double tempTime)
{
	EventType = tempEvent;
	EventTime = tempTime;
}

Event::Event()
{
	EventType = 0;
	EventTime = 0;
}

Event::~Event()
{
}

void Event::setEventTime(double tempTime)
{
	EventTime = tempTime;
}
int Event::getEventType()
{
	return EventType;
}

double Event::getEventTime() const
{
	return EventTime;
}