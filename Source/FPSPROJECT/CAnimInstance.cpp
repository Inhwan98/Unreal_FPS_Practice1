// Fill out your copyright notice in the Description page of Project Settings.

#include "IRifle.h"
#include "RifleActor.h"

#include "GameFramework/Character.h"
#include "Global.h"
#include "CAnimInstance.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (OwnerCharacter == nullptr)
	{
		return; //�Լ� ����
	}
	Speed = OwnerCharacter->GetVelocity().Size2D();
	Direction = CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetControlRotation());
	Pitch = OwnerCharacter->GetBaseAimRotation().Pitch;
	//GetBaseAimRotation : �⺻ ���� ���ϱ� �Լ�
	IIRifle* rifle = Cast<IIRifle>(OwnerCharacter);
	if (!!rifle)
	{
		bEquipped = rifle->GetRifle()->GetEquipped();
		bAiming = rifle->GetRifle()->GetAiming();
	}
}
