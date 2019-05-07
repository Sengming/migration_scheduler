#include "Model.h"

Model::Model()
{
	TimeInterval = 1;
	RunTime = 100;
	ContextSwitch = 0;
	ModelName = "simulationResults";
	modelTaskHandler.initialize(TaskSet->createIterator(), TaskSet->createAvailableTasksIterator());
}
Model::Model(std::string modelName, Set<Task*>* modelTaskSet, Scheduler* modelScheduler, double modelRunTime)
{
	ModelName = modelName;
	scheduler = modelScheduler;
	setTaskSet(modelTaskSet);
	RunTime = modelRunTime;
	ContextSwitch = 0;
	TimeInterval = 1;
	modelTaskHandler.initialize(TaskSet->createIterator(), TaskSet->createAvailableTasksIterator());

}

Model::Model(std::string modelName, int numberOfPeriodicTasks, int numberOfAperiodicTask, Scheduler* modelScheduler)
{
	ModelName = modelName;
	scheduler = modelScheduler;
	
	TaskSet = new Set < Task* > ;
	TaskSet->addItems(modelTaskHandler.generateTasks(numberOfPeriodicTasks, true));
	TaskSet->addItems(modelTaskHandler.generateTasks(numberOfAperiodicTask, false));

	setTaskSet(TaskSet);
	RunTime = 100;
	ContextSwitch = 0;
	TimeInterval = 1;
	modelTaskHandler.initialize(TaskSet->createIterator(), TaskSet->createAvailableTasksIterator());
}


Model::~Model()
{
}

void Model::setModelName(std::string myName)
{
	ModelName = myName;
}

void Model::setScheduler(Scheduler* myScheduler)
{
	scheduler = myScheduler;
}


void Model::setTaskSet(Set<Task*>* myTasks)
{
	TaskSet = myTasks;
}


void Model::setRunTime(double time)
{
	RunTime = time;
}



void Model::setContextSwitch(double msTime)
{
	ContextSwitch = msTime;
}

void Model::setTimeInterval(double time)
{
	TimeInterval = time;
}

bool Model::isTaskSetEmpty()
{
	return (TaskSet->numberOfItems() == 0);

}

void Model::addToTaskSet(Task* task)
{
	TaskSet->addItem(task);
}

void Model::removeFromTaskSet(Task* task)
{
	TaskSet->remove(task);
}