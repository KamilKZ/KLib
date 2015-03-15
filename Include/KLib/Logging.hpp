#pragma once

//TODO thread safe (dont allow logging as well as adding new listeners etc.)

#include <KLib/Config.hpp>
#include <KLib/Number.hpp>
#include <KLib/String.hpp>
#include <KLib/Memory.hpp>
#include <KLib/File.hpp>

namespace klib
{

namespace logging
{

enum LogLevel
{
	DEBUG_LOG 	= 0, 	// Arbitrary 'print' debugging, for testing only.
	INFO_LOG 	= 1, 	// Useful information but not always important.
	WARNING_LOG	= 2,	// Warnings are recoverable, should not cause errors.
	ERROR_LOG 	= 3,	// Errors are potentially fatal and should be dealt with.
	FATAL_LOG 	= 4		// Fatal errors stop execution.
};

struct LogEntry
{
	LogLevel level; //Severity of error
	String message; //Message of error, 'body'
	ULong timestamp;
	
	String sourceFunction;
	String sourceFile;
	UInt sourceLine;

	String ToString(void) const
	{
		String levelStr("Unkown");
		switch (level)
		{
			case DEBUG_LOG: levelStr = "DEBUG"; break;
			case INFO_LOG: levelStr = "INFO"; break;
			case WARNING_LOG: levelStr = "WARNING"; break;
			case ERROR_LOG: levelStr = "ERROR"; break;
			case FATAL_LOG: levelStr = "FATAL"; break;
		}
		return klib::ToString(timestamp) + " [" + levelStr + "] " + message;
	};
};
	
class LogListener
{
public:
	LogListener() : mEnabled(true) {}

	virtual bool Log(LogEntry entry) = 0;

	bool IsEnabled() const { return mEnabled; }
	void SetEnabled(bool enabled) { mEnabled = enabled; }

private:
	bool mEnabled;
};

class Logger
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
	klib::io::TextFile logFile;
};

} // logging

static logging::Logger DefaultLogger;

} // klib


#define KL_LOG(level, str) \
do \
{ \
	String s(str); \
	klib::logging::LogLevel l(level); \
	klib::DefaultLogger.Log(l, s, __FUNCTION__, __FILE__, __LINE__); \
} \
while (0) \

#define KL_DEBUGLOG(str) \
do \
{ \
	String s(str); \
	klib::DefaultLogger.Log(klib::logging::LogLevel::DEBUG_LOG, s, __FUNCTION__, __FILE__, __LINE__); \
} \
while (0) \

#define KL_INFO(str) \
do \
{ \
	String s(str); \
	klib::DefaultLogger.Log(klib::logging::LogLevel::INFO_LOG, s, __FUNCTION__, __FILE__, __LINE__); \
} \
while (0) \

#define KL_WARNING(str) \
do \
{ \
	String s(str); \
	klib::DefaultLogger.Log(klib::logging::LogLevel::WARNING_LOG, s, __FUNCTION__, __FILE__, __LINE__); \
} \
while (0) \

#define KL_ERROR(str) \
do \
{ \
	String s(str); \
	klib::DefaultLogger.Log(klib::logging::LogLevel::ERROR_LOG, s, __FUNCTION__, __FILE__, __LINE__); \
} \
while (0) \

#define KL_FATAL(str) \
do \
{ \
	String s(str); \
	klib::DefaultLogger.Log(klib::logging::LogLevel::FATAL_LOG, s, __FUNCTION__, __FILE__, __LINE__); \
} \
while (0) \

#define KL_ASSERT(expr) \
do \
{ \
if (!(expr)) \
{ \
	String s(#expr); \
	klib::DefaultLogger.Log(klib::logging::LogLevel::ERROR_LOG, s, __FUNCTION__, __FILE__, __LINE__); \
} \
} \
while (0) \

