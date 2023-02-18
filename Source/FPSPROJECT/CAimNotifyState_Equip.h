// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAimNotifyState_Equip.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API UCAimNotifyState_Equip : public UAnimNotifyState
{
	GENERATED_BODY()

public:
 
	FString GetNotifyName_Implementation() const;
	// * BlueprintNativeEvent
	// -C++ ������ ���������� �������Ʈ���� override�� ����
	// -����ο����� ��� ������ �����ο����� "_Implementation"�� �ٿ���� �Ѵ�.

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase * Animation, float TotalDuration) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;


	
};
