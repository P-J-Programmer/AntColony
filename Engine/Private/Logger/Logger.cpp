#include "Logger/Logger.h"

Logger& Log()
{
	static Logger Log;
	return Log;
}

void Logger::LogPreMessage(const ELogLevel LogLevel) const
{
}

void Logger::LogPostMessage() const
{
	std::cout << std::endl;
}
