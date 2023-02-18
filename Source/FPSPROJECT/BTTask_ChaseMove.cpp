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
	//�����忡 ����� �÷��̾� ��ġ�� ������

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(Cont, PlayerLocation);
	//�������ڤ� ���� �̵�������

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//�׽�Ʈ ����
	return EBTNodeResult::Succeeded; //���� ��ȯ
}
