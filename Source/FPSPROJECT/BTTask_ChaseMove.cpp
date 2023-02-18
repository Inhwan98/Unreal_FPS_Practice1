// Fill out your copyright notice in the Description page of Project Settings.

#include "Undead.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Blackboard_keys.h"
#include "UndeadAIController.h"

#include "BTTask_ChaseMove.h"

UBTTask_ChaseMove::UBTTask_ChaseMove()
{
	NodeName = TEXT("ChaseMove");
}

EBTNodeResult::Type UBTTask_ChaseMove::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	AUndeadAIController* const Cont = Cast<AUndeadAIController>(OwnerComp.GetAIOwner());

	FVector const PlayerLocation = Cont->GetBlackBoard()->GetValueAsVector(Blackboard_Keys::targetPos);
	//블랙보드에 저장된 플레이어 위치를 가져옴

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(Cont, PlayerLocation);
	//목적짂자ㅣ 폰을 이동시켜줌

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//테스트 종료
	return EBTNodeResult::Succeeded; //성공 반환
}
