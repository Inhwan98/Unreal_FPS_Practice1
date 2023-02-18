// Fill out your copyright notice in the Description page of Project Settings.


#include "blackboard_keys.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "UndeadAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTreeTypes.h"

#include "BTTask_GetPlayerLocation.h"


UBTTask_GetPlayerLocation::UBTTask_GetPlayerLocation()
{
	NodeName = TEXT("GetPlayerLocation");
}

EBTNodeResult::Type UBTTask_GetPlayerLocation::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ACharacter* const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//0��° �÷��̾ ������
	auto const Cont = Cast<AUndeadAIController>(OwnerComp.GetAIOwner());
	//EnemyAIController�� ������

	if (!!Player)
	{
		Cont->GetBlackBoard()->SetValueAsVector(Blackboard_Keys::targetPos, Player->GetActorLocation());
		//�÷��̾��� ��ġ�� �����忡 ����
	}


	return EBTNodeResult::Succeeded; //���� ��ȯ
}