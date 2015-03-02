#include <KLib/Time.hpp>

using namespace std::chrono;

namespace klib
{
namespace time
{

String GetTimeAsString(String formatString, time_t time)
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

String ToString(system_clock::time_point timePoint, String format)
{
	return GetTimeAsString(format, system_clock::to_time_t( timePoint ));
	//WkDay, Month Day Year H:M:S Timezone
	//Mon, Jun 23 2014 23:56:28 GMT
}

} // time
} // klib