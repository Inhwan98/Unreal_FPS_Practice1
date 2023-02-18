// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FPSPROJECT_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	//BlueprintReadOnly : 블루프린트 읽기 전용
	//액터가 아닌 클래스에서 에디터의 변수를 공개할 때
	//이렇게 해야 에디터에 나타남
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		float Speed;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		bool bEquipped;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		bool bAiming;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		float Direction;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		float Pitch;
public:
	virtual void NativeBeginPlay() override; //BeginPlay
	//override : 부모에서 재정의된 함수라고 명시해주는 키워드
	//기본C++에서는 생략해도 되지만 언리얼 C++에서는 필수
	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //Tick

private:
	class ACharacter* OwnerCharacter;

};