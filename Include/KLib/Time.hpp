#pragma once

#include <chrono>

#include <KLib/Exports.hpp>
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
String API_EXPORT GetTimeAsString(String formatString, time_t time);

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
ULong API_EXPORT GetCurrentTime(void)
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
ULong API_EXPORT GetElapsedTime(void)
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
ULong API_EXPORT GetStartTime(void)
{
	return duration_cast<T>( timeStarted ).count();
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
String API_EXPORT ToString(system_clock::time_point timePoint, String format = "%a, %d %b %Y %T %Z");



template<typename T = milliseconds>
class API_EXPORT Timer
{
public:
	Timer(String id = "unidentified") : mTimeCount(0) {};

	void Start(void)
	{
		mStartTime = time::GetCurrentTime<T>();
		mLast = mStartTime;
		mRunning = true;
	}

	void Stop(void)
	{
		Update();
		mRunning = false;
	}

	void Reset(void)
	{
		mTimeCount = 0;
		mRunning = false;
	}

	ULong GetElapsedTime(void)
	{
		Update();
		return mTimeCount;
	}

	bool IsRunning(void) const
	{
		return mRunning;
	}

private:
	void Update()
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