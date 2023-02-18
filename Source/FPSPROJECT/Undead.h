// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CombatInterface.h"
#include "Engine.h"
#include "Components/AudioComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Undead.generated.h"

UCLASS()
class FPSPROJECT_API AUndead : public ACharacter, public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AUndead();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void MeeleAttack_Implementation();
	//�������̽� �Լ� ���ǽ� Implementation�� �����־�� ��

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* DieMontage;


public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	//������ ���� �޴� �Լ�

	UPROPERTY()
		float currentHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		class USphereComponent* HitCollision;

	UFUNCTION()
		void MyBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//��ħ���·� �浹�� ȣ��Ǵ� �Լ�

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Audo, meta = (AllowPrivateAccess = true))
		class USoundCue* UndeadSoundCue;

	UAudioComponent* UndeadAudioComponent;

	UFUNCTION()
		void Die();

	bool isAttack = false;
	bool isDamage = false;
	bool isDie = false;

};
