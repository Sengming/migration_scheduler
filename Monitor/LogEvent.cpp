#include "LogEvent.h"


LogEvent::LogEvent()
{
	logTime = 0.0;
	logEventType = 0;
}

LogEvent::LogEvent(double mytime, int type)
{
	logTime = mytime;
	logEventType = type;
}

LogEvent::LogEvent(Task myTask, double mytime, int type)
{
	logTask = myTask;
	logTime = mytime;
	logEventType = type;
}

LogEvent::~LogEvent()
{
}


bool LogEvent::operator==(const LogEvent& b)
{
	Task tempTask = (b.logTask);
	int tempId = tempTask.getID();
	if ((logTask.getID()) == tempId && logTime == b.logTime && logEventType == b.logEventType)
	{
		return true;
	}
	return false;
}