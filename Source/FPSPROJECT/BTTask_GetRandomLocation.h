// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_GetRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API UBTTask_GetRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_GetRandomLocation();

	EBTNodeResult::Type ExecuteTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	//ExecuteTask : 비헤이비어 트리에서 태스크 실행시 ExecuteTask함수 실행
	//ExecuteTask함수는 1개의 중당과 3가지 값을 반환
	// * Aborted : 태스크 실행중에 중단됐다. 결과적으로 실패했고 값을 반환했다 할 수 없다.
	// * Failed : 태스크를 수행했지만 실패했다.
	// * Succeeded : 태스크를 성공적으로 마쳤다.
	// * InProgress : 태스크를 수행하고 있으며 결과는 추후 반환 예정


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = "true"))
		float SearchRadius = 1500.0f;
};
