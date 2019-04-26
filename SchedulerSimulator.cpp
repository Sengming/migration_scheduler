#include<iostream>
#include"SimulatorCore/Task.h"
#include"Containers/Set.h"
#include"Iterators/AbstractIterator.h"
#include"SimulatorCore/Model.h"
#include"SimulatorCore/Simulator.h"

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







int main()
{
	//Declare the schedulers
	FIFO SchedulerFIFO;
	EDF SchedulerEDF[5];

	int runtime = 200;

	//For MANUALLY creating tasks:
	//Create a Task Set:
	Task a(1, 0, 100, 24, 100);
	Task b(2, 0, 80, 20, 80);
	Task c(3, 0, 60, 20, 60);

	Set<Task*> myTasks;
	myTasks.addItem(&a);
	myTasks.addItem(&b);
	myTasks.addItem(&c);

	Set<Task*> remoteTasks[5];
	Set<Model*> remoteModels;

	//Create models
	Model localModel("LocalModel", &myTasks, &SchedulerFIFO, runtime);
	
	Model remoteModel0("RemoteModel", &remoteTasks[0], &SchedulerEDF[0], runtime);
	Model remoteModel1("RemoteModel", &remoteTasks[1], &SchedulerEDF[1], runtime);
	Model remoteModel2("RemoteModel", &remoteTasks[2], &SchedulerEDF[2], runtime);
	Model remoteModel3("RemoteModel", &remoteTasks[3], &SchedulerEDF[3], runtime);
	Model remoteModel4("RemoteModel", &remoteTasks[4], &SchedulerEDF[4], runtime);
	
	remoteModels.addItem(&remoteModel0);
	remoteModels.addItem(&remoteModel1);
	remoteModels.addItem(&remoteModel2);
	remoteModels.addItem(&remoteModel3);
	remoteModels.addItem(&remoteModel4);

	//Create a simulator
	Simulator mySimulator;

	//Run simulations
	mySimulator.runSimulation(&localModel, remoteModels);
}

