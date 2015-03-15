#pragma once

#include <iostream>
#include <sstream>

#include <KLib/Config.hpp>

namespace klib
{

class API_EXPORT ISerializable
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Serialize object into a string stream
	///
	/// Subclasses implement this with their own serialization
	///
	/// \code
	/// void MyClass::Serialize(std::ostringstream& out) const
	/// {
	///     ISerializable* object = new Object(123);
	///     out << object << mArray;
	/// }
	/// \endcode
	///
	/// Note: It may be required to explicitly type-cast to an
	/// ISerializable*
	///
	/// \param out String to serialize into
	///
	///////////////////////////////////////////////////////////
	virtual void Serialize(std::ostringstream& out) const = 0;

	///////////////////////////////////////////////////////////
	/// \brief Deserialize object from a string
	///
	/// Subclasses implement this with their own serialization
	/// \see Serialize
	///
	/// \param in String to deserialize from
	///
	///////////////////////////////////////////////////////////
	virtual void Deserialize(std::istringstream& in) = 0;
};
///////////////////////////////////////////////////////////
/// \interface ISerializable
/// \brief Interface for serializing objects to and from a string
///
/// Uses std::stringstream, which acts the same way as std::iostream
/// but automatically handles string conversions, and has some extra flags.
///
/// Subclasses are required to implement Serialize and Deserialize
/// methods in any way they want.
///
///////////////////////////////////////////////////////////

} // klib