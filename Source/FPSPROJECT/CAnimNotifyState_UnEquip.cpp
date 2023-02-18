// Fill out your copyright notice in the Description page of Project Settings.

#include "Global.h"
#include "IRifle.h"
#include "RifleActor.h"
#include "CAnimNotifyState_UnEquip.h"

FString UCAnimNotifyState_UnEquip::GetNotifyName_Implementation() const
{
	return "UnEquip";
}

void UCAnimNotifyState_UnEquip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	if (!!MeshComp)
	{
		IIRifle* rifle = Cast<IIRifle>(MeshComp->GetOwner());
		if (!!rifle)
		{
			rifle->GetRifle()->BeginUnequip(); //ÀåÂø
		}
	}
}

void UCAnimNotifyState_UnEquip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	if (!!MeshComp)
	{
		IIRifle* rifle = Cast<IIRifle>(MeshComp->GetOwner());
		if (!!rifle)
		{
			rifle->GetRifle()->EndUnEquip();
		}
	}
}
