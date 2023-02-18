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
	//PerceptionComponent ���� �� ����;

	SightConfig->SightRadius = 500.f; //��ǥ���� �� �� �ִ� �ִ� �þ߰Ÿ�
	SightConfig->LoseSightRadius = 550.f; //�̹� �� ��ǥ���� �� �� �ִ� �Ÿ�
	SightConfig->PeripheralVisionAngleDegrees = 90.f; //AI�� �� �� �ִ� ����
	SightConfig->SetMaxAge(5.0f); //�� �� �ִ� �ð�
	SightConfig->DetectionByAffiliation.bDetectEnemies = true; //�� ����
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true; //�Ʊ� ����
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true; //�߸� ����

	GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation()); //�ν����� ������Ʈ
	GetPerceptionComponent()->ConfigureSense(*SightConfig); //���� ����

	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AUndeadAIController::OnTargetDetected);
	//�þ� ���� �ȿ� Ÿ�������� ������Ʈ �Ǿ��ٸ� OnTargetDetected�Լ� ȣ��

	btreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("btreeComp"));
	blackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("blackboardComp"));
}

void AUndeadAIController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(btree);
	btreeComp->StartTree(*btree);
	//�����̺��Ʈ�� ����
}

void AUndeadAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);
	if (blackboardComp)
	{
		blackboardComp->InitializeBlackboard(*btree->BlackboardAsset);
		//������ �ʱ�ȭ
	}
}

void AUndeadAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AUndeadAIController::OnTargetDetected(AActor * actor, FAIStimulus const stimulus)
{
	if (auto const player = Cast<ACPlayer>(actor))
	{	//���� ������ ���� ���͸� PlayerCharacter�� ��ȯ
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("FindPlayer!!"));
		GetBlackBoard()->SetValueAsBool(Blackboard_Keys::isFindPlayer, stimulus.WasSuccessfullySensed());
		//�����忡 �ִ� isFindPlayer Ű�� ���� ����
		//WasSuccessfullySensed : ���������� ���� �ߴ��� ����
	}
}


UBlackboardComponent * AUndeadAIController::GetBlackBoard() const
{
	return blackboardComp;
}

