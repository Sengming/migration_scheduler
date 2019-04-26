#pragma once
#include <fstream>
#include<string>

#include"../SimulatorCore/Task.h"
#include"LogEvent.h"
#include"../Containers/Set.h"
#include"../Containers/Queue.h"

/*****************************************************************************
The Monitor keeps track of the events that occur during a simulation.
A report can be generated after the simulation has completed. 

"taskIDs" contains all the IDs for the tasks currently being 
simulated by the simulator.

"LogEventQueue" contains the logged events(LogEvent objects) from the simulation.

"myfile" is the outstream to the tex file that is going to be used to generate
a pdf-file to present resulting relevant information from the simulation.

"name" is the name of the Model currently being simulated, being used as a name
for the resulting report pdf file.

Functions starting with "log" are used by the simulator to log events.

"generateReport()" generates a report containing relevant information about the 
simulation. The function loops through the LogEventQueue, and handles the LogEvents
with appropriate functions.

The resulting report contains:
- The Model Name
- A list of tasks and their parameters
- Average TurnAround Time
- CPU idle time
- Number of Deadlines missed and reached
- Number of tasks executed at least one time.
- Gantt chart.
************************************************************************************/


class Monitor
{
private:
	std::vector<int> taskIDs;
	Queue<LogEvent> LogEventQueue;
	std::ofstream  myfile;
	std::string name;

	//The RT box is used to create Gantt charts
	void createRTBox(int numberOfTasks, double time, int iteration);
	void endRTBox();
	

	//functions to handle logEvents when generating report:
	void onArrived(LogEvent log);
	void onStarted(LogEvent log);
	void onBlocked(LogEvent log, double time);
	void onFinished(LogEvent log, double time);
	void onDeadline(LogEvent log);
	void onDeadlineMissed(LogEvent log);

	//Calculate TurnAroundTime, CPU IDLE time, DeadlineMisses...
	void calculateSimulatorStatistics(double time);
	//... using these functions:
	double CPUidleTime();
	double numberOfDeadlinesReached();
	double numberOfDeadlinesMissed();
	double AverageTurnAroundTime(int taskid);
	double numberOfTasksCompleted();

	void runLatex();

public:
	Monitor(){};
	void Initialize(std::string resultsname);
	~Monitor();

	//Functions to log events and information
	bool logTasks(Set<Task*> tasks);
	bool logArrivalTime(Task arrTask, double arrTime);
	bool logStart(Task startTaskId, double time);
	bool logPause(Task pauseTaskId, double time);
	bool logEnd(Task endTaskId, double time);
	bool logDeadline(Task deadlineTask, double time);
	bool logDeadlineBreach(Task deadId, double time);
	bool logSimEnd(double time);

	void generateReport();

};

