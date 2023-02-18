// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine.h"
#include "Blackboard_keys.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AISenseConfig.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "UObject/ConstructorHelpers.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"

#include "Perception/AIPerceptionComponent.h"
#include "Runtime/Engine/CLasses/Kismet/GameplayStatics.h"
#include "CPlayer.h"
#include "Perception/AISenseConfig_Sight.h"

#include "UndeadAIController.h"

AUndeadAIController::AUndeadAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent")));
	//PerceptionComponent 생성 및 적용;

	SightConfig->SightRadius = 500.f; //목표물을 볼 수 있는 최대 시야거리
	SightConfig->LoseSightRadius = 550.f; //이미 본 목표물을 볼 수 있는 거리
	SightConfig->PeripheralVisionAngleDegrees = 90.f; //AI가 볼 수 있는 각도
	SightConfig->SetMaxAge(5.0f); //볼 수 있는 시간
	SightConfig->DetectionByAffiliation.bDetectEnemies = true; //적 인지
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true; //아군 인지
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true; //중립 인지

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation()); //인식정보 업데이트
	GetPerceptionComponent()->ConfigureSense(*SightConfig); //감지 구성

	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AUndeadAIController::OnTargetDetected);
	//시야 범위 안에 타겟정보가 업데이트 되었다면 OnTargetDetected함수 호출

	btreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("btreeComp"));
	blackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("blackboardComp"));
}

void AUndeadAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(btree);
	btreeComp->StartTree(*btree);
	//비헤이비어트리 시작
}

void AUndeadAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);
	if (blackboardComp)
	{
		blackboardComp->InitializeBlackboard(*btree->BlackboardAsset);
		//블랙보드 초기화
	}
}

void AUndeadAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AUndeadAIController::OnTargetDetected(AActor * actor, FAIStimulus const stimulus)
{
	if (auto const player = Cast<ACPlayer>(actor))
	{	//감지 범위에 들어온 액터를 PlayerCharacter로 변환
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("FindPlayer!!"));
		GetBlackBoard()->SetValueAsBool(Blackboard_Keys::isFindPlayer, stimulus.WasSuccessfullySensed());
		//블랙보드에 있는 isFindPlayer 키의 값을 변경
		//WasSuccessfullySensed : 성공적으로 감지 했는지 여부
	}
}


UBlackboardComponent * AUndeadAIController::GetBlackBoard() const
{
	return blackboardComp;
}

