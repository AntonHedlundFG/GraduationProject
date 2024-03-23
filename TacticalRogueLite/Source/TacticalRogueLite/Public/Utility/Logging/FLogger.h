#pragma once
#include <mutex>

class FLogger
{
private:
	static FLogger& Get();
	void Log(const FString& Message);
    
	// Prevent copying and assignment for singleton
	explicit FLogger(const FLogger&) = delete;
	FLogger& operator=(const FLogger&) = delete;

	FLogger();
	~FLogger();
	void Initialize();
	void ShutDown();

private:
	FArchive* LogFile = nullptr;
	std::mutex LogMutex;

	inline static const FString RelativeLogPath = TEXT("TacticalRogueLiteLogs/");
	const FString LogInternalMessage = TEXT("[LOGGER INTERNAL]");

	friend class UCLogger; // Make UCLogger the only class that can access the logger
};
