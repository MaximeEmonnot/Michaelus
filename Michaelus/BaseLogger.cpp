#include "BaseLogger.h"

#include <filesystem>

std::string BaseLogger::outDirectory;

BaseLogger::BaseLogger()
{
	const time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);
	outDirectory = std::string("Logs/") + std::to_string(ltm.tm_mday) + std::string("-") + std::to_string(ltm.tm_mon + 1) + std::string("-") + std::to_string(ltm.tm_year + 1900);
	std::filesystem::create_directories(outDirectory);
}

BaseLogger::~BaseLogger()
{
	outFile.close();
}

void BaseLogger::Write(const std::string& message, ELoggerVerbosity priority)
{
	if (priority == verbosity) LogMessage(message);
	if (pNext) pNext->Write(message, priority);
}

