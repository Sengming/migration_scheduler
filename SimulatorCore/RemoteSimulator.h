#pragma once
#include"../Monitor/Monitor.h"
#include"Event.h"
#include"Model.h"
#include"../Containers/Queue.h"
#include "Simulator.h"

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
	RemoteSimulator(Model* model);
	~RemoteSimulator();
	void addToTaskSet(Task* task);
	int tickRemoteSimulations(int time);
	void initializeRemoteSim();
	virtual int runSimulation(Model* myModel, Set<RemoteSimulator*>* remoteSimulators);
};

