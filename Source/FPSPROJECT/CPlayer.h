// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IRifle.h"
#include "CAnimInstance.h"
#include "Components/AudioComponent.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPlayer.generated.h"

UCLASS()
class FPSPROJECT_API ACPlayer : public ACharacter, public IIRifle
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public: //변수
	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (allowprivateaccess = true))
		class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audo, meta = (allowprivateaccess = true))
		class USoundCue* BGMSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audo, meta = (allowprivateaccess = true))
		class USoundCue* WalkSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audo, meta = (allowprivateaccess = true))
		class USoundCue* TakeDamageSoundCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audo, meta = (allowprivateaccess = true))
		class USoundCue* DieSoundCue;

	UAudioComponent* BGMAudioComponent;
	UAudioComponent* WalkAudioComponent;
	UAudioComponent* TakeDamageAudioComponent;
	UAudioComponent* DieAudioComponent;
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<class UCUserWidget_CrossHair> CrossHairWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
		TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Widgets")
		TSubclassOf<UUserWidget> EndWidgetClass;

	UPROPERTY()
		UUserWidget* CurrentWidget;

	UPROPERTY()
		UUserWidget* EndWidget;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class APlayerController* MyController;*/

	//라이플
	class ARifleActor* Rifle;
	class ABullet* bullet;
	//에임
	class UCUserWidget_CrossHair* CrossHair;

private: //함수
	void OnMoveForward(float axis);
	void OnMoveRight(float axis);
	void OnHorizontalLook(float axis);
	void OnVerticalLook(float axis);

	//달리기
	void OnRunning();
	void OffRunning();

	// *라이플 장착
	void OnRifle();

	void OnAim();
	void OffAim();

	void JumpAnim();
public:
	
	FORCEINLINE class ARifleActor* GetRifle() override { return Rifle; }

	//에임시점
	void GetLocationAndDirection(FVector& OutStart, FVector& OutEnd, FVector& OutDirection) override;
	void OnFocus() override;
	void OffFocus() override;

	void OnFire();
	void OffFire();

protected:
	UFUNCTION(BlueprintImplementableEvent)
		void OnZoomIn();
	UFUNCTION(BlueprintImplementableEvent)
		void OnZoomOut();
	// * BlueprintImplementableEvent
	//- 정의는 블루프린트에서 한다는 뜻

	
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
		float currentHp; //현재 체력
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
		float maxHP; //최대 체력


	bool bIsPaused;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	//데미지를 전달 받은 경우 호출

	void Die();

	void OnGamePause();

	UPROPERTY()
		class UAIPerceptionStimuliSourceComponent* stimulus;
	//AI에게 시각적으로 감지할 수 있게 해주는 컴포넌트


	//아이템
public:
	UPROPERTY()
		class UItemComponent* MyInventory;

	UFUNCTION()
		void DropItem();
	UFUNCTION()
		void TakeItem(class AItemActor* item);

	//블록상태 충돌 함수
	UFUNCTION()
		virtual void NotifyHit(class UPrimitiveComponent* MyComp,
			AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved,
			FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)override;

};
