#include "Utility/Logging/FLogManager.h"

#pragma region Static Member Definitions

// Define the static members

// Log Manager
FString FLogManager::LogFilePath;
FArchive* FLogManager::LogFile = nullptr;
std::mutex FLogManager::LogMutex;
std::mutex FLogManager::WorkerMutex;
bool FLogManager::bIsWorkerDone = false;
TArray<FString> FLogManager::SessionLogFilePaths;
std::queue<FString> FLogManager::LogQueue;
std::mutex FLogManager::QueueMutex;
std::condition_variable FLogManager::WorkerDoneCondition;
std::condition_variable FLogManager::LogCondition;
std::thread FLogManager::LogThread;
bool FLogManager::bIsRunning = false;

// Recent Log Entries
std::vector<std::pair<int64, FString>> FLogManager::RecentLogEntries;
std::unordered_map<int64, size_t> FLogManager::RecentLogIndexMap;
std::mutex FLogManager::RecentLogMutex;
size_t FLogManager::NextLogIndex = 0;

#pragma endregion

// Constructor
FLogManager::FLogManager()
{
	Initialize();
}

// Destructor
FLogManager::~FLogManager()
{
	ShutDown();
}

// Adds a log message to the queue for processing
void FLogManager::Log(const FString& Message)
{
	std::lock_guard Lock(QueueMutex);
	LogQueue.push(Message);
	LogCondition.notify_one();
}

// Reads all log entries from the current log file
TArray<FString> FLogManager::ReadLog()
{
	TArray<FString> LogEntries;
	
	CheckAndCloseLog();

	{
		std::lock_guard Lock(LogMutex);
		for (FString SessionLogFilePath : SessionLogFilePaths)
		{
			TArray<FString> TempLogEntries;
			if(FFileHelper::LoadFileToStringArray(TempLogEntries, *SessionLogFilePath))
			{
				LogEntries.Append(TempLogEntries);
			}
		}
	}

	EnsureFileLogOpen();

	return LogEntries;
}

// Logs a message to the file
void FLogManager::WriteToLog(FString& Message)
{
	EnsureFileLogOpen();
	
	if(IsLogTooBig())
	{
		RotateLogFile();
	}

	LogItemID++;
	AddRecentLogEntry(Message);

	{
		std::lock_guard Lock(LogMutex);
		SanitizeMessage(Message);
		const FString LogMessage = FString::Printf(TEXT("[%lld][%s] %s\n"), LogItemID, *FDateTime::Now().ToString(), *Message);
		LogFile->Serialize(TCHAR_TO_ANSI(*LogMessage), LogMessage.Len());
		LogFile->Flush();
	}
	
}

// Background worker for processing log messages
void FLogManager::LogWorker()
{
	FString Message;
	while (true)
	{
		{
			std::unique_lock QueueLock(QueueMutex);
			LogCondition.wait(QueueLock, [] { return !bIsRunning || !LogQueue.empty(); });
			
			if(!bIsRunning && LogQueue.empty())
			{
				bIsWorkerDone = true;
				WorkerDoneCondition.notify_all();
				break;
			}
			
			if (!LogQueue.empty())
			{
				bIsWorkerDone = false;
				Message = LogQueue.front();
				LogQueue.pop();
			}
		}
		
		if (!Message.IsEmpty())
		{
			WriteToLog(Message);
		}
	}
}

// Generates a new log file path based on the current date and time
FString FLogManager::GetNewLogFilePath()
{
	FString FilePath = FPaths::ProjectSavedDir() + RelativeLogPath + FDateTime::Now().ToString() + FString::Printf(TEXT("_%d.log"), ++LogFileIndex);
	SessionLogFilePaths.Add(FilePath);
	return FilePath;
}

// Checks if the log file is too large
bool FLogManager::IsLogTooBig()
{
	std::lock_guard Lock(LogMutex);
	if(LogFile)
	{
		if( LogFile->TotalSize() > GetMaxLogSize())
		{
			return true;
		}
	}
	return false;
}

// Rotates the log file by closing the current one and creating a new one
void FLogManager::RotateLogFile()
{
	FString Message = FString::Printf(TEXT("%s %s"), *LogInternalTag, TEXT("Rotating Log File"));
	WriteToLog(Message);
	
	CheckAndCloseLog();

	{
		std::lock_guard Lock(LogMutex);
		LogFilePath = GetNewLogFilePath();
	}

	EnsureFileLogOpen();

	Message = FString::Printf(TEXT("%s %s"), *LogInternalTag, TEXT("Log File Rotated"));
	WriteToLog(Message);
}

// Initializes the logger by creating the log file and starting the worker thread
void FLogManager::Initialize()
{
	EnsureFileLogOpen();

	const FString Message = FString::Printf(TEXT("%s %s"), *LogInternalTag, TEXT("Logger Initialized"));
	std::lock_guard Lock(QueueMutex);
	LogQueue.push(Message);
	LogCondition.notify_one();

	StartLogWorkerThread();
}

// Shuts down the logger by closing the log file and stopping the worker thread
void FLogManager::ShutDown()
{
	FString Message = FString::Printf(TEXT("%s %s"), *LogInternalTag, TEXT("Logger Shut Down"));
	WriteToLog(Message);
	
	StopLogWorkerThread();
	CheckAndCloseLog();
}

// Sanitizes the message by removing unwanted characters and truncating it if necessary
void FLogManager::SanitizeMessage(FString& Message)
{
	Message.ReplaceInline(TEXT("\n"), TEXT("\\n"));
	Message.ReplaceInline(TEXT("\r"), TEXT("\\r"));
	Message.ReplaceInline(TEXT("\t"), TEXT("\\t"));
	if (Message.IsEmpty())
	{
		Message = TEXT("Empty Log Message Received -> Should not be possible!?");
	}
	if (Message.Len() > 1024)
	{
		Message = Message.Left(1024);
	}

}

// Starts the worker thread for logging
void FLogManager::StartLogWorkerThread()
{
	bIsRunning = true;
	LogThread = std::thread(LogWorker);
}

// Stops the worker thread for logging
void FLogManager::StopLogWorkerThread()
{
	bIsRunning = false;
	LogCondition.notify_all();

	{
		std::unique_lock WorkerLock(WorkerMutex);
		WorkerDoneCondition.wait(WorkerLock, [&]{ return bIsWorkerDone; });
	}


	if(LogThread.joinable())
	{
		LogThread.join();
	}
}

// Gets the current log file path or generates a new one if it's empty
FString FLogManager::GetLogPath()
{
	if(LogFilePath.IsEmpty())
	{
		LogFilePath = GetNewLogFilePath();
	}
	return LogFilePath;
}

// Ensures the log file is open for writing
void FLogManager::EnsureFileLogOpen()
{
	if (!LogFile) {
		std::lock_guard Lock(LogMutex);
		const EFileWrite WriteFlag = IFileManager::Get().FileExists(*GetLogPath()) ? FILEWRITE_Append : FILEWRITE_None;
		LogFile = IFileManager::Get().CreateFileWriter(*GetLogPath(), WriteFlag);
	}
}

// Closes the log file if it's open
void FLogManager::CheckAndCloseLog()
{
	if(LogFile)
	{
		std::lock_guard Lock(LogMutex);
		LogFile->Flush();
		LogFile->Close();
		delete LogFile;
		LogFile = nullptr;
	}
}

// Adds a recent log entry to the list of recent log entries
void FLogManager::AddRecentLogEntry(const FString& Message)
{
	std::lock_guard Lock(RecentLogMutex);

	if(RecentLogEntries.size() >= MaxLogEntries)
	{
		const auto& oldestEntry = RecentLogEntries[NextLogIndex];
		RecentLogIndexMap.erase(oldestEntry.first);
	}

	if(RecentLogEntries.size() < MaxLogEntries)
	{
		RecentLogEntries.push_back({LogItemID, Message});
	}
	else
	{
		RecentLogEntries[NextLogIndex] = {LogItemID, Message};
	}

	RecentLogIndexMap[LogItemID] = NextLogIndex;
	NextLogIndex = (NextLogIndex + 1) % MaxLogEntries;
	
}

// Get one of the most recent log entries, returns an empty string if the ID is invalid
FString FLogManager::GetRecentLogEntry(int64 LogID)
{
	std::lock_guard Lock(RecentLogMutex);
	const auto it = RecentLogIndexMap.find(LogID);
	if(it != RecentLogIndexMap.end())
	{
		return RecentLogEntries[it->second].second;
	}
	return "";
}
