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


const int MAX_TASK = 100;

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
	.deadline = 60,
	.executiontimehost = 133.06,
	.executiontimeremote = 303.90,
	.period = 60,
	.memoryusage = 0
};

struct bench CG{
	.tarrival = 0,
	.deadline = 60,
	.executiontimehost = 90.00,
	.executiontimeremote = 2678.16,
	.period = 60,
	.memoryusage = 0
};

struct bench LU{
	.tarrival = 0,
	.deadline = 60,
	.executiontimehost = 120.28,
	.executiontimeremote = 1538.78,
	.period = 60,
	.memoryusage = 0
};

struct bench UA{
	.tarrival = 0,
	.deadline = 60,
	.executiontimehost = 74.46,
	.executiontimeremote = 768.67,
	.period = 60,
	.memoryusage = 0
};

struct bench Kmeans{
	.tarrival = 0,
	.deadline = 60,
	.executiontimehost = 20,
	.executiontimeremote = 60,
	.period = 60,
	.memoryusage = 0
};

int main()
{
	//Declare the schedulers
	FIFO SchedulerFIFO;
	EDF SchedulerEDF[5];

	int runtime = 200;
	int totalTasks = 10;
	int taskSet = 0;


	Task *tasks[MAX_TASK];
	Set<Task*> myTasks;

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

	Set<Task*> remoteTasks[5];
	Set<Model*> remoteModels;

	Set <RemoteSimulator*> remoteSimulators;

	// Instantiate migration scheduler
	MigrationScheduler migrationScheduler(&myTasks, 5);

	//Create models and simulators
	Model localModel("LocalModel", &myTasks, &SchedulerFIFO, runtime,
			 &migrationScheduler);

	Model remoteModel0("RemoteModel", &remoteTasks[0], &SchedulerEDF[0], runtime);
	Model remoteModel1("RemoteModel", &remoteTasks[1], &SchedulerEDF[1], runtime);
	Model remoteModel2("RemoteModel", &remoteTasks[2], &SchedulerEDF[2], runtime);
	Model remoteModel3("RemoteModel", &remoteTasks[3], &SchedulerEDF[3], runtime);
	Model remoteModel4("RemoteModel", &remoteTasks[4], &SchedulerEDF[4], runtime);

	// Add to list of remote models
	remoteModels.addItem(&remoteModel0);
	remoteModels.addItem(&remoteModel1);
	remoteModels.addItem(&remoteModel2);
	remoteModels.addItem(&remoteModel3);
	remoteModels.addItem(&remoteModel4);

	RemoteSimulator remoteSimulator0(&remoteModel0, 0);
	RemoteSimulator remoteSimulator1(&remoteModel1, 1);
	RemoteSimulator remoteSimulator2(&remoteModel2, 2);
	RemoteSimulator remoteSimulator3(&remoteModel3, 3);
	RemoteSimulator remoteSimulator4(&remoteModel4, 4);

	remoteSimulators.addItem(&remoteSimulator0);
	remoteSimulators.addItem(&remoteSimulator1);
	remoteSimulators.addItem(&remoteSimulator2);
	remoteSimulators.addItem(&remoteSimulator3);
	remoteSimulators.addItem(&remoteSimulator4);

	//Create a simulator
	Simulator mySimulator;

	//Run simulations
	mySimulator.runSimulation(&localModel, &remoteSimulators);

	for(int i=0; i<totalTasks; i++)
		free(tasks[i]);
}


