#pragma once
#include <mutex>

// *
// FLogger: A thread-safe, asynchronous logging class
// *
class FLogger
{
public:
	// --- Public API --- //
	
	// Adds a log message to the queue for processing
	static void Log(const FString& Message);
	// Reads all log entries from the current log file
	static TArray<FString> ReadLog();
	// Rotates the log file by closing the current one and creating a new one
	static void RotateLogFile();
	
	// Initializes the logger by creating the log file and starting the worker thread
	static void Initialize();
	// Shuts down the logger by closing the log file and stopping the worker thread
	static void ShutDown();

private:
	// Constructor/Destructor
	FLogger();
	~FLogger();

	// Prevent copying and assignment
	FLogger(const FLogger&) = delete;
	FLogger& operator=(const FLogger&) = delete;

	// --- File management --- //

	// Gets the current log file path or generates a new one if it's empty
	static FString GetLogPath();
	// Generates a new log file path based on the current date and time
	static FString GetNewLogFilePath();
	// Checks if the log file is too large
	static bool IsLogTooBig();
	// Logs a message to the log file
	static void WriteToLog(FString& Message);
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
	static void SanitizeMessage(FString& Message);

	// --- Members --- //
	
	// Static members
	static FString LogFilePath;
	static FArchive* LogFile;
	static std::mutex LogMutex;
	static std::mutex WorkerMutex;
	static bool bIsWorkerDone;
	inline static int16 LogFileIndex;

	// Threading components
	static std::queue<FString> LogQueue;
	static std::mutex QueueMutex;
	static std::condition_variable WorkerDoneCondition;
	static std::condition_variable LogCondition;
	static std::thread LogThread;
	static bool bIsRunning;

	// Configuration
	inline static const FString RelativeLogPath = TEXT("TacticalRogueLiteLogs/");
	inline static const FString LogInternalTag = TEXT("[LOGGER INTERNAL]");
	static constexpr int16 MaxLogSizeMB = 100; // Maximum log file size in megabytes
	static int64 GetMaxLogSize() { return MaxLogSizeMB * 1024 * 1024; } // Convert MB to bytes
};