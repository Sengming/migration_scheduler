#include "Simulator.h"
#include "RemoteSimulator.h"
#include "MigrationScheduler.h"

int Simulator::runSimulation(Model* myModel, Set<RemoteSimulator*>* remoteSimulators)
{
	//Initialization
	AbstractIterator<Event*>* eventIterator = eventQueue.createIterator();
	AbstractIterator<RemoteSimulator*>* remoteSimIterator = remoteSimulators->createIterator();

	simModel = myModel;
	runTime = simModel->RunTime;
	timeInterrupt = simModel->TimeInterval;
	contextSwitch = simModel->ContextSwitch;
	preemptive = simModel->scheduler->isPreemptive();
	simModel->scheduler->setTasks(simModel->TaskSet);
	m_remoteSimulators = remoteSimulators;

	logMonitor.Initialize(simModel->ModelName);
	logMonitor.logTasks(*(simModel->TaskSet));

	double time = 0;
	Event* NextEvent;

	// Remote Sim Set individual scheduler tasks
	for(remoteSimIterator->First(); !remoteSimIterator->IsDone(); remoteSimIterator->Next()){
		RemoteSimulator* remoteSim = remoteSimIterator->CurrentItem();
		remoteSim->initializeRemoteSim(&m_migQueue);
	}

	//Create First Event
	Event first(TimeInterrupt, 0);

	//Create Final Event
	Event last(SimulationFinished, runTime);

	//Insert Events in EventQueue
	eventQueue.addItem(&first);
	eventQueue.addItem(&last);


	while (time < runTime)
	{
		eventQueue.sortQueue();
		eventIterator->First();
		NextEvent = eventIterator->CurrentItem();
		eventQueue.remove(NextEvent);
		time = NextEvent->getEventTime();

		//Update Tasks, and check for deadline misses.
		simModel->modelTaskHandler.updateTaskStates(&logMonitor, time);
		simModel->modelTaskHandler.checkForDeadlineBreaches(&logMonitor, time);

		currentEvent = NextEvent->getEventType();
		// We check for overload here
		//Handle current Event.
		switch (currentEvent)
		{
		case TimeInterrupt:
			onTimeInterrupt(time);
			break;
		case TaskReady:
			onTaskReady(time);
			break;
		case TaskFinished:
			onTaskFinished(time);
			break;
		case SimulationFinished:
			onSimulationFinished(time);
			break;
		default:
			break;
		}
		// Tick all remote simulations
		for(remoteSimIterator->First(); !remoteSimIterator->IsDone(); remoteSimIterator->Next()){
			RemoteSimulator* remoteSim = remoteSimIterator->CurrentItem();
			remoteSim->tickRemoteSimulations(time);

		 }

		// Tick Migration Scheduler
		// Inside tick, migrationScheduler checks Tasks vs CPUs and
		// generates event if we need to migrate to remote.

		MigrationInstruction migInstruction =
				simModel->m_migration_scheduler->checkMigrate(&m_migQueue);

		// If we need to migrate, add to remove taskset
		if (migInstruction.migrate == true) {
				addToRemoteTaskSet(migInstruction.simulatorTarget, migInstruction.migratedTask);
		}
	}
	return 1;
}

Simulator::Simulator()
{
	currentTask = new Task;
}


Simulator::~Simulator()
{
}

void Simulator::onTimeInterrupt(double time)
{
	//create next TimeInterrupt event
	static Event timeInter(TimeInterrupt, (time + timeInterrupt));
	timeInter.setEventTime(time + timeInterrupt);
	eventQueue.addItem(&timeInter);

	if (preemptive || time == 0)
	{
		runScheduler(time);
	}
}

void Simulator::runScheduler(double time)
{
	Task* nextTask(0);

	// If task set is empty we don't want to schedule
	if (simModel->isTaskSetEmpty()){
		return;
	}

	//Ask Scheduler to schedule if there are any tasks available.
	if (simModel->modelTaskHandler.taskAvailable())
	{
		nextTask = simModel->scheduler->Schedule(time);
	}
	else
	{
		nextTask = simModel->modelTaskHandler.getTaskClosestInTime(time);
	}

	if (nextTask->getState() == IDLE)
	{
		nextTask = simModel->modelTaskHandler.getTaskClosestInTime(time);
		if (!preemptive)
		{
			static Event readyTask(TaskReady, (nextTask->getTarrival()));
			readyTask.setEventTime(nextTask->getTarrival());
			eventQueue.addItem(&readyTask);
			currentTask = nextTask;
		}
		return;
	}
	if (nextTask->getState() == FINISHED)
	{
		//WHAT?
		return;
	}

	//If a different task is currently being handled, this task must delete its job finished event. 
	if (currentEvent == TimeInterrupt && time != 0)
	{
		if (nextTask->getID() != currentTask->getID() || nextTask->getState() == READY)
		{
			//If the current task is RUNNING, it has been "blocked".
			if (currentTask->getState() == RUNNING)
			{
				if (currentTask->getID() != 0)
				{
					eventQueue.remove(currentTaskFinishedEvent);
					currentTask->updateProgressionTime(time);
				}
				currentTask->State = WAITING;
				logMonitor.logPause(*currentTask, time);

				//add ContextSwitch penalty
				time = time + contextSwitch;
			}
			currentTask = nextTask;
			setUpTaskForExecution(time);
			return;
		}
	}
	if (currentEvent == TaskReady || time == 0)
	{
		currentTask = nextTask;
		setUpTaskForExecution(time);
	}

}

void Simulator::setUpTaskForExecution(double time)
{
	currentTask->Tstarted = time;

	static Event TaskFin(TaskFinished, (currentTask->getRemainingExecutionTime() + time));
	TaskFin.setEventTime((currentTask->getRemainingExecutionTime() + time));
	eventQueue.addItem(&TaskFin);
	currentTaskFinishedEvent = &TaskFin;

	currentTask->State = RUNNING;
	logMonitor.logStart(*currentTask, time);
}

void Simulator::onTaskReady(double time)
{
	runScheduler(time);
}

void Simulator::onTaskFinished(double time)
{
	currentTask->State = FINISHED;
	static Event readyTask(TaskReady, time);
	readyTask.setEventTime(time);
	logMonitor.logEnd(*currentTask, time);
	eventQueue.addItem(&readyTask);
}


void Simulator::onSimulationFinished(double time)
{
	eventQueue.sortQueue();
	AbstractIterator<Event*>* it = eventQueue.createIterator();
	it->First();
	if (it->CurrentItem()->getEventType() == TaskFinished && it->CurrentItem()->getEventTime() == time)
	{
		logMonitor.logEnd(*currentTask, time);
	}
	//logMonitor.logSimEnd(time);
	//logMonitor.generateReport();
	resetSimulator();
}

void Simulator::addToRemoteTaskSet(int simulatorNumber, Task* task)
{
	RemoteSimulator* remoteSim = m_remoteSimulators->getItem(simulatorNumber);
	// Successfully removed from original task set and added to remote task set, else do nothing
	if (simModel->removeFromTaskSet(task)){
		remoteSim->addToTaskSet(task);
		std::cout << "Add to task set" << std::endl;
	}
}

void Simulator::resetSimulator()
{
	simModel->modelTaskHandler.resetTasks();
	eventQueue.emptyQueue();
}
