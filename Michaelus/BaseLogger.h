#pragma once
#include <fstream>
#include <memory>
#include <string>

enum class ELoggerVerbosity
{
	Console,
	Debug,
	Error,
	None
};

class BaseLogger
{
public:
	BaseLogger();
	virtual ~BaseLogger();

	void Write(const std::string& message, ELoggerVerbosity priority);

protected:
	virtual void LogMessage(const std::string& message) = 0;

protected:
	static std::string outDirectory;

	std::fstream outFile;

	std::shared_ptr<BaseLogger> pNext;
	ELoggerVerbosity verbosity = ELoggerVerbosity::None;
};

