// Fill out your copyright notice in the Description page of Project Settings.
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Engine.h"

#include "ItemComponent.h"
#include "ItemActor.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraShake.h"

#include "Kismet/KismetMathLibrary.h"
#include "CUserWidget_CrossHair.h"
#include "RifleActor.h"
#include "CAnimInstance.h"

#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

#include "CPlayer.h"

// Sets default values
ACPlayer::ACPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// * ī�޶� ����
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	
	// * �̵��ӵ� �� ȸ��
	bUseControllerRotationYaw = false;
	// ȸ�� ����
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;


	// * ���̷�Ż �޽� ����
	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		asset1(L"SkeletalMesh'/Game/Sci_Fi_Character_08/Mesh/Character/SK_Sci_Fi_Character_08_Full_02.SK_Sci_Fi_Character_08_Full_02'");
	verifyf(asset1.Succeeded(), L"assetSucceedsd()");
	GetMesh()->SetSkeletalMesh(asset1.Object);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	// * ī�޶� ��ġ ����
	SpringArm->SetRelativeLocation(FVector(0, 0, 60));
	SpringArm->TargetArmLength = 200.f; //ī�޶� �Ÿ�
	SpringArm->bDoCollisionTest = false; //�ݸ��� ���� X
	SpringArm->bUsePawnControlRotation = true; //��Ʈ�ѷ��� ���� ȸ��
	SpringArm->SocketOffset = FVector(0, 60, 0);


	// * �ִϸ��̼� �������Ʈ ����
	TSubclassOf<UAnimInstance> animInstance;
	ConstructorHelpers::FClassFinder<UAnimInstance> asset2(L"AnimBlueprint'/Game/BluePrint/CBP_CPlayer.CBP_CPlayer_C'");
	verifyf(asset2.Succeeded(), L"asset2.Succeeded()");
	animInstance = asset2.Class;
	GetMesh()->SetAnimInstanceClass(animInstance);

	ConstructorHelpers::FClassFinder<UCUserWidget_CrossHair> WidgetAsset(L"WidgetBlueprint'/Game/BluePrint/WB_CrossHair.WB_CrossHair_C'");
	//�������Ʈ�� ��� ������ �ڿ� _C�� �־��־�� �Ѵ�.
	CrossHairWidget = WidgetAsset.Class;

	stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	//������ ���� Ŭ������ �ҽ��� ���
	stimulus->RegisterWithPerceptionSystem();
	//�ν� �ý��� ���

	//load the sound cue object
	static ConstructorHelpers::FObjectFinder<USoundCue> BGMSoundCueObject(TEXT("SoundCue'/Game/Audio/NewSoundCue.NewSoundCue'"));
	if (BGMSoundCueObject.Succeeded())
	{
		BGMSoundCue = BGMSoundCueObject.Object;

		//UAudioComponent �ν��Ͻ��� BGMAudioComponent�� �� �Է�
		BGMAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMAudioComponent"));
		BGMAudioComponent->SetupAttachment(RootComponent);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> WalkSoundCueObject(TEXT("SoundCue'/Game/Audio/Walk_sound_Cue.Walk_sound_Cue'"));
	if (BGMSoundCueObject.Succeeded())
	{
		WalkSoundCue = WalkSoundCueObject.Object;

		//UAudioComponent �ν��Ͻ��� BGMAudioComponent�� �� �Է�
		WalkAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("WalkAudioComponent"));
		WalkAudioComponent->SetupAttachment(RootComponent);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> TakeDamageSoundCueObject(TEXT("SoundCue'/Game/Audio/TakeDamgae_Cue.TakeDamgae_Cue'"));
	if (TakeDamageSoundCueObject.Succeeded())
	{
		TakeDamageSoundCue = TakeDamageSoundCueObject.Object;

		//UAudioComponent �ν��Ͻ��� BGMAudioComponent�� �� �Է�
		TakeDamageAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("TakeDamageAudioComponent"));
		TakeDamageAudioComponent->SetupAttachment(RootComponent);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> DieSoundCueObject(TEXT("SoundCue'/Game/Audio/DieSound_Cue.DieSound_Cue'"));
	if (DieSoundCueObject.Succeeded())
	{
		DieSoundCue = DieSoundCueObject.Object;

		//UAudioComponent �ν��Ͻ��� BGMAudioComponent�� �� �Է�
		DieAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("DieAudioComponent"));
		DieAudioComponent->SetupAttachment(RootComponent);
	}

	bIsPaused = false;

	maxHP = 100.0f;
	currentHp = maxHP;

	//������
	MyInventory = CreateDefaultSubobject<UItemComponent>("MyInventory");
	//Itemcomponent�� ����
}

// Called when the game starts or when spawned
void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	Rifle = ARifleActor::Spawn(GetWorld(), this);
	
	OnRifle();
	
	CrossHair = CreateWidget<UCUserWidget_CrossHair, APlayerController>(GetController<APlayerController>(), CrossHairWidget);
	CrossHair->AddToViewport();
	CrossHair->SetVisibility(ESlateVisibility::Hidden);
	
	if (BGMAudioComponent && BGMSoundCue && WalkAudioComponent && WalkSoundCue && TakeDamageAudioComponent && TakeDamageSoundCue
		&& DieAudioComponent && DieSoundCue)
	{
		TakeDamageAudioComponent->SetSound(TakeDamageSoundCue);
		DieAudioComponent->SetSound(DieSoundCue);
		WalkAudioComponent->SetSound(WalkSoundCue);
		BGMAudioComponent->SetSound(BGMSoundCue);
		BGMAudioComponent->Play();
	}

}

// Called every frame
void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

// Called to bind functionality to input
void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//�Լ������͸� ����Ҷ��� �Լ��� �ּҷ� ������ش�.
	// * �̵�
	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);

	// * ȸ��
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);

	// * �޸���
	PlayerInputComponent->BindAction("Running", EInputEvent::IE_Pressed, this, &ACPlayer::OnRunning);
	PlayerInputComponent->BindAction("Running", EInputEvent::IE_Released, this, &ACPlayer::OffRunning);

	// * ����

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// * ���� ����
	PlayerInputComponent->BindAction("Rifle", EInputEvent::IE_Pressed, this, &ACPlayer::OnRifle);

	// * ����
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &ACPlayer::OnAim);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &ACPlayer::OffAim);
	// * ���
	/*PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &ACPlayer::CreateBullet);*/

	// * Ÿ��
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &ACPlayer::OnFire);
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &ACPlayer::OffFire);

	PlayerInputComponent->BindAction("GamePause", EInputEvent::IE_Pressed, this, &ACPlayer::OnGamePause);

	// * drop
	PlayerInputComponent->BindAction("Drop", EInputEvent::IE_Pressed, this,
		&ACPlayer::DropItem);

}

void ACPlayer::OnMoveForward(float axis)
{
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	//��Ʈ�ѷ��� Yaw���� ������
	FVector direction = FQuat(rotator).GetForwardVector().GetUnsafeNormal2D();
	//GetUnsafeNormal2D : ���̰� 0���� Ȯ������ �ʰ� ���̸� ���ϴ� �Լ�
	//Yaw���� �չ����� direction�� �־���
	AddMovementInput(direction, axis);
	//�̵�
}

void ACPlayer::OnMoveRight(float axis)
{
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector().GetUnsafeNormal2D();
	AddMovementInput(direction, axis);
	//�̵�
}

void ACPlayer::OnHorizontalLook(float axis)
{
	AddControllerYawInput(axis);
}

void ACPlayer::OnVerticalLook(float axis)
{
	AddControllerPitchInput(axis);
}

void ACPlayer::OnRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = 600;
}

void ACPlayer::OffRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = 400;
}

void ACPlayer::OnRifle()
{
	if (Rifle->GetEquipped())
	{
		Rifle->Unequip();
		return;
	}
	Rifle->Equip();
}

// * ���� ON/OFF
void ACPlayer::OnAim()
{
	if (Rifle->GetEquipped() && !Rifle->GetEquipping())
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		//ȸ���� ����
		SpringArm->TargetArmLength = 100;
		SpringArm->SocketOffset = FVector(0, 30, 10);
		//SocketOffset : ������Ʈ�� ���� ��ġ(����ٴ� �߽� ��ġ)
		Camera->FieldOfView = 40;
		//FieldOfView : ī�޶� �þ� ����
		OnZoomIn();
		Rifle->BeginAiming();

		CrossHair->SetVisibility(ESlateVisibility::Visible);
	}
}

void ACPlayer::OffAim()
{
	if (Rifle->GetEquipped() && !Rifle->GetEquipping())
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		//ȸ���� ����
		SpringArm->TargetArmLength = 200;
		SpringArm->SocketOffset = FVector(0, 60, 0);
		Camera->FieldOfView = 90;
		OnZoomOut();
		Rifle->EndAiming();

		CrossHair->SetVisibility(ESlateVisibility::Hidden);
	}
}

//////////

void ACPlayer::GetLocationAndDirection(FVector& OutStart, FVector& OutEnd, FVector& OutDirection)
{
	OutDirection = Camera->GetForwardVector(); //ī�޶��� �չ���
	FTransform transform = Camera->GetComponentTransform();
	FVector cameraLocation = transform.GetLocation();
	OutStart = cameraLocation + OutDirection; //ī�޶��� ��ġ�κ���
	FVector conDirection = UKismetMathLibrary::RandomUnitVectorInEllipticalConeInDegrees(OutDirection, 0.2f, 0.3f);
	//RandomUnitVectorInEllipticalConeInDegrees : ������ ���� ���� ���̰� 1�� ���� ���͸� ������ ���� ������ ��ȯ
	//�Ѿ��� �ణ �����ϰ� �߻�����ֱ� ���� ���
	conDirection *= 3000.f;
	OutEnd = cameraLocation + conDirection;
}

void ACPlayer::OnFocus()
{
	CrossHair->OnFocus();
}

void ACPlayer::OffFocus()
{
	CrossHair->OffFocus();
}

void ACPlayer::OnFire()
{
	Rifle->BeginFire();
}

void ACPlayer::OffFire()
{
	Rifle->EndFire();
}

float ACPlayer::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	const float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("TakeDamege!"));
	currentHp -= damage;
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, FString::Printf(TEXT("HP : %f"), currentHp));
	
	//�´� �ִϸ��̼� ���
	TakeDamageAudioComponent->Play();

	if (currentHp <= 0)
	{
		Die();
	}

	return 0.0f;
}

void ACPlayer::Die()
{
	
	DieAudioComponent->Play();
	GetCapsuleComponent()->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//�ݸ��� ����
	GetCharacterMovement()->StopMovementImmediately();
	//�̵� ����
	GetCharacterMovement()->DisableMovement();
	//�̵� ��Ȱ��ȭ
	Controller->UnPossess();
	//��Ʈ�ѷ��� ���� ����
	Rifle->DieOwner();
	this->SetActorHiddenInGame(true); //���͸� ������

	
	EndWidget = CreateWidget(GetWorld(), EndWidgetClass);
	EndWidget->AddToViewport();
	
}

void ACPlayer::OnGamePause()
{
	APlayerController* const MyPlayer = Cast<APlayerController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	if (MyPlayer != NULL)
	{
		if (bIsPaused == true)
		{
			bIsPaused = false;
		}
		else bIsPaused = true;
		MyPlayer->SetPause(bIsPaused);

		CurrentWidget = CreateWidget(GetWorld(), PauseWidgetClass);
		CurrentWidget->AddToViewport();
		
		//they both can be compiled, but make the UE Editor crash
		
		MyPlayer->bShowMouseCursor = true;
		MyPlayer->bEnableClickEvents = true;
		MyPlayer->bEnableMouseOverEvents = true;
	}
}

//���

void ACPlayer::DropItem()
{

	if (MyInventory->CurrentItem.Num() == 0) //�迭�� ����� �������� ���ٸ�
		return; //�Լ� ����
	AItemActor* item = MyInventory->CurrentItem.Last();
	//�迭�� ������ �ε����� �ִ� �������� ����
	MyInventory->RemoveItem(item); //�迭���� ����

	FTransform pos = GetTransform() + FTransform(RootComponent->GetForwardVector() * 100); // ���� ��ġ���� �չ��⿡ 100��ŭ ������ �Ÿ�
	item->PutDown(pos); //������ Ȱ��ȭ
}

void ACPlayer::TakeItem(AItemActor * item)
{
	item->PickUp(); //������ ��Ȱ��ȭ
	MyInventory->AddToItem(item); //������ �迭�� �߰�
}

void ACPlayer::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	if (Other->IsA(AItemActor::StaticClass()))
	{//�浹�� ��� ���Ͱ� ItemActorŬ���� Ÿ���̶��
		AItemActor* item = Cast<AItemActor>(Other);
		//�浹�� ��� ���͸� AItemActorŬ������ ��ȯ
		TakeItem(item);
	}
}