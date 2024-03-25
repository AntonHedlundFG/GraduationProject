#include "Utility/Logging/FLogger.h"

#pragma region Static Member Definitions

// Define the static members
FString FLogger::LogFilePath;
FArchive* FLogger::LogFile = nullptr;
std::mutex FLogger::LogMutex;
std::queue<FString> FLogger::LogQueue;
std::mutex FLogger::QueueMutex;
std::condition_variable FLogger::LogCondition;
std::thread FLogger::LogThread;
bool FLogger::bIsRunning = false; // Initialize to false

#pragma endregion

FLogger::FLogger()
{
	Initialize();
}

FLogger::~FLogger()
{
	ShutDown();
}

FLogger& FLogger::Get()
{
	static FLogger Instance;
	return Instance;
}

// Adds a log message to the queue for processing
void FLogger::Log(const FString& Message)
{
	std::lock_guard Lock(QueueMutex);
	LogQueue.push(Message);
	LogCondition.notify_one();
}

// Reads all log entries from the current log file
TArray<FString> FLogger::ReadLog()
{
	TArray<FString> LogEntries;
	std::lock_guard Lock(LogMutex);
	
	CheckAndCloseLog();
	FFileHelper::LoadFileToStringArray(LogEntries, *GetLogPath());
	LogFile = IFileManager::Get().CreateFileWriter(*GetLogPath(), FILEWRITE_Append);

	return LogEntries;
}

// Logs a message to the file
void FLogger::LogToFile(FString& Message)
{
	std::lock_guard Lock(LogMutex);
	EnsureFileLogOpen(); 
	CheckAndRotateLogFile(); 
	
	if (LogFile) { // Check might be redundant but it's better to be safe
		SanitizeMessage(Message);
		const FString LogMessage = FString::Printf(TEXT("%s - %s\n"), *FDateTime::Now().ToString(), *Message);
		LogFile->Serialize(TCHAR_TO_ANSI(*LogMessage), LogMessage.Len());
		LogFile->Flush();
	}
}

// Background worker for processing log messages
void FLogger::LogWorker()
{
	FString Message;
	while (true)
	{
		{
			std::unique_lock Lock(QueueMutex);
			LogCondition.wait(Lock, [] { return !bIsRunning || !LogQueue.empty(); });
          
			if(!bIsRunning && LogQueue.empty())
			{
				break;
			}
			
			Message = LogQueue.front();
			LogQueue.pop();
		}
		
		if (!Message.IsEmpty())
		{
			LogToFile(Message);
		}
	}
}

// Generates a new log file path based on the current date and time
FString FLogger::GetNewLogFilePath()
{
	return FPaths::ProjectSavedDir() + RelativeLogPath + FDateTime::Now().ToString() + TEXT(".log");
}

// Checks if the log file is too large and rotates it if necessary
void FLogger::CheckAndRotateLogFile()
{
	// Check if the log file is too large and rotate it if necessary
	if (LogFile && LogFile->TotalSize() > GetMaxLogSize()) 
	{
		CheckAndCloseLog();
		LogFilePath = GetNewLogFilePath();
		EnsureFileLogOpen();
	}
}

// Initializes the logger by creating the log file and starting the worker thread
void FLogger::Initialize()
{
	EnsureFileLogOpen();

	const FString Message = FString::Printf(TEXT("%s %s"), *LogInternalTag, TEXT("Logger Initialized"));
	std::lock_guard Lock(QueueMutex);
	LogQueue.push(Message);
	LogCondition.notify_one();

	StartLogWorkerThread();
}

// Shuts down the logger by closing the log file and stopping the worker thread
void FLogger::ShutDown()
{
	FString Message = FString::Printf(TEXT("%s %s"), *LogInternalTag, TEXT("Logger Shut Down"));
	LogToFile(Message);
	
	StopLogWorkerThread();
	CheckAndCloseLog();
}

// Sanitizes the message by removing unwanted characters and truncating it if necessary
void FLogger::SanitizeMessage(FString& Message)
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
void FLogger::StartLogWorkerThread()
{
	bIsRunning = true;
	LogThread = std::thread(LogWorker);
}

// Stops the worker thread for logging
void FLogger::StopLogWorkerThread()
{
	bIsRunning = false;
	LogCondition.notify_all();

	if(LogThread.joinable())
	{
		LogThread.join();
	}
}

// Gets the current log file path or generates a new one if it's empty
FString FLogger::GetLogPath()
{
	if(LogFilePath.IsEmpty())
	{
		LogFilePath = GetNewLogFilePath();
	}
	return LogFilePath;
}

// Ensures the log file is open for writing
void FLogger::EnsureFileLogOpen()
{
	if (!LogFile) {
		const EFileWrite WriteFlag = IFileManager::Get().FileExists(*GetLogPath()) ? FILEWRITE_Append : FILEWRITE_None;
		LogFile = IFileManager::Get().CreateFileWriter(*GetLogPath(), WriteFlag);
	}
}

// Closes the log file if it's open
void FLogger::CheckAndCloseLog()
{
	if(LogFile)
	{
		LogFile->Flush();
		LogFile->Close();
		delete LogFile;
		LogFile = nullptr;
		IFileManager::Get().SetTimeStamp(*GetLogPath(), true);
	}
}

