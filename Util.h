#include <chrono>
#include <ctime>
#include <string>
#include <filesystem>
#include <iostream>
#include <algorithm>
namespace Util
{
	std::string getCurrentTime();
	bool doesDirectoryExist(std::string dirName);
	void createDirectory(std::string dirName);

	#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
		const std::string slash = "\\";
	#else
		const std::string slash = "/";
	#endif
}
