#include<iostream>
#include"SimulatorCore/Task.h"
#include"Containers/Set.h"
#include"Iterators/AbstractIterator.h"
#include"SimulatorCore/Model.h"
#include "SimulatorCore/Simulator.h"
#include "SimulatorCore/RemoteSimulator.h"
#include "SimulatorCore/MigrationScheduler.h"

//Include Schedulers here
#include"Schedulers/RM.h"
#include"Schedulers/RR.h"
#include"Schedulers/FIFO.h"
#include"Schedulers/EDF.h"

/******************************************************************
					SchedulerSimulator

This file contains the main()-function for the Scheduler Simulator.

This software is created as a part of a Masters project at NTNU.
The SchedulerSimulator can simulate the execution of task sets, 
based on a scheduler algorithm. Relevant reports will be generated.
New schedulers can be added and simulated. Task sets can be created 
manually and generated. 

						June 2015
				by Inger Johanne Rudolfsen
*******************************************************************/


const int MAX_TASK = 10000;
const int MAX_REMOTE_NODE = 100;

struct bench{
	double tarrival;
	double deadline;
	double executiontimehost;
	double executiontimeremote;
	double period;
	unsigned memoryusage;
};

struct bench EP{
	.tarrival = 0,
	.deadline = 100000000,
	.executiontimehost = 133.06,
	.executiontimeremote = 303.90,
	.period =  100000000,
	.memoryusage = 0
};

struct bench CG{
	.tarrival = 0,
	.deadline = 100000000,
	.executiontimehost = 90.00,
	.executiontimeremote = 2678.16,
	.period =  100000000,
	.memoryusage = 0
};

struct bench LU{
	.tarrival = 0,
	.deadline = 100000000,
	.executiontimehost = 120.28,
	.executiontimeremote = 1538.78,
	.period =  100000000,
	.memoryusage = 0
};

struct bench UA{
	.tarrival = 0,
	.deadline =  100000000,
	.executiontimehost = 74.46,
	.executiontimeremote = 768.67,
	.period =  100000000,
	.memoryusage = 0
};

struct bench Kmeans{
	.tarrival = 0,
	.deadline =  100000000,
	.executiontimehost = 20,
	.executiontimeremote = 60,
	.period =  100000000,
	.memoryusage = 0
};

int main(int argc, char* argv[])
{
	//Declare the schedulers
	FIFO SchedulerFIFO;
	FIFO SchedulerEDF[MAX_REMOTE_NODE];

	int runtime = 200000;
	int totalTasks = 1000;
	int taskSet = 0;
	int remoteNodes = 5;


	Task *tasks[MAX_TASK];
	Set<Task*> myTasks;


	if(argc < 4)
	{
		std::cout << "./Simulator TotalTasks TaskSet TotalRemoteNode" << endl;
		return 0;
	}

	std::string::size_type sz;   // alias of size_t

	totalTasks = std::stoi(argv[1], &sz);
	taskSet = std::stoi(argv[2], &sz);
	remoteNodes = std::stoi(argv[3], &sz);

	for(int i=0; i<totalTasks; i++)
	{
		switch(taskSet)
		{
			case 0:
				tasks[i] = new Task(i+1, EP.tarrival, EP.deadline, EP.executiontimehost, EP.executiontimeremote, EP.period, EP.memoryusage);
				myTasks.addItem(tasks[i]);
				break;

			case 1:
				tasks[i] = new Task(i+1, CG.tarrival, CG.deadline, CG.executiontimehost, CG.executiontimeremote, CG.period, CG.memoryusage);
				myTasks.addItem(tasks[i]);
				break;

			case 2:
				tasks[i] = new Task(i+1, EP.tarrival, EP.deadline, EP.executiontimehost, EP.executiontimeremote, EP.period, EP.memoryusage);
				myTasks.addItem(tasks[i++]);
				tasks[i] = new Task(i+1, CG.tarrival, CG.deadline, CG.executiontimehost, CG.executiontimeremote, CG.period, CG.memoryusage);
				myTasks.addItem(tasks[i]);
				break;

			case 3:
				tasks[i] = new Task(i+1, EP.tarrival, EP.deadline, EP.executiontimehost, EP.executiontimeremote, EP.period, EP.memoryusage);
				myTasks.addItem(tasks[i++]);
				tasks[i] = new Task(i+1, CG.tarrival, CG.deadline, CG.executiontimehost, CG.executiontimeremote, CG.period, CG.memoryusage);
				myTasks.addItem(tasks[i++]);
				tasks[i] = new Task(i+1, UA.tarrival, UA.deadline, UA.executiontimehost, UA.executiontimeremote, UA.period, UA.memoryusage);
				myTasks.addItem(tasks[i++]);
				tasks[i] = new Task(i+1, LU.tarrival, LU.deadline, LU.executiontimehost, LU.executiontimeremote, LU.period, LU.memoryusage);
//				myTasks.addItem(tasks[i++]);
//				tasks[i] = new Task(i+1, Kmeans.tarrival, Kmeans.deadline, Kmeans.executiontimehost, Kmeans.executiontimeremote, Kmeans.period, Kmeans.memoryusage);
				myTasks.addItem(tasks[i]);
				break;

			default:
				break;
		}
	}

	Set<Task*> remoteTasks[MAX_REMOTE_NODE];
	Set<Model*> remoteModels;

	Set <RemoteSimulator*> remoteSimulators;

	// Instantiate migration scheduler
	MigrationScheduler migrationScheduler(&myTasks, remoteNodes);

	//Create models and simulators
	Model localModel("LocalModel", &myTasks, &SchedulerFIFO, runtime,
			 &migrationScheduler);

	Model *remoteModel[MAX_REMOTE_NODE];
	RemoteSimulator *remoteSimulator[MAX_REMOTE_NODE];
	for(int i=0; i<remoteNodes; i++)
	{
		remoteModel[i] = new Model ("RemoteModel", &remoteTasks[i], &SchedulerEDF[i], runtime);
		remoteModels.addItem(remoteModel[i]);
		remoteSimulator[i] = new RemoteSimulator(remoteModel[i], i);
		remoteSimulators.addItem(remoteSimulator[i]);
	}

	//Create a simulator
	Simulator mySimulator;

	//Run simulations
	mySimulator.runSimulation(&localModel, &remoteSimulators);

	for(int i=0; i<totalTasks; i++)
		free(tasks[i]);

	for(int i=0; i<remoteNodes; i++)
	{
		free(remoteModel[i]);
		free(remoteSimulator[i]);
	}

	return 0;

}


