#pragma once
#include "BaseLogger.h"

class ConsoleLogger : public BaseLogger
{
public:
	ConsoleLogger();

protected:
	void LogMessage(const std::string& message) override;
};

