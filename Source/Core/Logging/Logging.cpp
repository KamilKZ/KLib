
#include <KLib/Time.hpp>
#include <KLib/Logging.hpp>
#include <KLib/File.hpp>

namespace klib
{
namespace logging
{

void Logger::Log(LogLevel level, String message, String func, String file, UInt line)
{
	LogEntry entry;
	entry.level = level;
	entry.message = message;
	entry.sourceFunction = func;
	entry.sourceFile = file;
	entry.sourceLine = line;
	entry.timestamp = klib::time::GetCurrentTime<klib::time::nanoseconds>();

	for (auto it = mListeners.begin(); it != mListeners.end(); it++)
	{
		if (!it->get()->Log(entry))
		{
			//is this ever valid?
			break;
		}
	}
}

bool ErrorMessenger::Log(LogEntry entry)
{
	if (entry.level >= LogLevel::ERROR_LOG)
	{
		//display error box
		return false;
	}
}

LogFileWriter::LogFileWriter(String logFileName)
{
	if (logFile.Open(logFileName, std::ios::app))
	{
		logFile.Seek(logFile.GetSize());
	}
	else
	{
		KL_FATAL("Logging file could not be opened or created");
	}
}

LogFileWriter::~LogFileWriter(void)
{
	logFile.Close();
}

bool LogFileWriter::Log(LogEntry entry)
{
	//TODO Async
	logFile.Write(entry.ToString());
	logFile.Flush();

	return true;
}

} // logging
} // klib

