#pragma once
#include "BaseLogger.h"
class DebugLogger : public BaseLogger
{
public:
	DebugLogger();
	DebugLogger(std::shared_ptr<BaseLogger> pNext);

protected:
	void LogMessage(const std::string& message) override;
};

