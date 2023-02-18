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

public: //����
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

	//������
	class ARifleActor* Rifle;
	class ABullet* bullet;
	//����
	class UCUserWidget_CrossHair* CrossHair;

private: //�Լ�
	void OnMoveForward(float axis);
	void OnMoveRight(float axis);
	void OnHorizontalLook(float axis);
	void OnVerticalLook(float axis);

	//�޸���
	void OnRunning();
	void OffRunning();

	// *������ ����
	void OnRifle();

	void OnAim();
	void OffAim();

	void JumpAnim();
public:
	
	FORCEINLINE class ARifleActor* GetRifle() override { return Rifle; }

	//���ӽ���
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
	//- ���Ǵ� �������Ʈ���� �Ѵٴ� ��

	
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
		float currentHp; //���� ü��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
		float maxHP; //�ִ� ü��


	bool bIsPaused;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	//�������� ���� ���� ��� ȣ��

	void Die();

	void OnGamePause();

	UPROPERTY()
		class UAIPerceptionStimuliSourceComponent* stimulus;
	//AI���� �ð������� ������ �� �ְ� ���ִ� ������Ʈ


	//������
public:
	UPROPERTY()
		class UItemComponent* MyInventory;

	UFUNCTION()
		void DropItem();
	UFUNCTION()
		void TakeItem(class AItemActor* item);

	//��ϻ��� �浹 �Լ�
	UFUNCTION()
		virtual void NotifyHit(class UPrimitiveComponent* MyComp,
			AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved,
			FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)override;

};
