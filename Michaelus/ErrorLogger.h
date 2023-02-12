#pragma once
#include "BaseLogger.h"
class ErrorLogger : public BaseLogger
{
public:
	ErrorLogger();
	ErrorLogger(std::shared_ptr<BaseLogger> pNext);

protected:
	void LogMessage(const std::string& message) override;
};

