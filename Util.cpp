#include "Util.h"
#pragma warning(disable : 4996)

namespace fs = std::filesystem;

std::string Util::getCurrentTime()
{
    std::time_t current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::string result= std::string(std::ctime(&current_time));
	std::replace(result.begin(), result.end(), ':', '_');
	result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
	return result;

}

bool Util::doesDirectoryExist(std::string dirName)
{
	fs::path pathToDir(dirName);
	return fs::is_directory(pathToDir);
}

void Util::createDirectory(std::string dirName)
{
	fs::path pathToDir(dirName);
	fs::create_directory(pathToDir);
}
