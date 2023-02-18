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
	//0번째 플레이어를 가져옴
	auto const Cont = Cast<AUndeadAIController>(OwnerComp.GetAIOwner());
	//EnemyAIController를 가져옴

	if (!!Player)
	{
		Cont->GetBlackBoard()->SetValueAsVector(Blackboard_Keys::targetPos, Player->GetActorLocation());
		//플레이어의 위치를 블랙보드에 저장
	}


	return EBTNodeResult::Succeeded; //성공 반환
}