#pragma once

/******************************************************************
The Task represents a process or thread that is to be executed. 

ExecutionTime represents the estimated execution time of the task.

Progression is the time the task has executed since Tstarted. 

Tarrival is the time the task is submitted to the system.

FirstTarrival is the original value of Tarrival, created to reset
the task later, as Tarrival is changed and updated for periodic
tasks.

Tstarted is the time the execution of a task is started. 

Deadline is the relative deadline of the task.

Period is the period of a periodic task. If the task is non-periodic,
the period is zero.

State is the task state in its life cycle, which can be:
IDLE, READY, RUNNING, WAITING, BLOCKED or FINISHED

ID is the task identification, or task number. This must be a unique
number, not shared with other tasks in the same task set. 
*********************************************************************/

class TaskHandler;
class Simulator;
enum taskState{ IDLE, READY, RUNNING, WAITING, BLOCKED, FINISHED };

class Task
{
	friend class TaskHandler;
	friend class Simulator;
private:
	double ExecutionTime;
	double Progression;
	double Tarrival;
	double FirstTarrival;
	double Tstarted;
	double Deadline;
	double Period;
	unsigned MemoryUsage;

	int State;
	int ID;

	bool DeadlineMissed;

	void updateProgressionTime(double time);

public:
	Task();
	Task(int id, double tarrival, double deadline, double executiontime, double period);  
        Task(int id, double tarrival, double deadline, double executiontime, double period, unsigned memoryusage=2);
	Task(int id);
	~Task();

	int Priority;

	int getID();
	int getState();
	double getPeriod();
	double getDeadline();
	double getTarrival();
	double getExecutionTime();
	double getRemainingExecutionTime();
};
