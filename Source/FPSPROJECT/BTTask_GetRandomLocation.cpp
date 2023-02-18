// Fill out your copyright notice in the Description page of Project Settings.


#include "BehaviorTree/BlackboardComponent.h"
#include "UndeadAIController.h"
#include "blackboard_keys.h"
#include "NavigationSystem.h"

#include "BTTask_GetRandomLocation.h"

UBTTask_GetRandomLocation::UBTTask_GetRandomLocation()
{
	NodeName = TEXT("GetRandomLocation");
}


EBTNodeResult::Type UBTTask_GetRandomLocation::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	auto const Cont = Cast<AUndeadAIController>(OwnerComp.GetAIOwner());
	auto const Enemy = Cont->GetPawn();

	FVector const EnemyPos = Enemy->GetActorLocation();
	//Enemy ��ġ�� ������
	FNavLocation NavPos;

	UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	//���忡 �ִ� �׺���̼� �ý����� ������
	if (NavSys->GetRandomPointInNavigableRadius(EnemyPos, SearchRadius, NavPos, nullptr))
	{ //���̰��̼� �ý��۱�� ������ ��ġ�� NavPos ������ �־��
		Cont->GetBlackBoard()->SetValueAsVector
		(Blackboard_Keys::targetPos, NavPos.Location);
		//������ ��ġ�� targetPos��� ������ Ű�� ����
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); //�ൿ����
	return EBTNodeResult::Succeeded; //���� ��ȯ
}
