#include "UI/TurnOrder/CTurnOrderUIManager.h"
#include "Kismet/GameplayStatics.h"
#include "Utility/Logging/CLogManager.h"


void UTurnOrderAnimationTask_Remove::Initialize(const TArray<UCTurnOrderPortraitWidget*>& inAffectedPortraits,
                                                const float inWaitTimeAfterCompletion, const float inWaitTimeBetweenAnimations)
{
	Portraits = inAffectedPortraits;
	WaitTimeAfterCompletion = inWaitTimeAfterCompletion;
	WaitTimeBetweenAnimations = inWaitTimeBetweenAnimations;
}


bool UTurnOrderAnimationTask_Remove::Execute(float DeltaTime)
{
	LOG_INFO("UTurnOrderAnimationTask_Remove");

	switch (State)
	{
	case EExecutableState::NotStarted:
		return true;
	case EExecutableState::Paused:
		return true;
	case EExecutableState::Running:
		
		if(Timer == 0)
		{
			UCTurnOrderPortraitWidget* Widget = Portraits[0];
			Portraits.RemoveAt(0);
			Widget->AnimateOut();
		}
		
		if(Timer >= WaitTimeBetweenAnimations)
		{
			Timer = 0;
			if(Portraits.Num() == 0)
			{
				State = EExecutableState::Completed;
			}
			return false;
		}
		break;
	case EExecutableState::Completed:
		if(Timer > WaitTimeAfterCompletion)
		{
			return true;
		}
		break;
	default:
		return true;
	}
	Timer += DeltaTime;
	return false;
}

void UTurnOrderAnimationTask_Add::Initialize( const TArray<UCTurnOrderPortraitWidget*>& inAffectedPortraits,
	const TArray<FVector2D>& inPositions, const float inWaitTimeAfterCompletion, const float inWaitTimeBetweenAnimations)
{
	Portraits = inAffectedPortraits;
	Positions = inPositions;
	WaitTimeAfterCompletion = inWaitTimeAfterCompletion;
	WaitTimeBetweenAnimations = inWaitTimeBetweenAnimations;
}

bool UTurnOrderAnimationTask_Add::Execute(float DeltaTime)
{
	LOG_INFO("UTurnOrderAnimationTask_Add");
	switch (State)
	{
	case EExecutableState::NotStarted:
		return true;
	case EExecutableState::Paused:
		return true;
	case EExecutableState::Running:
		if(Timer == 0)
		{
			if(Portraits.Num() == 0)
			{
				State = EExecutableState::Completed;
				break;
			}
			UCTurnOrderPortraitWidget* Widget = Portraits[0];
			Portraits.RemoveAt(0);
			Widget->SetPosition(Positions[0]);
			Positions.RemoveAt(0);
			Widget->SetVisibility(ESlateVisibility::Visible);
			Widget->AnimateIn();
		}
		if(Timer >= WaitTimeBetweenAnimations)
		{
			Timer = 0;
			return false;
		}
		break;
	case EExecutableState::Completed:
			if(Timer >= WaitTimeAfterCompletion)
			{
				return true;
			}
			break;
		default:
			return true;
	}
	Timer += DeltaTime;
	return false;
}

void UTurnOrderAnimationTask_MoveTo::Initialize( const TArray<UCTurnOrderPortraitWidget*>& inAffectedPortraits, const TArray<FVector2D>& inPositions, const float inWaitTimeAfterCompletion, const float inWaitTimeBetweenAnimations, UCurveFloat* inAnimationEasing)
{
	Portraits = inAffectedPortraits;
	Positions = inPositions;
	WaitTimeAfterCompletion = inWaitTimeAfterCompletion;
	WaitTimeBetweenAnimations = inWaitTimeBetweenAnimations;
	AnimationEasing = inAnimationEasing;
}

bool UTurnOrderAnimationTask_MoveTo::Execute(float DeltaTime)
{
	LOG_INFO("UTurnOrderAnimationTask_MoveTo");
	switch (State)
	{
	case EExecutableState::NotStarted:
		return true;
	case EExecutableState::Paused:
		return true;
	case EExecutableState::Running:
		if(Timer == 0)
		{
			for (UCTurnOrderPortraitWidget* Portrait : Portraits)
			{
				StartPositions.Add(Portrait->GetPosition());
			}
		}
		for(int i = 0; i < Portraits.Num(); i++)
		{
			FVector2D LerpedPosition = StartPositions[i] + (Positions[i] - StartPositions[i]) * AnimationEasing->GetFloatValue(Timer / WaitTimeBetweenAnimations);
			Portraits[i]->SetPosition(LerpedPosition);
		}
		if(Timer >=  WaitTimeBetweenAnimations)
		{
				Timer = 0;
				State = EExecutableState::Completed;
			return false;
		}
		break;
	case EExecutableState::Completed:
			if(Timer > WaitTimeAfterCompletion)
			{
				return true;
			}
			break;
	default:
		return true;
	}
	Timer += DeltaTime;
	return false;
}

void UTurnOrderAnimationTask_EnqueueWidgets::Initialize( const TArray<UCTurnOrderPortraitWidget*>& inWidgetsToEnqueue,
	ACTurnOrderUIManager* inTurnManager)
{
	WidgetsToEnqueue = inWidgetsToEnqueue;
	TurnManager = inTurnManager;
}

bool UTurnOrderAnimationTask_EnqueueWidgets::Execute(float DeltaTime)
{
	for (UCTurnOrderPortraitWidget* Widget : Portraits)
	{
		TurnManager->EnQueuePortraitWidget(Widget);
	}
	return true;
}

ACTurnOrderUIManager::ACTurnOrderUIManager(): GameState(nullptr), Executor(nullptr), AnimationMoveToEasing(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACTurnOrderUIManager::BeginPlay()
{
	Super::BeginPlay();
	GameState = Cast<ACGameState>(GetWorld()->GetGameState());
	if(GameState)
	{
		GameState->OnTurnOrderUpdate.AddDynamic(this,&ACTurnOrderUIManager::UpdateTurnList);
	}

	Executor = GetWorld()->GetSubsystem<UCCORExecutor>();
	
}
void ACTurnOrderUIManager::UpdateTurnList()
{
	return;
	UE_LOG(LogTemp,Warning,TEXT("UpdateTurnList Called"));
	TArray<ACUnit*> NewTurnOrder = GetWorld()->GetGameState<ACGameState>()->GetCurrentTurnOrder(true);
	if(NewTurnOrder.Num() == 0 && LastTurnOrder.Num() == 0){return;}
	
	TArray<UCTurnOrderPortraitWidget*> WidgetsToAdd;
	TArray<FVector2D> AddPositions;
	TArray<UCTurnOrderPortraitWidget*> WidgetsToRemove;
	TArray<UCTurnOrderPortraitWidget*> WidgetsToMove;
	TArray<FVector2D> MovePositions;

	TArray<FVector2D> NewPositions = CalculateViewportPositions(NewTurnOrder.Num());

	TArray<ACUnit*> UnitQueue = TArray<ACUnit*>(NewTurnOrder);

	for(int i = 0; i < NewTurnOrder.Num(); i++)
	{
		ACUnit* Unit = UnitQueue[0];
		UnitQueue.RemoveAt(0);

		if (!Unit) continue;

		if(!LastTurnOrder.Contains(Unit))
		{
			UCTurnOrderPortraitWidget* Widget = DeQueuePortraitWidget();
			FString UnitName = Unit->GetUnitName();
			Widget->SetText(UnitName);
			ActivePortraits.Add(Unit,Widget);
			WidgetsToAdd.Add(Widget);
			AddPositions.Add(NewPositions[i]);
		}
		else if(i < LastTurnOrder.Num() && LastTurnOrder[i] != Unit)
		{
			UCTurnOrderPortraitWidget* Widget = GetActiveWidget(Unit);
			if(Widget != nullptr)
			{
				WidgetsToMove.Add(Widget);
				MovePositions.Add(NewPositions[i]);
			}
			else
			{
				LOG(ELogCategory::LC_Error,"Trying to remove portrait Widget that doesn't exist.[CTurnOrderUIManager]");
			}
		}
		else if(i >= LastTurnOrder.Num())
		{
			UCTurnOrderPortraitWidget* Widget = GetActiveWidget(Unit);
			if(Widget != nullptr)
			{
				WidgetsToMove.Add(Widget);
				MovePositions.Add(NewPositions[i]);
			}
		}
	}

	for(ACUnit* Unit: LastTurnOrder)
	{
		if(!NewTurnOrder.Contains(Unit))
		{
			UCTurnOrderPortraitWidget* Widget = GetActiveWidget(Unit);
			if(Widget != nullptr)
			{
				WidgetsToRemove.Add(Widget);
				ActivePortraits.Remove(Unit);
			}
			else
			{
				LOG(ELogCategory::LC_Error,"Trying to remove portrait Widget that doesn't exist.[CTurnOrderUIManager]");
			}
		}
	}
	if(WidgetsToRemove.Num() != 0)
	{
		UTurnOrderAnimationTask_Remove* RemoveTask = NewObject<UTurnOrderAnimationTask_Remove>();
		RemoveTask->Initialize(WidgetsToRemove,AnimationWaitTime,AnimationTimeOffset);
		Executor->AddExecutable(this, RemoveTask);
		
		UTurnOrderAnimationTask_EnqueueWidgets* EnqueueTask = NewObject<UTurnOrderAnimationTask_EnqueueWidgets>();
		EnqueueTask->Initialize(WidgetsToRemove,this);
		Executor->AddExecutable(this, EnqueueTask);
	}
	if(WidgetsToMove.Num() != 0)
	{
		Algo::Reverse(WidgetsToMove);
		Algo::Reverse(MovePositions);
		
		UTurnOrderAnimationTask_MoveTo* MoveTask = NewObject<UTurnOrderAnimationTask_MoveTo>();
		MoveTask->Initialize(WidgetsToMove,MovePositions,AnimationWaitTime,MoveAnimationLerpTime,AnimationMoveToEasing);
		Executor->AddExecutable(this, MoveTask);
	}
	if(WidgetsToAdd.Num() != 0)
	{
		UTurnOrderAnimationTask_Add* AddTask = NewObject<UTurnOrderAnimationTask_Add>();
		AddTask->Initialize(WidgetsToAdd,AddPositions,AnimationWaitTime,AnimationTimeOffset);
		Executor->AddExecutable(this, AddTask);
	}
	LastTurnOrder = TArray<ACUnit*>(NewTurnOrder);
}

UCTurnOrderPortraitWidget* ACTurnOrderUIManager::CreatePortraitWidget()
{
	UCTurnOrderPortraitWidget* Widget = CreateWidget<UCTurnOrderPortraitWidget>(GetWorld()->GetGameInstance(),PortraitWidget);
	UE_LOG(LogTemp,Warning,TEXT("Creating new portrait widget"));
	Widget->AddToViewport(-999);
	Widget->SetVisibility(ESlateVisibility::Hidden);
	return Widget;
}
UCTurnOrderPortraitWidget* ACTurnOrderUIManager::DeQueuePortraitWidget()
{
	if(WidgetPool.IsEmpty())
	{
		WidgetPool.Enqueue(CreatePortraitWidget());
	}
	UCTurnOrderPortraitWidget* Widget;
	WidgetPool.Dequeue(Widget);
	HandleDequeue(Widget);
	return Widget;
}
void ACTurnOrderUIManager::EnQueuePortraitWidget(UCTurnOrderPortraitWidget* widget)
{
	HandleEnqueue(widget);
	WidgetPool.Enqueue(widget);
}
void ACTurnOrderUIManager::HandleEnqueue(UCTurnOrderPortraitWidget* widget)
{
	widget->SetVisibility(ESlateVisibility::Hidden);
}
void ACTurnOrderUIManager::HandleDequeue(UCTurnOrderPortraitWidget* widget)
{
	widget->SetVisibility(ESlateVisibility::Hidden);
}
UCTurnOrderPortraitWidget* ACTurnOrderUIManager::GetActiveWidget(ACUnit* key)
{
	if(ActivePortraits.Contains(key))
	{
		return ActivePortraits[key];
	}
	return nullptr;
}
TArray<FVector2D> ACTurnOrderUIManager::CalculateViewportPositions(int AmountOfUnits)
{
	int32 ViewportX;
	int32 ViewportY;
	UGameplayStatics::GetPlayerController(GetWorld(),0)->GetViewportSize(ViewportX,ViewportY);

	TArray<FVector2D> Positions;

	for(int i = 0; i < AmountOfUnits; i++)
	{
		int XPos = 0;
		int YPos = ViewportY * WidgetListStartPositionOffsetFromAnchor + (PortraitPixelOffset * i);
		Positions.Add(FVector2D(XPos,YPos));
	}
	
	return Positions;
}
