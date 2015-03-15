#pragma once

#include <KLib/Config.hpp>
#include <KLib/String.hpp>
#include <KLib/Logging.hpp>

namespace klib
{
//namespace utilities
//{

template <class T>
class API_EXPORT Singleton
{
private:

	static T* pInstance;

protected:

	explicit Singleton()
	{
		KL_ERROR("Singleton instancized incorrectly");
	}

	/////////////////////////////////////////////////
	/// \brief Default constructor
	///
	/// Creates pointer to instance and checks for instance collision
	///
	/// \param instance Pointer to templated type
	/// \param name Optional identifier of singleton
	///
	/////////////////////////////////////////////////
	explicit Singleton(T* instance, String name = "Unidentified")
	{
		if (pInstance)
		{
			KL_ERROR("Attempting to create two instances of a singleton '" + name + "'");
			delete pInstance;
		}
		pInstance = instance;
	}

	/////////////////////////////////////////////////
	/// \brief Default destructor
	///
	/// Removes pointer to instance
	///
	/////////////////////////////////////////////////
	virtual ~Singleton()
	{
		if (pInstance)
		{
			pInstance = nullptr;
		}
	}

public:
	/////////////////////////////////////////////////
	/// \brief Returns singleton instance
	///
	/// Returns the templated type (singleton type)
	/// Asserts that the pointer exists and is not null
	///
	/// \return Singleton instance
	/////////////////////////////////////////////////
	inline static T* Get()
	{
		if (pInstance == nullptr)
		{
			new T();
		}
		KL_ASSERT(pInstance);
		return pInstance;
	}

	/////////////////////////////////////////////////
	/// \class Singleton
	///
	/// Template base class for singletons
	/// The template is the instance class/type
	/// \code
	/// class Instance : Singleton<Instance>
	///
	/// Instance::Instance();
	/// Instance* instance = Instance::Get();
	/// \endcode
	/////////////////////////////////////////////////
};

template<class T>
T* Singleton<T>::pInstance = nullptr;

//} // utilities
} // klib