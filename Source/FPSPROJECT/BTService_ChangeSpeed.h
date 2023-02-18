// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_ChangeSpeed.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API UBTService_ChangeSpeed : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_ChangeSpeed();
	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	//Ω««‡»Â∏ß

private:
	UPROPERTY(EditAnywhere, BlueprintREadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		float speed = 150.f;
	
};
