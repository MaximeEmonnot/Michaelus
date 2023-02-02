#pragma once
#include "BaseLogger.h"
class ErrorLogger : public BaseLogger
{
public:
	ErrorLogger();

protected:
	void LogMessage(const std::string& message) override;
};

