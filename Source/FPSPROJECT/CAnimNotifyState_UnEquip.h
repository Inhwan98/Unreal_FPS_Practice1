// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState_UnEquip.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API UCAnimNotifyState_UnEquip : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	FString GetNotifyName_Implementation() const;
	// *  BlueprintNativeEvent
	// - C++ 구현이 존재하지만 블루프린트에서 override가 가능
	// - 선언부에서는 상관 없지만 구현부에서는 "_Implementation"을 붙여줘야 한다.

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override; 
};
