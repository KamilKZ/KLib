#pragma once

//TODO thread safe (dont allow logging as well as adding new listeners etc.)

#include <KLib/Exports.hpp>
#include <KLib/Singleton.hpp>
#include <KLib/Number.hpp>
#include <KLib/String.hpp>
#include <KLib/Memory.hpp>
//#include <KLib/TextFile.hpp>

namespace klib
{
namespace logging
{

enum LogLevel
{
	DEBUGGING 	= 0, 	// Arbitrary 'print' debugging, for testing only.
	INFO 		= 1, 	// Useful information but not always important.
	WARNING		= 2,	// Warnings are recoverable, should not cause errors.
	ERROR 		= 3,	// Errors are potentially fatal and should be dealt with.
	FATAL 		= 4		// Fatal errors stop execution.
};

struct LogEntry
{
	LogLevel level; //Severity of error
	String message; //Message of error, 'body'
	ULong timestamp;
	
	String sourceFunction;
	String sourceFile;
	UInt sourceLine;
};
	
class LogListener
{
public:
	LogListener() { mEnabled = true; }

	virtual bool Log(LogEntry entry) = 0;

	bool IsEnabled() const { return mEnabled; }
	void SetEnabled(bool enabled) { mEnabled = enabled; }

private:
	bool mEnabled;
};

class Logger : public Singleton<Logger>
{
public:
	Logger(){};
	virtual ~Logger(void);
	
	void Log(LogLevel level, String message, String func, String file, UInt line);
	
private:
	ArrayList<StrongPtr<LogListener>> mListeners;
};

class ErrorMessenger : public LogListener
{
public:
	virtual bool Log(LogEntry entry) = 0;
};

class LogFileWriter : public LogListener
{
public:
	LogFileWriter(String logFileName);
	
	virtual ~LogFileWriter(void);
	
	virtual bool Log(LogEntry entry) = 0;
	
private:
	//TextFile logFile;
};

} // logging

static logging::Logger DefaultLogger;

} // klib


#define KL_LOG(level, str) \
do \
{ \
	klib::DefaultLogger.Log(level, str, __FUNCTION__, __FILE__, __LINE__); \
} \
while (0)

#define KL_DEBUGLOG(str) \
do \
{ \
	klib::DefaultLogger.Log(klib::logging::LogLevel::DEBUGGING, str, __FUNCTION__, __FILE__, __LINE__); \
} \
while (0)

#define KL_INFO(str) \
do \
{ \
	klib::DefaultLogger.Log(klib::logging::LogLevel::INFO, str, __FUNCTION__, __FILE__, __LINE__); \
} \
while (0)

#define KL_WARNING(str) \
do \
{ \
	klib::DefaultLogger.Log(klib::logging::LogLevel::WARNING, str, __FUNCTION__, __FILE__, __LINE__); \
} \
while (0)

#define KL_ERROR(str) \
do \
{ \
	klib::DefaultLogger.Log(klib::logging::LogLevel::ERROR, str, __FUNCTION__, __FILE__, __LINE__); \
} \
while (0)

#define KL_FATAL(str) \
do \
{ \
	klib::DefaultLogger.Log(klib::logging::LogLevel::FATAL, str, __FUNCTION__, __FILE__, __LINE__); \
} \
while (0)
