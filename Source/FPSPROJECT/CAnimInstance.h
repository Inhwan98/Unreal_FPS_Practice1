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
	//BlueprintReadOnly : �������Ʈ �б� ����
	//���Ͱ� �ƴ� Ŭ�������� �������� ������ ������ ��
	//�̷��� �ؾ� �����Ϳ� ��Ÿ��
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
	//override : �θ𿡼� �����ǵ� �Լ���� ������ִ� Ű����
	//�⺻C++������ �����ص� ������ �𸮾� C++������ �ʼ�
	virtual void NativeUpdateAnimation(float DeltaSeconds) override; //Tick

private:
	class ACharacter* OwnerCharacter;

};