#include "Monitor.h"
#include<cmath>

#ifdef _WIN32
#define DELETE "DEL "
#define MOVE "move "

#else
#define DELETE "rm "
#define MOVE "mv "
#endif



void Monitor::Initialize(std::string resultsname)
{
	LogEventQueue.emptyQueue();
	taskIDs.clear();
	std::string b = ".tex";
	name = resultsname;
	std::string FilenameString = name + ".tex";
	const char* Filename = FilenameString.c_str();
	myfile.open(Filename);
	myfile << "\%Simulation of Scheduling in Real-Time Systems\n";
	myfile << "\\documentclass[13pt, a4paper, english]{report}\n";
	myfile << "\\usepackage[english]{babel}\n";
	myfile << "\\usepackage[latin1]{inputenc}\n";
	myfile << "\\usepackage{ times }\n";
	myfile << "\\usepackage[T1]{fontenc}\n";
	myfile << "\\usepackage{ rtsched }\n";
	myfile << "\\usepackage{ graphicx }\n";
	myfile << "\\usepackage{ xmpmulti }\n";
	myfile << "\\usepackage{ listings }\n";
	myfile << "\\usepackage{ pst - node }\n";
	myfile << "\\usepackage{ amsmath }\n";
	myfile << "\\usepackage{float}\n";
	myfile << "\\begin{document}\n";
	myfile << "\\begin{center}\n";
	myfile << "\\large{Simulation of Scheduling in Real-Time Systems}\n";
	myfile << "\\large{Model: " << name << " }\n";
	myfile << "\\end{center}\n";

}


Monitor::~Monitor()
{

}


bool Monitor::logTasks(Set<Task*> tasks)
{
	myfile << "\\begin{table}[H]\n";
	myfile << "\\caption{Tasks} \n";
	myfile << "\\centering\n";
	myfile << "\\begin{tabular}{|c|c|c|c|c|} \n";
	myfile << "\\hline \n";
	myfile << "Task number \& Arrival Time \& Execution Time \& Deadline \& Period \\\\ \\hline \n";
	AbstractIterator<Task*>* it = tasks.createIterator();
	int counter = 0;
	for (it->First(); !it->IsDone(); it->Next())
	{
		if (it->CurrentItem()->getPeriod() > 0)
		{
			counter++;
			if (counter > 40)
			{
				myfile << "\\end{tabular}\n";
				myfile << "\\end{table}\n";
				myfile << "\\begin{table}[H]\n";
				myfile << "\\caption{Tasks} \n";
				myfile << "\\centering\n";
				myfile << "\\begin{tabular}{|c|c|c|c|c|} \n";
				myfile << "\\hline \n";
				myfile << "Task number \& Arrival Time \& Execution Time \& Deadline \& Period \\\\ \\hline \n";
				counter = 0;
			}
			myfile << it->CurrentItem()->getID() << "\&";
			myfile << it->CurrentItem()->getTarrival() << "\&";
			myfile << it->CurrentItem()->getExecutionTime() << "\&";
			myfile << it->CurrentItem()->getDeadline() << "\&";
			myfile << it->CurrentItem()->getPeriod();
			myfile << "\\\\ \\hline \n";
			taskIDs.push_back(it->CurrentItem()->getID());
		}
	}
	for (it->First(); !it->IsDone(); it->Next())
	{
		if (it->CurrentItem()->getPeriod() == 0)
		{
			counter++;
			if (counter > 40)
			{
				myfile << "\\end{tabular}\n";
				myfile << "\\end{table}\n";
				myfile << "\\begin{table}[H]\n";
				myfile << "\\caption{Tasks} \n";
				myfile << "\\centering\n";
				myfile << "\\begin{tabular}{|c|c|c|c|c|} \n";
				myfile << "\\hline \n";
				myfile << "Task number \& Arrival Time \& Execution Time \& Deadline \& Period \\\\ \\hline \n";
				counter = 0;
			}
			myfile << it->CurrentItem()->getID() << "\&";
			myfile << it->CurrentItem()->getTarrival() << "\&";
			myfile << it->CurrentItem()->getExecutionTime() << "\&";
			myfile << it->CurrentItem()->getDeadline() << "\&";
			myfile << "Aperiodic";
			myfile << "\\\\ \\hline \n";
			taskIDs.push_back(it->CurrentItem()->getID());
		}
	}
	myfile << "\\end{tabular}\n";
	myfile << "\\end{table}\n";
	return true;
}


bool Monitor::logArrivalTime(Task arrTask, double arrTime)
{
	LogEvent myLogEvent(arrTask, arrTime, logARRIVED);
	LogEventQueue.addItem(myLogEvent);
	return true;
}

bool Monitor::logStart(Task startTaskId, double time)
{
	LogEvent myLogEvent(startTaskId, time, logSTARTED);
	LogEventQueue.addItem(myLogEvent);
	return true;
}


bool Monitor::logPause(Task pauseTaskId, double time)
{
	LogEvent myLogEvent(pauseTaskId, time, logBLOCKED);
	LogEventQueue.addItem(myLogEvent);
	return true;
}

bool Monitor::logDeadline(Task deadlineTask, double time)
{
	LogEvent myLogEvent(deadlineTask, time, logDEADLINE);
	LogEventQueue.addItem(myLogEvent);
	return true;
}

bool Monitor::logEnd(Task endTaskId, double time)
{
	LogEvent myLogEvent(endTaskId, time, logFINISHED);
	LogEventQueue.addItem(myLogEvent);

	return true;
}

bool Monitor::logDeadlineBreach(Task deadId, double time)
{
	LogEvent myLogEvent(deadId, time, logDEADLINEMISSED);
	LogEventQueue.addItem(myLogEvent);
	return true;
}


bool Monitor::logSimEnd(double time)
{
	LogEvent myLogEvent;
	myLogEvent.logTime = time;
	myLogEvent.logEventType = logSIMEND;
	LogEventQueue.addItem(myLogEvent);
	return true;
}


void Monitor::generateReport()
{
	//Initialize
	int iteration = 1;
	int interval = 50;
	double startTime = -1;
	double finishTime = 0;
	int CurrentlyExecutingTaskID = 0;
	int TaskSwitchCounter = 0;
	LogEvent myLogEvent;
	AbstractIterator<LogEvent>* it = LogEventQueue.createIterator();
	int numberOfTasks = taskIDs.size();


	//Find finishTime:
	for (it->First(); !it->IsDone(); it->Next())
	{
		if (it->CurrentItem().logEventType == logSIMEND)
		{
			finishTime = it->CurrentItem().logTime;
			break;
		}
	}

	calculateSimulatorStatistics(finishTime);

	//Caluculate how many RTboxes needed in the report
	double TotalNumberOfRTBoxes = ceil(finishTime / interval);

	//For each RTBox: Loop through the logEventQueue and find events that fits the time limits of the RTBox
	if (numberOfTasks <= 20 && finishTime <= 1000)
	{
		for (int RTBoxCounter = 0; RTBoxCounter < TotalNumberOfRTBoxes; RTBoxCounter++)
		{
			createRTBox(numberOfTasks, finishTime, iteration);
			startTime = -1;
			for (it->First(); !it->IsDone(); it->Next())
			{
				myLogEvent = it->CurrentItem();
				myLogEvent.logTime = (myLogEvent.logTime - (interval * RTBoxCounter));
				if (myLogEvent.logTime <= interval && myLogEvent.logTime >= 0)
				{
					switch (myLogEvent.logEventType)
					{
					case logARRIVED:
						onArrived(myLogEvent);
						break;

					case logSTARTED:
						//Store startTime for later use.
						startTime = myLogEvent.logTime;
						TaskSwitchCounter++;
						CurrentlyExecutingTaskID = myLogEvent.logTask.getID();
						break;

					case logBLOCKED:
						if (startTime == -1)
						{
							startTime = 0;
						}
						onBlocked(myLogEvent, startTime);
						startTime = -1;
						CurrentlyExecutingTaskID = 0;
						break;

					case logFINISHED:
						if (startTime == -1)
						{
							startTime = 0;
						}
						onFinished(myLogEvent, startTime);
						startTime = -1;
						CurrentlyExecutingTaskID = 0;
						break;

					case logDEADLINE:
						onDeadline(myLogEvent);
						break;

					case logDEADLINEMISSED:
						onDeadlineMissed(myLogEvent);
						break;

					default:
						break;
					}
				}
			}
			//If a task started executing in this RTBox, it has to continue in the next one:

			if (CurrentlyExecutingTaskID > 0)
			{
				//A task is currently being executed at the end of the RTBox.
				if (TaskSwitchCounter > 0)
				{
					if (((RTBoxCounter + 1)*interval) > finishTime)
					{
						Task tempTask(CurrentlyExecutingTaskID);
						LogEvent temp(tempTask, finishTime - (RTBoxCounter*interval), logBLOCKED);
						onBlocked(temp, startTime);
					}
					else
					{
						Task tempTask(CurrentlyExecutingTaskID);
						LogEvent temp(tempTask, interval, logBLOCKED);
						onBlocked(temp, startTime);
					}
				}
				else
				{
					Task tempTask(CurrentlyExecutingTaskID);
					LogEvent temp(tempTask, interval, logBLOCKED);
					onBlocked(temp, 0);
				}
				startTime = -1;
			}
			TaskSwitchCounter = 0;
			endRTBox();
			iteration++;
		}
	}
	myfile << "\\end{document}\n";
	myfile.close();
	runLatex();
}


void Monitor::onArrived(LogEvent log)
{
	myfile << "\\TaskArrival{" << log.logTask.getID() << "}{" << log.logTime << "}\n";
}

void Monitor::onStarted(LogEvent log)
{

}

void Monitor::onBlocked(LogEvent log, double time)
{
	myfile << "\\TaskExecution{" << log.logTask.getID() << "}{" << time << "}{" << log.logTime << "}\n";
}

void Monitor::onFinished(LogEvent log, double time)
{
	myfile << "\\TaskExecution{" << log.logTask.getID() << "}{" << time << "}{" << log.logTime << "}\n";
}

void Monitor::onDeadline(LogEvent log)
{
	myfile << "\\TaskDeadline{" << log.logTask.getID() << "}{" << log.logTime << "}\n";
}

void Monitor::onDeadlineMissed(LogEvent log)
{
	myfile << "\\TaskDeadlineMissed{" << log.logTask.getID() << "}{" << log.logTime << "}\n";
}


void Monitor::createRTBox(int numberOfTasks, double time, int iteration)
{
	int interval = 50;
	myfile << "\\def\\RTNumberOffset{" << (iteration - 1)*interval << "}\n";
	if (time < (iteration*interval))
	{
		myfile << "\\RTGridBegin[height=" << numberOfTasks << "cm,width=15cm,numbersize=6]{" << numberOfTasks << "}{" << (time - ((iteration - 1)*interval)) << "}\n";
	}
	else
	{
		myfile << "\\RTGridBegin[height=" << numberOfTasks << "cm,width=15cm,numbersize=6]{" << numberOfTasks << "}{" << 50 << "}\n";
	}
}

void Monitor::endRTBox()
{
	myfile << "\\RTGridEnd\n";
	myfile << "\\\\ . \\\\. \\\\\n";
}



void Monitor::runLatex()
{
	std::string latexString = "latex " + name + ".tex";
	const char* latex = latexString.c_str();

	std::string dvipsString = "dvips " + name + ".dvi";
	const char* dvips = dvipsString.c_str();

	std::string ps2pdfString = "ps2pdf " + name + ".ps";
	const char* ps2pdf = ps2pdfString.c_str();

	std::string pdfString = name + ".pdf";
	const char* pdf = pdfString.c_str();

	std::string moveString = MOVE + pdfString + " Results/";
	const char* move = moveString.c_str();

	std::string deleteAuxString = DELETE + name + ".aux";
	const char* deleteAux = deleteAuxString.c_str();

	std::string deleteDviString = DELETE + name + ".dvi";
	const char* deleteDvi = deleteDviString.c_str();

	std::string deleteLogString = DELETE + name + ".log";
	const char* deleteLog = deleteLogString.c_str();

	std::string deletePsString = DELETE + name + ".ps";
	const char* deletePs = deletePsString.c_str();

	std::string deleteTexString = DELETE + name + ".tex";
	const char* deleteTex = deleteTexString.c_str();

	std::string openPdfString = name + ".pdf";
	const char* openPdf = openPdfString.c_str();


	system(latex);
	system(dvips);
	system(ps2pdf);
	system(deleteAux);
	system(deleteDvi);
	system(deleteLog);
	system(deletePs);
	system(deleteTex);
	//system(openPdf);
	system(move);

}


void Monitor::calculateSimulatorStatistics(double time)
{
	//Initializing 
	std::vector<double> cpuUsage;

	double sumTurnAroundTime = 0;
	double tempTurnAroundTime = 0;
	double totAverageTurnAroundTime = 0;
	double unusedCpu = CPUidleTime();
	int numberOfTasks = taskIDs.size();
	double numberOfCompletedTasks = numberOfTasksCompleted();
	double numberOfReachedDeadlines = numberOfDeadlinesReached();
	double numDeadlinesMissed = numberOfDeadlinesMissed();

	myfile << "\\begin{table}[H]\n";
	myfile << "\\caption{Average Turnaround Times} \n";
	myfile << "\\centering\n";
	myfile << "\\begin{tabular}{|c|c|} \n";
	myfile << "\\hline \n";
	myfile << "Task \& Average Turnaround Time \\\\ \\hline \n";
	for (std::vector<int>::iterator IdIt = taskIDs.begin(); IdIt != taskIDs.end(); ++IdIt)
	{
		tempTurnAroundTime = AverageTurnAroundTime(*IdIt);
		sumTurnAroundTime += tempTurnAroundTime;
		if (tempTurnAroundTime == 0)
		{
			myfile << *IdIt << " \&  No Value \\\\ \\hline \n";
		}
		else
		{
			myfile << *IdIt << " \& " << tempTurnAroundTime << "\\\\ \\hline \n";
		}
	}

	totAverageTurnAroundTime = (sumTurnAroundTime / numberOfCompletedTasks);

	if (totAverageTurnAroundTime > 0)
	{
		myfile << "Total Average Turnaround Time \&" << totAverageTurnAroundTime << "\\\\ \\hline \n";
	}
	else
	{
		myfile << "Total Average Turnaround Time \& No Value \\\\ \\hline \n";
	}
	myfile << "\\end{tabular}\n";
	myfile << "\\end{table}\n";

	myfile << "\\begin{table}[H]\n";
	myfile << "\\caption{Simulation Results in Numbers} \n";
	myfile << "\\centering\n";
	myfile << "\\begin{tabular}{|c|c|} \n";
	myfile << "\\hline \n";
	myfile << "CPU Idle Time \&" << unusedCpu << "\\\\ \\hline \n";
	myfile << "Number of Deadlines reached \&" << numberOfReachedDeadlines << "\\\\ \\hline \n";
	myfile << "Number of Deadlines missed \&" << numDeadlinesMissed << "\\\\ \\hline \n";
	myfile << "Number of Completed tasks \&" << numberOfCompletedTasks << " \\\\ \\hline \n";
	myfile << "Number of Tasks \&" << numberOfTasks << "\\\\ \\hline \n";
	myfile << "\\end{tabular}\n";
	myfile << "\\end{table}\n";

	myfile << "\\begin{table}[H]\n";
	myfile << "\\caption{Simulation Results in Percentage} \n";
	myfile << "\\centering\n";
	myfile << "\\begin{tabular}{|c|c|} \n";
	myfile << "\\hline \n";
	myfile << "CPU Idle Time \&" << (unusedCpu / time) * 100 << "\\% \\\\ \\hline \n";
	if (numDeadlinesMissed > 0)
	{
		myfile << "Deadlines missed \&" << (numDeadlinesMissed / (numberOfReachedDeadlines+numDeadlinesMissed)) * 100 << "\\% \\\\ \\hline \n";
	}
	else
	{
		myfile << "Deadlines missed \& 0 \\% \\\\ \\hline \n";
	}
	myfile << "Completed tasks \&" << (numberOfCompletedTasks / numberOfTasks) * 100 << "\\% \\\\ \\hline \n";
	myfile << "\\end{tabular}\n";
	myfile << "\\end{table}\n";
}

double Monitor::CPUidleTime()
{
	double CPUIdle = 0.0;
	double LastEndTime = 0.0;
	AbstractIterator<LogEvent>* it = LogEventQueue.createIterator();

	for (it->First(); !it->IsDone(); it->Next())
	{
		if (it->CurrentItem().logEventType == logSTARTED)
		{
			if (LastEndTime != it->CurrentItem().logTime)
			{
				CPUIdle += it->CurrentItem().logTime - LastEndTime;
			}
		}
		else if (it->CurrentItem().logEventType == logBLOCKED || it->CurrentItem().logEventType == logFINISHED)
		{
			LastEndTime = it->CurrentItem().logTime;
		}
	}
	return CPUIdle;
}


double Monitor::numberOfDeadlinesReached()
{
	double numberOfDeadlinesReached = 0.0;
	int MissedDeadlinesCounter = 0;

	AbstractIterator<LogEvent>* it = LogEventQueue.createIterator();
	for (std::vector<int>::iterator IDit = taskIDs.begin(); IDit != taskIDs.end(); ++IDit)
	{
		for (it->First(); !it->IsDone(); it->Next())
		{
			if (it->CurrentItem().logTask.getID() == *IDit)
			{
				switch (it->CurrentItem().logEventType)
				{
				case logFINISHED:
					if (MissedDeadlinesCounter==0)
					{
						numberOfDeadlinesReached += 1;
					}
					else
					{
						MissedDeadlinesCounter--;
					}
					break;
				case logDEADLINEMISSED:
					MissedDeadlinesCounter++;
					break;
				default:
					break;
				}
			}
		}
	}
	return numberOfDeadlinesReached;
}

double Monitor::numberOfDeadlinesMissed()
{
	double deadlinesMissed = 0.0;

	AbstractIterator<LogEvent>* it = LogEventQueue.createIterator();
	for (it->First(); !it->IsDone(); it->Next())
	{
		if (it->CurrentItem().logEventType == logDEADLINEMISSED)
		{
			deadlinesMissed += 1;
		}
	}
	return deadlinesMissed;
}

double Monitor::AverageTurnAroundTime(int taskid)
{
	double ExecutionCounter = 0.0;
	double sumTurnAroundTime = 0.0;
	double ArrivalTime = 0.0;
	AbstractIterator<LogEvent>* it = LogEventQueue.createIterator();
	for (it->First(); !it->IsDone(); it->Next())
	{
		if (it->CurrentItem().logTask.getID() == taskid)
		{
			if (it->CurrentItem().logEventType == logARRIVED)
			{
				ArrivalTime = it->CurrentItem().logTime;
			}
			else if (it->CurrentItem().logEventType == logFINISHED)
			{
				sumTurnAroundTime += it->CurrentItem().logTime - ArrivalTime;
				ExecutionCounter += 1;
			}
		}
	}
	if (sumTurnAroundTime > 0)
	{
		return (sumTurnAroundTime / ExecutionCounter);
	}
	else
	{
		return 0;
	}
}

double Monitor::numberOfTasksCompleted()
{

	std::vector<int> CompletedTasksIDs;
	double CompletedTasksCounter = 0.0;
	bool CompletedFlag = false;

	AbstractIterator<LogEvent>* it = LogEventQueue.createIterator();

	for (it->First(); !it->IsDone(); it->Next())
	{
		if (it->CurrentItem().logEventType == logFINISHED)
		{
			//If task is not completed before, add to completedTasksIDs
			for (std::vector<int>::iterator IdIt = CompletedTasksIDs.begin(); IdIt != CompletedTasksIDs.end(); ++IdIt)
			{
				if (*IdIt == it->CurrentItem().logTask.getID())
				{
					CompletedFlag = true;
					break;
				}
			}
			if (CompletedFlag == false)
			{
				CompletedTasksIDs.push_back(it->CurrentItem().logTask.getID());
				CompletedTasksCounter += 1;
			}
			else
			{
				CompletedFlag = false;
			}
		}
	}
	return CompletedTasksCounter;
}

