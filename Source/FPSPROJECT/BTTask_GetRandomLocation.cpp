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
	//Enemy 위치를 가져옴
	FNavLocation NavPos;

	UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	//월드에 있는 네비게이션 시스템을 가져옴
	if (NavSys->GetRandomPointInNavigableRadius(EnemyPos, SearchRadius, NavPos, nullptr))
	{ //네이게이션 시스템기반 랜덤한 위치를 NavPos 변수에 넣어둠
		Cont->GetBlackBoard()->SetValueAsVector
		(Blackboard_Keys::targetPos, NavPos.Location);
		//랜덤한 위치를 targetPos라는 블랙보드 키에 저장
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); //행동종료
	return EBTNodeResult::Succeeded; //성공 반환
}
