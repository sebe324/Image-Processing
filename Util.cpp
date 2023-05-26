#include "Util.h"

namespace fs = std::filesystem;

std::string Util::getCurrentTime()
{
    std::time_t current_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    return std::string(std::ctime(&current_time));

}

bool Util::doesDirectoryExist(std::string dirName)
{
	fs::path filepath = std::string(dirName);
	return fs::is_directory(filepath.parent_path());
}

void Util::createDirectory(std::string dirName)
{
	const fs::path pathToDir{dirName};
	fs::create_directory(pathToDir);
}
