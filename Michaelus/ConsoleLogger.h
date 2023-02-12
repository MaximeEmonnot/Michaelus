#pragma once
#include "BaseLogger.h"

class ConsoleLogger : public BaseLogger
{
public:
	ConsoleLogger();
	ConsoleLogger(std::shared_ptr<BaseLogger> pNext);

protected:
	void LogMessage(const std::string& message) override;
};

