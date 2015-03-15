#pragma once

#include <chrono>

#include <KLib/Config.hpp>
#include <KLib/Number.hpp>
#include <KLib/String.hpp>

namespace klib
{
namespace time
{

using namespace std::chrono;

///////////////////////////////////////////////////////////////
/// \brief Format time into a string
///
/// \param formatString String to use when formatting
/// \param time Time to format into a string
///
/// \return Formatted time as string
///
///////////////////////////////////////////////////////////////
inline API_EXPORT String FormatT(String formatString, time_t time)
{
	struct tm *timeinfo(localtime(&time));

	formatString += '\a'; //force at least one character in the result
	std::string buffer;
	buffer.resize(formatString.size());
	int len = strftime(&buffer[0], buffer.size(), formatString.c_str(), timeinfo);
	while (len == 0) {
		buffer.resize(buffer.size() * 2);
		len = strftime(&buffer[0], buffer.size(), formatString.c_str(), timeinfo);
	}
	buffer.resize(len - 1); //remove that trailing '\a'
	return buffer;
}

///////////////////////////////////////////////////////////////
/// \brief Format a time point
///
/// Format a time_point into a string.
/// Default format is "%a, %d %b %Y %T %Z"; WkDay, Month Day Year H:M:S Timezone,
/// For example, Mon, Jun 23 2014 23:56:28 GMT.
///
/// See *link* for more options
///
/// \param timePoint An std::chrono::system_clock::time_point to format
/// \param format String format pattern to format by
///
/// \return Formatted time
///
///////////////////////////////////////////////////////////////
inline API_EXPORT String Format(system_clock::time_point timePoint, String format = "%a, %d %b %Y %T %Z")
{
	return FormatT(format, system_clock::to_time_t(timePoint));
	//WkDay, Month Day Year H:M:S Timezone
	//Mon, Jun 23 2014 23:56:28 GMT
}

///////////////////////////////////////////////////////////////
/// \brief The time since the unix epoch, in miiliseconds
///
/// Can be called with or without template parameters,
/// the default is nanoseconds
///
/// \param template An std::chrono::duration defining the type to cast to, default is nanoseconds
///
/// \return Time in miiliseconds
///
///////////////////////////////////////////////////////////////
template<typename T = nanoseconds>
inline API_EXPORT ULong GetCurrentTime(void)
{
	return duration_cast<T>( high_resolution_clock::now().time_since_epoch() ).count();
}

///////////////////////////////////////////////////////////////
/// \brief The time since the game stated, in nanoseconds
///
/// Can be called with or without template parameters,
/// the default is nanoseconds
///
/// \param template An std::chrono::duration defining the type to cast to, default is nanoseconds
///
/// \return Time in nanoseconds
///
///////////////////////////////////////////////////////////////
template<typename T = nanoseconds>
inline API_EXPORT ULong GetElapsedTime(void)
{
	return duration_cast<T>( high_resolution_clock::now().time_since_epoch() - timeStarted.time_since_epoch() ).count();
}

///////////////////////////////////////////////////////////////
/// \brief The time when the game stated, in nanoseconds
///
/// Can be called with or without template parameters,
/// the default is nanoseconds
///
/// \param template An std::chrono::duration defining the type to cast to, default is nanoseconds
///
/// \return Time in nanoseconds
///
///////////////////////////////////////////////////////////////
template<typename T = nanoseconds>
inline API_EXPORT ULong GetStartTime(void)
{
	return duration_cast<T>( timeStarted ).count();
}

template<typename T = milliseconds>
class API_EXPORT Timer
{
public:
	Timer(String id = "unidentified") : mTimeCount(0) {};

	inline void Start(void)
	{
		mStartTime = time::GetCurrentTime<T>();
		mLast = mStartTime;
		mRunning = true;
	}

	inline void Stop(void)
	{
		Update();
		mRunning = false;
	}

	inline void Reset(void)
	{
		mTimeCount = 0;
		mRunning = false;
	}

	inline ULong GetElapsedTime(void)
	{
		Update();
		return mTimeCount;
	}

	inline bool IsRunning(void) const
	{
		return mRunning;
	}

private:
	inline void Update()
	{
		if (mRunning) // only if running
		{
			mTimeCount += ( time::GetCurrentTime<T>() - mLast );
			mLast = time::GetCurrentTime<T>();
		}
	}

	bool mRunning;
	ULong mStartTime;
	ULong mLast;
	ULong mTimeCount;
};

} // time
} // te