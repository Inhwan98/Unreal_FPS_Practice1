// Fill out your copyright notice in the Description page of Project Settings.

#include "IRifle.h"
#include "RifleActor.h"
#include "CAimNotifyState_Equip.h"

FString UCAimNotifyState_Equip::GetNotifyName_Implementation() const
{
	return "Equip";
}

void UCAimNotifyState_Equip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	if (!!MeshComp)
	{

		IIRifle* rifle = Cast<IIRifle>(MeshComp->GetOwner());
		if (!!rifle)
		{
			rifle->GetRifle()->BeginEquip(); //ÀåÂø
		}
	}
}

void UCAimNotifyState_Equip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	if (!!MeshComp)
	{
		IIRifle* rifle = Cast<IIRifle>(MeshComp->GetOwner());
		if (!!rifle)
		{
			rifle->GetRifle()->EndEquip();
		}
	}
}
