#pragma once

/************************************************************
The Event describes the different events that can occur
during a simulation, and at what time they are set to 
happen.
************************************************************/


enum eventType{ TimeInterrupt, TaskReady, TaskFinished, SimulationFinished, MigrateToRemote, RemoteComplete };

class Event
{
private:
	int EventType;
	double EventTime;

public:
	Event(int tempEvent, double tempTime);
	Event();
	~Event();

	int getEventType();
	double getEventTime() const;
	void setEventTime(double tempTime);
	bool operator<(const Event& first)
	{
		return (EventTime < first.getEventTime());
	}
};



