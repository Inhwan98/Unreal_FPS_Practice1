// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_ChaseMove.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API UBTTask_ChaseMove : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_ChaseMove();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};