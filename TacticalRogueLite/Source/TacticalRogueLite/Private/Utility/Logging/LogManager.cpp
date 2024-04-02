#include "Utility/Logging/LogManager.h"

#include <iostream>

#pragma region Static Members

std::mutex LogManager::logMutex;
std::mutex LogManager::queueMutex;
std::mutex LogManager::recentLogMutex;
std::mutex LogManager::WorkerMutex;
std::condition_variable LogManager::logCondition;
std::condition_variable LogManager::workerDoneCondition;
bool LogManager::bIsRunning;
std::thread LogManager::logThread;
bool LogManager::bIsWorkerDone;
std::queue<std::string> LogManager::logQueue;
std::string LogManager::logFilePath;
std::ofstream LogManager::logFile;
long long LogManager::logItemID;
size_t LogManager::logFileIndex;
size_t LogManager::nextLogIndex; 
std::vector<std::string> LogManager::sessionLogFilePaths;
std::vector<std::pair<int64, std::string>> LogManager::recentLogEntries;
std::unordered_map<int64, size_t> LogManager::recentLogIndexMap; 

#pragma endregion

std::string GetCurrentDateTime();
std::string GetWritableLogDirectory_Windows(const std::string& inAppName);

// Constructor
LogManager::LogManager()
{
	Initialize();
}

// Destructor
LogManager::~LogManager()
{
	ShutDown();
}

// Adds a log message to the queue for processing
void LogManager::Log(const std::string& Message)
{
	std::lock_guard Lock(queueMutex);
	logQueue.push(Message);
	logCondition.notify_one();
}

// Reads all log entries from the current log file
std::vector<std::string> LogManager::ReadLog()
{
	std::vector<std::string> logEntries;
	
	CheckAndCloseLog();

	{
		std::lock_guard lock(logMutex);
		for (const auto& sessionLogFilePath : sessionLogFilePaths)
		{
			std::ifstream inFile(sessionLogFilePath);
			if (inFile.is_open())
			{
				std::string line;
				while (std::getline(inFile, line))
				{
					logEntries.push_back(line);
				}
			}
		}
	}

	EnsureFileLogOpen();

	return logEntries;
}

// Logs a message to the file
void LogManager::WriteToLog(std::string& Message)
{
	EnsureFileLogOpen();
	
	if(IsLogTooBig())
	{
		RotateLogFile();
	}

	logItemID++;
	AddRecentLogEntry(Message);

	{
		std::lock_guard Lock(logMutex);
		SanitizeMessage(Message);
		logFile << "[" << logItemID << "][" << GetCurrentDateTime() << "] " << Message << std::endl;
		logFile.flush();
	}
	
}

// Background worker for processing log messages
void LogManager::LogWorker()
{
	while (true)
	{
		{
			std::unique_lock queueLock(queueMutex);
			logCondition.wait(queueLock, [] { return !bIsRunning || !logQueue.empty(); });
			
			if(!bIsRunning && logQueue.empty())
			{
				bIsWorkerDone = true;
				workerDoneCondition.notify_all();
				break;
			}
			
			if (!logQueue.empty())
			{
				std::string message = logQueue.front();
				logQueue.pop();
				queueLock.unlock();
				bIsWorkerDone = false;
				WriteToLog(message);
			}
		}
	}
}

// Generates a new log file path based on the current date and time
std::string LogManager::GetNewLogFilePath()
{
	const std::string logDirectory = GetWritableLogDirectory_Windows(appName);
	if(!std::filesystem::exists(logDirectory))
	{
		std::filesystem::create_directories(logDirectory);
	}
	std::string filePath = logDirectory + GetCurrentDateTime() + "_" + std::to_string(++logFileIndex) +  ".log";
	sessionLogFilePaths.push_back(filePath);
	return filePath;
}

// Checks if the log file is too large
bool LogManager::IsLogTooBig()
{
	std::lock_guard lock(logMutex);
	const std::filesystem::path path = GetLogPath();

		if( std::filesystem::exists(path))
		{
			const auto fileSize = std::filesystem::file_size(path);
			return fileSize > maxLogSize;
		}
	return false;
}

// Rotates the log file by closing the current one and creating a new one
void LogManager::RotateLogFile()
{
	std::string message = logInternalTag + " Rotating Log File";
	WriteToLog(message);
	
	CheckAndCloseLog();

	{
		std::lock_guard Lock(logMutex);
		logFilePath = GetNewLogFilePath();
	}

	EnsureFileLogOpen();

	message = logInternalTag + " Log File Rotated";
	WriteToLog(message);
}

// Initializes the logger by creating the log file and starting the worker thread
void LogManager::Initialize()
{
	EnsureFileLogOpen();

	const std::string message = logInternalTag + " Logger Initialized";
	std::lock_guard lock(queueMutex);
	logQueue.push(message);
	logCondition.notify_one();

	StartLogWorkerThread();
}

// Shuts down the logger by closing the log file and stopping the worker thread
void LogManager::ShutDown()
{
	std::string message = logInternalTag + " Logger Shut Down";
	WriteToLog(message);
	
	StopLogWorkerThread();
	CheckAndCloseLog();
}

// Sanitizes the message by removing unwanted characters and truncating it if necessary
void LogManager::SanitizeMessage(std::string& message)
{
	std::erase(message, '\n');
	std::erase(message, '\r');
	std::erase(message, '\t');
	if(message.empty())
	{
		message = "Empty Log Message Received -> Should not be possible?";
	}
	if (message.size() > 1024)
	{
		message = message.substr(0, 1024);
	}
}

// Starts the worker thread for logging
void LogManager::StartLogWorkerThread()
{
	if(!bIsRunning)
	{
		bIsRunning = true;
		logThread = std::thread(LogWorker);
	}
}

// Stops the worker thread for logging
void LogManager::StopLogWorkerThread()
{
	bIsRunning = false;
	logCondition.notify_all();

	{
		std::unique_lock workerLock(WorkerMutex);
		workerDoneCondition.wait(workerLock, [&]{ return bIsWorkerDone; });
	}
	
	if(logThread.joinable())
	{
		logThread.join();
	}
}

// Gets the current log file path or generates a new one if it's empty
std::string LogManager::GetLogPath()
{
	if(logFilePath.empty())
	{
		logFilePath = GetNewLogFilePath();
	}
	return logFilePath;
}

// Ensures the log file is open for writing
void LogManager::EnsureFileLogOpen()
{
	if (!logFile.is_open())
	{
		std::lock_guard lock(logMutex);
		logFile.open(GetLogPath(), std::ios::out | std::ios::app);
	}
}

// Closes the log file if it's open
void LogManager::CheckAndCloseLog()
{
	if(logFile)
	{
		std::lock_guard lock(logMutex);
		logFile.flush();
		logFile.close();
	}
}

// Adds a recent log entry to the list of recent log entries
void LogManager::AddRecentLogEntry(const std::string& Message)
{
	std::lock_guard lock(recentLogMutex);

	if(recentLogEntries.size() >= maxLogEntries)
	{
		const auto& oldestEntry = recentLogEntries[nextLogIndex];
		recentLogIndexMap.erase(oldestEntry.first);
	}

	if(recentLogEntries.size() < maxLogEntries)
	{
		recentLogEntries.push_back({logItemID, Message});
	}
	else
	{
		recentLogEntries[nextLogIndex] = {logItemID, Message};
	}

	recentLogIndexMap[logItemID] = nextLogIndex;
	nextLogIndex = (nextLogIndex + 1) % maxLogEntries;
	
}

// Get one of the most recent log entries, returns an empty string if the ID is invalid
std::string LogManager::GetRecentLogEntry(int64_t logID)
{
	std::lock_guard lock(recentLogMutex);
	const auto it = recentLogIndexMap.find(logID);
	if(it != recentLogIndexMap.end())
	{
		return recentLogEntries[it->second].second;
	}
	return "";
}

// Helper to get the current date-time as string
std::string GetCurrentDateTime() {
	const auto now = std::chrono::system_clock::now();
	const auto inTime_t = std::chrono::system_clock::to_time_t(now);

	std::tm buf;
	localtime_s(&buf, &inTime_t);

	std::stringstream ss;
	ss << std::put_time(&buf, "%Y-%m-%d_%H-%M-%S");
	return ss.str();
}

std::string GetEnvVariable(const char* varName) {
	char* buf = nullptr;
	size_t sz = 0;
	if (_dupenv_s(&buf, &sz, varName) == 0 && buf != nullptr) {
		std::string varValue(buf);
		free(buf); // Free the buffer 
		return varValue;
	}
	return "";
}

// Helper to get a writable log directory (platform dependent -> this if for Windows)
std::string GetWritableLogDirectory_Windows(const std::string& inAppName) {
	const std::vector<std::string> potentialPaths = {
		!GetEnvVariable("APPDATA").empty() ? GetEnvVariable("APPDATA") + "\\" + inAppName + "\\Logs\\" : "",
		!GetEnvVariable("HOME").empty() ? GetEnvVariable("HOME") + "/.local/share/" + inAppName + "/Logs/" : "",
		"./Logs/"
	};

	for (const auto& path : potentialPaths) {
		if (!path.empty()) {
			try {
				std::filesystem::create_directories(path);
				return path;
			} catch (const std::filesystem::filesystem_error& e) {
				std::cerr << "Failed to create log directory at " << path << ": " << e.what() << std::endl;
			}
		}
	}
	return std::filesystem::current_path().string() + "/Logs/";
}