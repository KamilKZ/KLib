#ifdef _WIN32
#	define WIN32_LEAN_AND_MEAN
#	define NOMINMAX
#	include <windows.h>
#endif

#include <deps/dirent.h>
#include <iostream>
#include <sstream>

#include <KLib/Logging.hpp>
#include <KLib/ArrayList.hpp>
#include <KLib/File.hpp>
#include <KLib/FileSystem.hpp>

namespace klib
{
namespace filesystem
{

ArrayList<String> Path::Split(const String& path)
{
	ArrayList<String> elems;
	std::stringstream ss(path);
	String item;

	while (std::getline(ss, item, '/'))
	{
		elems.push_back(item);
	}
	return elems;
}

bool File::Exists(const String& path)
{
	klib::io::BinaryFile file(path, klib::io::FileModes::Read);
	return file.IsOpen(); // destructor closes handle
}

bool Dir::IsDir(const String& path)
{
	DWORD fa = GetFileAttributesA(path.c_str());
	if (fa == INVALID_FILE_ATTRIBUTES)
		return false;  //something is wrong with your path!

	if (fa & FILE_ATTRIBUTE_DIRECTORY)
		return true;   // this is a directory!

	return false;    // this is not a directory!
}

bool Dir::Create(const String& path)
{
	ArrayList<String> paths = Path::Split(path);

	Int find = -1;
	while (true)
	{
		find = path.find('/', find+1);

		String dir;

		if (find != String::npos)
		{
			String dir = String(path, 0, find);
			CreateDirectory(dir.c_str(), NULL);
		}
		else
		{
			CreateDirectory(path.c_str(), NULL);
			break;
		}
	}
	return true;
}

ArrayList<String> Dir::Read(const String& path)
{
	DIR* dir = nullptr;
	ArrayList<String> results;

	dir = opendir(path.c_str());
	if (dir == NULL)
	{
		KL_WARNING("Couldn't ReadDir '" + path + "'");
		return results;
	}

	dirent* dirEnt;

	while (( dirEnt = readdir(dir) ) != 0)
	{
		String entry = String(dirEnt->d_name);
		if (entry != "." && entry != "..")
		{
			results.push_back(String(dirEnt->d_name));
		}
	}

	closedir(dir);

	return results;
}

bool Exists(const String& path)
{
	return Dir::IsDir(path) || File::Exists(path);
}

} // filesystem
} // klib