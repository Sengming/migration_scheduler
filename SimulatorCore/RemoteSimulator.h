#pragma once
#include"../Monitor/Monitor.h"
#include"Event.h"
#include"Model.h"
#include"../Containers/Queue.h"
#include "Simulator.h"

class MigrationEvent;
/*************************************************************
The Simulator conducts the simulation of the behavior of a 
processor executing a task set using a scheduler policy.

The Simulator runs a simulation using a model, containing all
the parameters needed to condutct the simulation. 

The simulation is event based, where different events evoces
calls on different functions to handle the event. 

**************************************************************/


class RemoteSimulator : public Simulator
{
public:
	RemoteSimulator(Model* model, unsigned id);
	~RemoteSimulator();
	void addToTaskSet(Task* task);
	int tickRemoteSimulations(int time);
	void initializeRemoteSim(Queue<MigrationEvent*>* migrationQueue);
	virtual void onTaskFinished(double time);
	virtual int runSimulation(Model* myModel, Set<RemoteSimulator*>* remoteSimulators);

protected:
	Queue<MigrationEvent*>* m_migQueue;
	unsigned myId;
};

