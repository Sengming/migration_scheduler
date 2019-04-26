#pragma once
#include"../SimulatorCore/Task.h"

/******************************************************************************************
The LogEvent represents an event that has been logged by the 
Monitor.

logTask represents the task the logEvent concerns.

logTime is the time of the logEvent.

logEventType is the type of event. The different LogEventTypes are:
logARRIVED, logSTARTED, logBLOCKED, logFINISHED, logDEADLINE, logDEADLINEMISSED, logSIMEND
*******************************************************************************************/

enum logEvent{logARRIVED, logSTARTED, logBLOCKED, logFINISHED, logDEADLINE, logDEADLINEMISSED, logSIMEND};


class LogEvent
{
public:
	Task logTask;
	double logTime;
	int logEventType;

	LogEvent();
	LogEvent(Task myTask, double mytime, int type);
	LogEvent(double mytime, int type);
	~LogEvent();
	
	bool operator==(const LogEvent& b);
	bool operator<(const LogEvent& first)
	{
		return (logTime < first.logTime);
	}

};

