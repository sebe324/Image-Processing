#include <chrono>
#include <ctime>
#include <string>
#include <filesystem>
namespace Util
{
	std::string getCurrentTime();
	bool doesDirectoryExist(std::string dirName);
	void createDirectory(std::string dirName);
}
