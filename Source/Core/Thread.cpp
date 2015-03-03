#include <KLib/String.hpp> // ::ToHexString
#include <KLib/Logging.hpp>
#include <KLib/Thread.hpp>

namespace klib
{

Thread::Thread() : mRunning(false), mDetached(false) {};

Thread::~Thread()
{
	if (mRunning)
	{
		mRunning = false;
		if (!mDetached)
			Detach();
	}
}

bool Thread::Start()
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

void Thread::Stop(bool immediate)
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

bool Thread::Join()
{
	if (mRunning)
	{
		mThread.join();
		mDetached = false;
		return true;
	}
	return false;
}

bool Thread::Detach()
{
	if (mRunning && !mDetached)
	{
		mThread.detach();
		mDetached = true;
		return true;
	}
	return false;
}

std::thread::id Thread::GetThreadID()
{
	return mThread.get_id();
}

} // klib