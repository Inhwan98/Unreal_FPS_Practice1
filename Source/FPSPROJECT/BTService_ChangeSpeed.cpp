// Fill out your copyright notice in the Description page of Project Settings.

#include "Undead.h"
#include "UndeadAIController.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "BTService_ChangeSpeed.h"

UBTService_ChangeSpeed::UBTService_ChangeSpeed()
{
	bNotifyBecomeRelevant = true; //BecomeRelevant함수 활성화
	NodeName = TEXT("ChangeSpeed");
}

void UBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	auto const Cont = OwnerComp.GetAIOwner();
	AUndead* const Enemy = Cast<AUndead>(Cont->GetPawn());

	if (Enemy->isDie == true)
	{
		Enemy->GetCharacterMovement()->MaxWalkSpeed = 0.0f;
	}
	else
		Enemy->GetCharacterMovement()->MaxWalkSpeed = speed;

}