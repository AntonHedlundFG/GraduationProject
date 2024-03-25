#pragma once
#include <mutex>

// *
// FLogger: A thread-safe, asynchronous logging class
// *
class FLogger
{
public:
	// Public API
	static FLogger& Get();
	void Log(const FString& Message);
	TArray<FString> ReadLog();

private:
	// Constructor/Destructor
	FLogger();
	~FLogger();

	// Initialization and Shutdown
	static void Initialize();
	static void ShutDown();
	
	// Prevent copying and assignment
	FLogger(const FLogger&) = delete;
	FLogger& operator=(const FLogger&) = delete;

	// File management
	static FString GetLogPath();
	static FString GetNewLogFilePath();
	static void CheckAndRotateLogFile();
	static void LogToFile(FString& Message);
	static void EnsureFileLogOpen();
	static void CheckAndCloseLog();

	// Worker thread for asynchronous logging
	static void StartLogWorkerThread();
	static void StopLogWorkerThread();
	static void LogWorker();

	// Utilities
	static void SanitizeMessage(FString& Message);

	// Static members
	static FString LogFilePath;
	static FArchive* LogFile;
	static std::mutex LogMutex;

	// Threading components
	static std::queue<FString> LogQueue;
	static std::mutex QueueMutex;
	static std::condition_variable ShutdownCondition;
	static std::condition_variable LogCondition;
	static std::thread LogThread;
	static bool bIsRunning;

	// Configuration
	inline static const FString RelativeLogPath = TEXT("TacticalRogueLiteLogs/");
	inline static const FString LogInternalTag = TEXT("[LOGGER INTERNAL]");
	static constexpr int16 MaxLogSizeMB = 100; // Maximum log file size in megabytes
	static int64 GetMaxLogSize() { return MaxLogSizeMB * 1024 * 1024; } // Convert MB to bytes
};