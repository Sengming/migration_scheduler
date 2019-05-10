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
	// simModel->modelTaskHandler.checkForDeadlineBreaches(&logMonitor, time);

	currentEvent = NextEvent->getEventType();

	//Handle current Event.
	switch (currentEvent)
	{
	case TimeInterrupt:
		//std::cout << "Remote Node " << myId <<" Time interrupt " << time << std::endl;
		onTimeInterrupt(time);
		break;
	case TaskReady:
		//std::cout << "Task Ready " << time << " " << currentTask << " Progression: " << currentTask->ExecutionTime << std::endl;
		onTaskReady(time);
		// std::cout << "[" << time << "]"
		// 		  << "Started Execution for Task: " << currentTask->getID() << std::endl;
		break;
	case TaskFinished:
		//std::cout << "Task Finished " << time << " " << currentTask <<" Progression: " << currentTask->ExecutionTime << std::endl;
		currentTask->updateProgressionTime(time);
		std::cout << "["<< time <<"]" << "Task on Remote Node " << myId << " completed: " << currentTask->getID() << " Progression: " << currentTask->Progression << std::endl;
		onTaskFinished(time);
		break;
	default:
		break;
	}

	return 1;
}
void RemoteSimulator::initializeRemoteSim(Queue<MigrationEvent*>* migrationQueue)
{
	simModel->scheduler->setTasks(simModel->TaskSet);
	//Create First Event
	Event* first = new Event(TimeInterrupt, 0);
	eventQueue.addItem(first);

	// Initialize our pointer to the main simulator's migrationQueue
	m_migQueue = migrationQueue;
}

void RemoteSimulator::setUpTaskForExecution(double time)
{
	Event* TaskFin = new Event(TaskFinished, (currentTask->getRemainingExecutionTime() + time));
		// std::cout << "Current Task: " << currentTask->getID() << " Remaining: " << currentTask->getRemainingExecutionTime() << std::endl;
	TaskFin->setEventTime((currentTask->getRemainingExecutionTime() + time));
	eventQueue.addItem(TaskFin);
	currentTaskFinishedEvent = TaskFin;

	currentTask->State = RUNNING;
	logMonitor.logStart(*currentTask, time);
}

void RemoteSimulator::onTaskFinished(double time)
{
	// Run base class' method first
	Simulator::onTaskFinished(time);
	simModel->removeFromTaskSet(currentTask);
	MigrationEvent* event = new MigrationEvent();
	event->remoteComplete = myId;
	event->taskComplete = currentTask;
	m_migQueue->addItem(event);
}

RemoteSimulator::RemoteSimulator(Model* model, unsigned id) : Simulator()
{
	simModel = model;
	myId = id;
}

void RemoteSimulator::addToTaskSet(Task* task)
{
	simModel->addToTaskSet(task);
}

RemoteSimulator::~RemoteSimulator()
{

}
