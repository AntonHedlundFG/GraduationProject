#pragma once
#include <mutex>
#include <unordered_map>
#include <filesystem>
#include <fstream>

// *
// FLogger: A thread-safe, asynchronous logging class
// *
class LogManager
{
public:
	// --- Public API --- //
	
	// Adds a log message to the queue for processing
	static void Log(const std::string& Message);
	// Reads all log entries from the current log file
	static std::vector<std::string> ReadLog();
	// Get one of the most recent log entries, returns an empty string if the ID is invalid
	static std::string GetRecentLogEntry(int64_t logID);
	// Rotates the log file by closing the current one and creating a new one
	static void RotateLogFile();
	
	// Initializes the logger by creating the log file and starting the worker thread
	static void Initialize();
	// Shuts down the logger by closing the log file and stopping the worker thread
	static void ShutDown();
	

private:
	// Constructor/Destructor
	LogManager();
	~LogManager();

	// Prevent copying and assignment
	LogManager(const LogManager&) = delete;
	LogManager& operator=(const LogManager&) = delete;

	// --- File management --- //

	// Logs a message to the log file
	static void WriteToLog(std::string& Message);
	// Gets the current log file path or generates a new one if it's empty
	static std::string GetLogPath();
	// Generates a new log file path based on the current date and time
	static std::string GetNewLogFilePath();
	// Checks if the log file is too large
	static bool IsLogTooBig();
	// Ensures the log file is open for writing
	static void EnsureFileLogOpen();
	// Closes the log file if it's open
	static void CheckAndCloseLog();

	// --- Threading --- //

	// Starts the worker thread for logging
	static void StartLogWorkerThread();
	// Stops the worker thread for logging
	static void StopLogWorkerThread();
	// Background worker for processing queued log messages
	static void LogWorker();

	// --- Utilities --- //

	// Sanitizes the message by removing unwanted characters and truncating it if necessary
	static void SanitizeMessage(std::string& message);

	// --- Members --- //
	
	// Static members
	static std::string logFilePath;
	static std::ofstream logFile;
	static std::mutex logMutex;
	static std::mutex WorkerMutex;
	static bool bIsWorkerDone;
	static long long logItemID;
	static size_t logFileIndex;
	static std::vector<std::string> sessionLogFilePaths;
	
	// Threading components
	static std::queue<std::string> logQueue;
	static std::mutex queueMutex;
	static std::condition_variable workerDoneCondition;
	static std::condition_variable logCondition;
	static std::thread logThread;
	static bool bIsRunning;

	// Configuration
	inline static const std::string appName = "TacticalRogueLite";
	inline static const std::string logInternalTag = "[LOGGER INTERNAL]";
	static constexpr size_t maxLogSize = 10 * 1024 * 1024; // mb * kb * b

	// --- Recent Log Entries Management --- //
	static constexpr size_t maxLogEntries = 300;
	static std::vector<std::pair<int64, std::string>> recentLogEntries;
	static std::unordered_map<int64, size_t> recentLogIndexMap; 
	static std::mutex recentLogMutex;
	static size_t nextLogIndex; 
	static void AddRecentLogEntry(const std::string& Message);
};

