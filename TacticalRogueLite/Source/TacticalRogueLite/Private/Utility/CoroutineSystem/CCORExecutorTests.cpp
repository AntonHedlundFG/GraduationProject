
#pragma once
#include "../../../Public/Utility/CoroutineSystem/CCORExecutor.h"
#include "Misc/AutomationTest.h"


IMPLEMENT_SIMPLE_AUTOMATION_TEST(IndividualHandleTest, "TacticalRogueLite.TacticalRogueLite.Public.Utility.CoroutineSystem.CCORExecutorTests.IndividualHandleTest",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);

bool IndividualHandleTest::RunTest(const FString& Parameters)
{
	FExecutable A{};
	FExecutable B{};
	FExecutableHandle HandleA = A.GenerateHandle();
	FExecutableHandle HandleB = B.GenerateHandle();
	TestNotEqual("Handles for individual executables should be unique." + FString::Printf(TEXT("Value 1: %llu   Value 2: %llu "),HandleB.ID,HandleA.ID),HandleB.ID,HandleA.ID);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(DeterministicHandleTest, "TacticalRogueLite.TacticalRogueLite.Public.Utility.CoroutineSystem.CCORExecutorTests.DeterministicHandleTest",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);

bool DeterministicHandleTest::RunTest(const FString& Parameters)
{
	FExecutable A{};
	FExecutableHandle HandleA = A.GenerateHandle();
	FExecutableHandle HandleB = A.GenerateHandle();
	TestEqual("Handles for the same executable should be equal." + FString::Printf(TEXT("Value 1: %llu   Value 2: %llu "),HandleB.ID,HandleA.ID),HandleB.ID,HandleA.ID);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(StatelessDeterministicHandleTest, "TacticalRogueLite.TacticalRogueLite.Public.Utility.CoroutineSystem.CCORExecutorTests.StatelessDeterministicHandleTest",
								 EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter);

bool StatelessDeterministicHandleTest::RunTest(const FString& Parameters)
{
	FExecutable A{};
	FExecutableHandle HandleA = A.GenerateHandle();
	A.bPaused = true;
	FExecutableHandle HandleB = A.GenerateHandle();
	TestEqual("Handles for the same executable should be equal." + FString::Printf(TEXT("Value 1: %llu  Value 2: %llu "),HandleB.ID,HandleA.ID),HandleB.ID,HandleA.ID);

	return true;
}
