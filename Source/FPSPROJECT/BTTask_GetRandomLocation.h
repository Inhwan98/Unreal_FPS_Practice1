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
	//ExecuteTask : �����̺�� Ʈ������ �½�ũ ����� ExecuteTask�Լ� ����
	//ExecuteTask�Լ��� 1���� �ߴ�� 3���� ���� ��ȯ
	// * Aborted : �½�ũ �����߿� �ߴܵƴ�. ��������� �����߰� ���� ��ȯ�ߴ� �� �� ����.
	// * Failed : �½�ũ�� ���������� �����ߴ�.
	// * Succeeded : �½�ũ�� ���������� ���ƴ�.
	// * InProgress : �½�ũ�� �����ϰ� ������ ����� ���� ��ȯ ����


private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = "true"))
		float SearchRadius = 1500.0f;
};
