// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RifleActor.generated.h"

UCLASS()
class FPSPROJECT_API ARifleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARifleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private: //����
	

	UPROPERTY(VisibleDefaultsOnly, Category = "Rifle")
		class USkeletalMeshComponent* Mesh;
	UPROPERTY(VisibleDefaultsOnly, Category = "Rifle")
		FName HandSocket = "Hand_Rifle";
	UPROPERTY(VisibleDefaultsOnly, Category = "Rifle")
		FName HolsterSocket = "Holster_Rifle";
	UPROPERTY(VisibleDefaultsOnly, Category = "Rifle")
		FName GunSocket = "hand_gun_bullet";
	class ACharacter* OwnerCharacter;
	class ABullet* bullet;
	class Undead* undead;
	

	UPROPERTY(VisibleDefaultsOnly, Category = "Rifle")
		class UAnimMontage* GrabMontage;
	UPROPERTY(VisibleDefaultsOnly, Category = "Rifle")
		class UAnimMontage* UnGrabMontage;
	UPROPERTY(VisibleDefaultsOnly, Category = "Rifle")
		class UAnimMontage* FireMontage;

	bool bEquipped; //���� ���� ����
	bool bEquipping; //���� ���������� ����
	bool bAiming; //�� ��������
	bool bFiring;

public: //�Լ�
	static ARifleActor* Spawn(class UWorld* InWord, class ACharacter* InOwner);

	UFUNCTION()
		void DieOwner();

	FORCEINLINE bool GetEquipped() { return bEquipped; }
	// * FORCEINLINE : �ζ��� �Լ�
	//- �����Ϸ��� ����� �����ϰ� �ζ��� �Լ��� ���������
	//- �Լ�ȣ������� �����ϰ� �ڵ带 �״�� �����ؼ� �־���
	//- �ڵ尡 ª�� ��� �ַ� ���
	FORCEINLINE bool GetEquipping() { return bEquipping; }
	void Equip();
	void BeginEquip();
	void EndEquip();
	void Unequip();
	void BeginUnequip();
	void EndUnEquip();
	void BeginAiming();
	void EndAiming();

	void BeginFire();
	void Firing();
	void EndFire();

	FORCEINLINE bool GetAiming() { return bAiming; }

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
		class UParticleSystem* HitFX;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
		class UParticleSystem* FireFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audo, meta = (allowprivateaccess = true))
		class USoundCue* FireSoundCue;

	UAudioComponent* FireAudioComponent;

};
