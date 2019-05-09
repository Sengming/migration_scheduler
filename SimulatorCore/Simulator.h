#pragma once
#include"../Monitor/Monitor.h"
#include"Event.h"
#include"Model.h"
#include"../Containers/Queue.h"
#include "MigrationScheduler.h"

class RemoteSimulator;

/*************************************************************
The Simulator conducts the simulation of the behavior of a 
processor executing a task set using a scheduler policy.

The Simulator runs a simulation using a model, containing all
the parameters needed to condutct the simulation. 

The simulation is event based, where different events evoces
calls on different functions to handle the event. 

**************************************************************/


class Simulator
{
private:
	// Remote simulators
	Set<RemoteSimulator*>* m_remoteSimulators;

	void addToRemoteTaskSet(int simulatorNumber, Task* task);

protected:
	Monitor logMonitor;


	Task* currentTask;
	Event* currentTaskFinishedEvent;
	int currentEvent;

	Queue<Event*> eventQueue;

	Queue<MigrationEvent*> m_migQueue;
	//Simulation parameters
	bool preemptive;
	double timeInterrupt;
	double runTime;
	double contextSwitch;

	//Event-handeling functions:
	void onTimeInterrupt(double time);
	void onTaskReady(double time);
	virtual void onTaskFinished(double time);
	void onSimulationFinished(double time);

	void runScheduler(double time);
	void setUpTaskForExecution(double time);

	//After a simulation, the simulator must reset
	void resetSimulator();
public:
	Simulator();
	~Simulator();
	int runSimulation(Model* myModel, Set<RemoteSimulator*>* remoteSimulators);
	Model* simModel;
};

