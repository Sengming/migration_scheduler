#pragma once
#include"SimulatorCore/Task.h"
#include"Containers/Set.h"

struct MigrationInstruction 
{
    bool migrate;
    int simulatorTarget;
    Task* migratedTask;
};

struct MigrationEvent
{
    int remoteComplete;
    Task* taskComplete;
};

// class MigrationScheduler
// {
// protected:

// public:
// 	MigrationScheduler(Set<Task*> remoteTasks, int numberOfTasks);
// 	~MigrationScheduler();

// 	Task* Schedule(double time) = 0;
// };

