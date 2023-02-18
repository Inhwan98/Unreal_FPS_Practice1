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

private: //변수
	

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

	bool bEquipped; //무기 장착 여부
	bool bEquipping; //무기 장착중인지 여부
	bool bAiming; //줌 상태인지
	bool bFiring;

public: //함수
	static ARifleActor* Spawn(class UWorld* InWord, class ACharacter* InOwner);

	UFUNCTION()
		void DieOwner();

	FORCEINLINE bool GetEquipped() { return bEquipped; }
	// * FORCEINLINE : 인라인 함수
	//- 컴파일러의 비용을 무시하고 인라인 함수를 실행시켜줌
	//- 함수호출과정을 무시하고 코드를 그대로 복제해서 넣어줌
	//- 코드가 짧을 경우 주로 사용
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
