#pragma once

#include <atomic>

#include <KLib/Config.hpp>
#include <KLib/String.hpp> // ::ToHexString
#include <KLib/Logging.hpp>
#include <KLib/Threading.hpp> //convenience & to avoid #include thread, threading

namespace klib
{

class API_EXPORT Thread
{
public:
	///////////////////////////////////////////////////////////
	/// \biref Default Constructor
	///
	/// Creates a new thread object: not an actual thread, but
	/// a wrapper for one. (C++11)
	///
	///////////////////////////////////////////////////////////
	Thread() : mRunning(false), mDetached(false) {};
	
	///////////////////////////////////////////////////////////
	/// \brief Default Destructor
	///
	/// Gracefully stops a thread, detaching it if's still running
	///
	///////////////////////////////////////////////////////////
	virtual ~Thread()
	{
		if (mRunning)
		{
			mRunning = false;
			if (!mDetached)
				Detach();
		}
	}

	///////////////////////////////////////////////////////////
	/// \brief Start the thread
	///
	/// Checks if the thread is already running or not, and
	/// returns false if it is. If the thread has been started
	/// successfully, returns true.
	///
	/// \return Started succesfully
	///
	/// \see Thread::Run
	///
	///////////////////////////////////////////////////////////
	inline bool Start()
	{
		if (mRunning)
		{
			KL_WARNING("Attempted to Start() a Thread which is already running");

			return false;
		}
		else
		{
			mRunning = true;
			mThread = std::thread(&Thread::Run, this);

			KL_WARNING("Thread", "Created a new thread (" + ToHexString(mThread.get_id().hash()) + ")");

			return true;
		}
	}
	
	///////////////////////////////////////////////////////////
	/// \brief Stop the thread
	///
	/// The param immediate controls whether the thread will
	/// be left to finish it's Run() function detaching it, or
	/// forcibly joining it.
	///
	/// \param immediate Stop thread immediately or leave to exit
	///
	///////////////////////////////////////////////////////////
	inline void Stop(bool immediate = false)
	{
		if (mRunning)
		{
			mRunning = false;

			if (immediate)
			{
				Detach();
			}
			else
			{
				Join();
			}
		}
		else
		{
			KL_WARNING("Attempted to Stop() a Thread which isn't running");
		}
	}
	
	///////////////////////////////////////////////////////////
	/// \brief Join thread to the thread it was created on
	///
	/// Stops execution on caling thread and makes it wait until
	/// this thread finishes execution.
	///
	/// \return Joined
	///
	///////////////////////////////////////////////////////////
	inline bool Join()
	{
		if (mRunning)
		{
			mThread.join();
			mDetached = false;
			return true;
		}
		return false;
	}

	
	///////////////////////////////////////////////////////////
	/// \brief Detach a thread
	///
	/// Detach this thread from the thread it was created on,
	/// causing them to lose references of eachother.
	///
	/// \return Detached
	///
	///////////////////////////////////////////////////////////
	inline bool Detach()
	{
		if (mRunning && !mDetached)
		{
			mThread.detach();
			mDetached = true;
			return true;
		}
		return false;
	}
	
	///////////////////////////////////////////////////////////
	/// \brief Get the thread id as a std::thread::id
	///
	/// \return Thread ID
	///
	///////////////////////////////////////////////////////////
	inline std::thread::id GetThreadID() const
	{
		return mThread.get_id();
	}

	///////////////////////////////////////////////////////////
	/// \brief Check if thread is running
	///
	/// \return Running
	///
	///////////////////////////////////////////////////////////
	bool IsRunning(void) const
	{
		return mRunning;
	}

	///////////////////////////////////////////////////////////
	/// \brief Thread entry-point/Thread function
	///
	/// This is the function that will be run on the actual
	/// thread, so anything which is supposed to be threaded
	/// should be in this function.
	///
	/// The function can contain a loop, the thread should either
	/// clean up itself, or be cleaned up by a manager of it.
	///
	/// Otherwise, if the function is not a loop, or finishes
	/// on it's own, the thread will simply terminate while the
	/// Thread object remains.
	///
	/// For a loop, the loop should continue as long as mRunning
	/// is true. For function which is not a loop, it should
	/// set mRunning to false when it exists.
	///
	///////////////////////////////////////////////////////////
	virtual void* Run() = 0;
	
protected:
	std::thread mThread;
	std::atomic<bool> mRunning;
	std::atomic<bool> mDetached;
};
///////////////////////////////////////////////////////////
/// \class Thread
/// \brief Baseclass for threads with a nice interface.
///
/// The subclass which is intended to be threaded, should
/// overload the Run method with it's own routine.
/// See Thread::Run for a detailed explanation.
///
/// Accessing the thread from outside (Start, Stop, IsRunning)
/// is thread-safe because of the use of atomics
///
/// Example usage in subclass:
/// \code
/// class ThreadedIO : Thread
/// {
///     void* Run() override;
/// }
///
/// void* ThreadedIO::Run()
/// {
///     while (mRunning)
///     {
///          ProcessQueueAsync();
///     }
/// }
///
/// ThreadedIO tio;
/// tio.Start();
/// ...
/// tio.Stop();
/// \endcode
///
///////////////////////////////////////////////////////////

} // klib