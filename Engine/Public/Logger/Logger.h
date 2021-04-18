#pragma once

#include <iostream>

class Logger;

Logger& Log();

/*
 * Basic Logging : Currently work-in-progress and not thread safe.
 */
class Logger
{
public:
	template <typename... Args>
	void Info(const Args&... Messages) const
	{
		Log(ELogLevel::Info, Messages...);
	}
	
	template <typename... Args>
	void Warning(const Args&... Messages) const
	{
		Log(ELogLevel::Warning, Messages...);
	}

	template <typename... Args>
	void Error(const Args&... Messages) const
	{
		Log(ELogLevel::Error, Messages...);
	}

private:
	enum class ELogLevel
	{
		Info,
		Warning,
		Error
	};

private:
	template <typename... Args>
	void Log(const ELogLevel LogLevel, const Args&... Messages) const
	{
		LogPreMessage(LogLevel);
		(LogEntry(Messages), ...);
		LogPostMessage();
	}

	template <typename T>
	void LogEntry(const T& LogEntry) const
	{
		std::cout << LogEntry;
	}

	void LogPreMessage(const ELogLevel LogLevel) const;
	void LogPostMessage() const;
};