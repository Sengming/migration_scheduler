#include "RemoteSimulator.h"
#include "Simulator.h"
#include <iostream>

int RemoteSimulator::runSimulation(Model* myModel, Set<RemoteSimulator*>* remoteSimulators)
{
	std::cout << "Remote runSimulations currently not allowed" << std::endl;
	return 0;
}

int RemoteSimulator::tickRemoteSimulations(int time)
{
	//Initialization
	AbstractIterator<Event*>* eventIterator = eventQueue.createIterator();

	timeInterrupt = simModel->TimeInterval;
	contextSwitch = simModel->ContextSwitch;
	preemptive = simModel->scheduler->isPreemptive();

	Event* NextEvent;

	eventQueue.sortQueue();
	eventIterator->First();
	NextEvent = eventIterator->CurrentItem();
	eventQueue.remove(NextEvent);

	//Update Tasks, and check for deadline misses.
	simModel->modelTaskHandler.updateTaskStates(&logMonitor, time);
	simModel->modelTaskHandler.checkForDeadlineBreaches(&logMonitor, time);

	currentEvent = NextEvent->getEventType();
	// We check for overload here
	//Handle current Event.
	switch (currentEvent)
	{
	case TimeInterrupt:
		std::cout << "Remote Time interrupt " << time << std::endl;
		onTimeInterrupt(time);
		break;
	case TaskReady:
		std::cout << "Task Ready " << time << std::endl;
		onTaskReady(time);
		break;
	case TaskFinished:
		std::cout << "Task Finished " << time << std::endl;
		onTaskFinished(time);
		break;
	default:
		break;
	}

	return 1;
}
void RemoteSimulator::initializeRemoteSim()
{
	simModel->scheduler->setTasks(simModel->TaskSet);
	//Create First Event
	Event* first = new Event(TimeInterrupt, 0);
	eventQueue.addItem(first);
}

RemoteSimulator::RemoteSimulator(Model* model) : Simulator()
{
	simModel = model;
}

void RemoteSimulator::addToTaskSet(Task* task)
{
	simModel->addToTaskSet(task);
}

RemoteSimulator::~RemoteSimulator()
{

}
