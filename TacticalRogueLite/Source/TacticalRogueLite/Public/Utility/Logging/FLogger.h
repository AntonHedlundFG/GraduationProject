#pragma once
#include <mutex>

class FLogger
{
private:
	static FLogger& Get();
	void Log(const FString& Message);
	TArray<FString> ReadLog();
	
	static FString GetLogPath()
	{
		if(LogFilePath.IsEmpty())
		{
			LogFilePath = FPaths::ProjectSavedDir() + RelativeLogPath + FDateTime::Now().ToString() + TEXT(".log");
		}
		return LogFilePath;
	}
    
	// Prevent copying and assignment for singleton
	explicit FLogger(const FLogger&) = delete;
	FLogger& operator=(const FLogger&) = delete;

	FLogger();
	~FLogger();
	void Initialize();
	void ShutDown();

private:
	static FString LogFilePath;
	FArchive* LogFile = nullptr;
	std::mutex LogMutex;

	inline static const FString RelativeLogPath = TEXT("TacticalRogueLiteLogs/");
	const FString LogInternalTag = TEXT("[LOGGER INTERNAL]");

	friend class UCLogger; // Make UCLogger the only class that can access the logger
};
