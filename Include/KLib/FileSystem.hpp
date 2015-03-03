#pragma once

#include <KLib/Exports.hpp>
#include <KLib/String.hpp>

namespace klib
{
namespace filesystem
{

/////////////////////////////////////////////////
/// \namespace klib::filesystem
/// \defgroup FileSystem
/// \ingroup FileSystem
///
/// A set of functions and classes for manipulating
/// files and directories
///
/////////////////////////////////////////////////

class API_EXPORT Path
{
public:
	static ArrayList<String> Split(const String& path);
};

class API_EXPORT File
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Check if a file exists
	///
	/// Checks if a file exists.
	/// Exclusive: doesn't work on directories.
	///
	/// \param path Path to file
	///
	/// \return True if file exists, false otherwise
	///
	///////////////////////////////////////////////////////////
	static bool Exists(const String& path);

	///////////////////////////////////////////////////////////
	/// \brief Delete a file
	///
	/// Attempts to delete a file
	///
	/// \param path Path to file
	///
	/// \return True is successful, false otherwise
	///
	///////////////////////////////////////////////////////////
	static bool Delete(const String& path);
};

class API_EXPORT Dir
{
public:
	///////////////////////////////////////////////////////////
	/// \brief Check if path is a directory
	///
	/// Note: this may also be used as 'Dir::Exists', because
	/// it works the same way.
	///
	/// \param path Path to directory
	///
	/// \return True if exists, false otherwise
	///
	///////////////////////////////////////////////////////////
	static bool IsDir(const String& path);

	///////////////////////////////////////////////////////////
	/// \brief Create a directory
	///
	/// Attempts to create a directory
	///
	/// \param path Path to directory
	///
	/// \return True if successful, false otherwise
	///
	///////////////////////////////////////////////////////////
	static bool Create(const String& path);

	///////////////////////////////////////////////////////////
	/// \brief Read directory entries in a directory
	///
	/// Returns a list of files and folders in a directory
	/// If directory cannot be found or read, returns an empty
	/// array instead.
	///
	/// The list contains paths relative from the executable.
	///
	/// \param path Path to directory
	///
	/// \return String array of directory entries
	///
	///////////////////////////////////////////////////////////
	static ArrayList<String> Read(const String& path);

	///////////////////////////////////////////////////////////
	/// \brief Find files in a directory
	///
	/// Uses pattern match to find files. 
	/// A pattern '*.txt' matches all .txt files,
	/// '*.*' matches all files in the directory,
	/// '**.*' matches all files in the directory and subdirectories.
	///
	/// The list contains paths relative from the executable.
	///
	/// \param pattern Find files by this pattern
	///
	/// \return String array of files
	///
	///////////////////////////////////////////////////////////
	static ArrayList<String> Find(const String& pattern);
};

///////////////////////////////////////////////////////////
/// \brief Check if a file OR directory exists
///
/// \param path Path to check
///
/// \return True if file or directory exists, false otherwise
///
///////////////////////////////////////////////////////////
bool API_EXPORT Exists(const String& path);

} // filesystem
} // klib