#pragma once
#include "BaseLogger.h"
class DebugLogger : public BaseLogger
{
public:
	DebugLogger();

protected:
	void LogMessage(const std::string& message) override;
};

