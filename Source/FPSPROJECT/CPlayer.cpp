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
	// * 카메라 생성
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	
	// * 이동속도 및 회전
	bUseControllerRotationYaw = false;
	// 회전 중지
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;


	// * 스켈레탈 메쉬 적용
	ConstructorHelpers::FObjectFinder<USkeletalMesh>
		asset1(L"SkeletalMesh'/Game/Sci_Fi_Character_08/Mesh/Character/SK_Sci_Fi_Character_08_Full_02.SK_Sci_Fi_Character_08_Full_02'");
	verifyf(asset1.Succeeded(), L"assetSucceedsd()");
	GetMesh()->SetSkeletalMesh(asset1.Object);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	// * 카메라 위치 세팅
	SpringArm->SetRelativeLocation(FVector(0, 0, 60));
	SpringArm->TargetArmLength = 200.f; //카메라 거리
	SpringArm->bDoCollisionTest = false; //콜리전 반응 X
	SpringArm->bUsePawnControlRotation = true; //컨트롤러에 따른 회전
	SpringArm->SocketOffset = FVector(0, 60, 0);


	// * 애니메이션 블루프린트 세팅
	TSubclassOf<UAnimInstance> animInstance;
	ConstructorHelpers::FClassFinder<UAnimInstance> asset2(L"AnimBlueprint'/Game/BluePrint/CBP_CPlayer.CBP_CPlayer_C'");
	verifyf(asset2.Succeeded(), L"asset2.Succeeded()");
	animInstance = asset2.Class;
	GetMesh()->SetAnimInstanceClass(animInstance);

	ConstructorHelpers::FClassFinder<UCUserWidget_CrossHair> WidgetAsset(L"WidgetBlueprint'/Game/BluePrint/WB_CrossHair.WB_CrossHair_C'");
	//블루프린트는 경로 마지막 뒤에 _C를 넣어주어야 한다.
	CrossHairWidget = WidgetAsset.Class;

	stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	//지정된 감지 클래스의 소스로 등록
	stimulus->RegisterWithPerceptionSystem();
	//인식 시스템 등록

	//load the sound cue object
	static ConstructorHelpers::FObjectFinder<USoundCue> BGMSoundCueObject(TEXT("SoundCue'/Game/Audio/NewSoundCue.NewSoundCue'"));
	if (BGMSoundCueObject.Succeeded())
	{
		BGMSoundCue = BGMSoundCueObject.Object;

		//UAudioComponent 인스턴스인 BGMAudioComponent에 값 입력
		BGMAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMAudioComponent"));
		BGMAudioComponent->SetupAttachment(RootComponent);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> WalkSoundCueObject(TEXT("SoundCue'/Game/Audio/Walk_sound_Cue.Walk_sound_Cue'"));
	if (BGMSoundCueObject.Succeeded())
	{
		WalkSoundCue = WalkSoundCueObject.Object;

		//UAudioComponent 인스턴스인 BGMAudioComponent에 값 입력
		WalkAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("WalkAudioComponent"));
		WalkAudioComponent->SetupAttachment(RootComponent);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> TakeDamageSoundCueObject(TEXT("SoundCue'/Game/Audio/TakeDamgae_Cue.TakeDamgae_Cue'"));
	if (TakeDamageSoundCueObject.Succeeded())
	{
		TakeDamageSoundCue = TakeDamageSoundCueObject.Object;

		//UAudioComponent 인스턴스인 BGMAudioComponent에 값 입력
		TakeDamageAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("TakeDamageAudioComponent"));
		TakeDamageAudioComponent->SetupAttachment(RootComponent);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> DieSoundCueObject(TEXT("SoundCue'/Game/Audio/DieSound_Cue.DieSound_Cue'"));
	if (DieSoundCueObject.Succeeded())
	{
		DieSoundCue = DieSoundCueObject.Object;

		//UAudioComponent 인스턴스인 BGMAudioComponent에 값 입력
		DieAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("DieAudioComponent"));
		DieAudioComponent->SetupAttachment(RootComponent);
	}

	bIsPaused = false;

	maxHP = 100.0f;
	currentHp = maxHP;

	//아이템
	MyInventory = CreateDefaultSubobject<UItemComponent>("MyInventory");
	//Itemcomponent를 생성
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
	//함수포인터를 사용할때는 함수의 주소로 명시해준다.
	// * 이동
	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);

	// * 회전
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::OnHorizontalLook);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::OnVerticalLook);

	// * 달리기
	PlayerInputComponent->BindAction("Running", EInputEvent::IE_Pressed, this, &ACPlayer::OnRunning);
	PlayerInputComponent->BindAction("Running", EInputEvent::IE_Released, this, &ACPlayer::OffRunning);

	// * 점프

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// * 무기 장착
	PlayerInputComponent->BindAction("Rifle", EInputEvent::IE_Pressed, this, &ACPlayer::OnRifle);

	// * 줌인
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Pressed, this, &ACPlayer::OnAim);
	PlayerInputComponent->BindAction("Aim", EInputEvent::IE_Released, this, &ACPlayer::OffAim);
	// * 사격
	/*PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &ACPlayer::CreateBullet);*/

	// * 타겟
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
	//컨트롤러의 Yaw축을 가져옴
	FVector direction = FQuat(rotator).GetForwardVector().GetUnsafeNormal2D();
	//GetUnsafeNormal2D : 길이가 0인지 확인하지 않고 길이를 구하는 함수
	//Yaw축의 앞방향을 direction에 넣어줌
	AddMovementInput(direction, axis);
	//이동
}

void ACPlayer::OnMoveRight(float axis)
{
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector().GetUnsafeNormal2D();
	AddMovementInput(direction, axis);
	//이동
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

// * 에임 ON/OFF
void ACPlayer::OnAim()
{
	if (Rifle->GetEquipped() && !Rifle->GetEquipping())
	{
		bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		//회전을 중지
		SpringArm->TargetArmLength = 100;
		SpringArm->SocketOffset = FVector(0, 30, 10);
		//SocketOffset : 오브젝트의 소켓 위치(따라다닐 중심 위치)
		Camera->FieldOfView = 40;
		//FieldOfView : 카메라 시야 범위
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
		//회전을 중지
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
	OutDirection = Camera->GetForwardVector(); //카메라의 앞방향
	FTransform transform = Camera->GetComponentTransform();
	FVector cameraLocation = transform.GetLocation();
	OutStart = cameraLocation + OutDirection; //카메라의 위치로부터
	FVector conDirection = UKismetMathLibrary::RandomUnitVectorInEllipticalConeInDegrees(OutDirection, 0.2f, 0.3f);
	//RandomUnitVectorInEllipticalConeInDegrees : 지정된 원뿔 내에 길이가 1인 랜덤 벡터를 균일한 랜덤 분포로 반환
	//총알을 약간 랜덤하게 발사시켜주기 위해 사용
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
	
	//맞는 애니메이션 재생
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
	//콜리전 없음
	GetCharacterMovement()->StopMovementImmediately();
	//이동 중지
	GetCharacterMovement()->DisableMovement();
	//이동 비활성화
	Controller->UnPossess();
	//컨트롤러와 연결 해제
	Rifle->DieOwner();
	this->SetActorHiddenInGame(true); //액터를 감춰줌

	
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

//드롭

void ACPlayer::DropItem()
{

	if (MyInventory->CurrentItem.Num() == 0) //배열에 저장된 아이템이 없다면
		return; //함수 종료
	AItemActor* item = MyInventory->CurrentItem.Last();
	//배열의 마지막 인덱스에 있는 아이템을 참조
	MyInventory->RemoveItem(item); //배열에서 제거

	FTransform pos = GetTransform() + FTransform(RootComponent->GetForwardVector() * 100); // 현재 위치에서 앞방향에 100만큼 떨어진 거리
	item->PutDown(pos); //아이템 활성화
}

void ACPlayer::TakeItem(AItemActor * item)
{
	item->PickUp(); //아이템 비활성화
	MyInventory->AddToItem(item); //아이템 배열에 추가
}

void ACPlayer::NotifyHit(UPrimitiveComponent * MyComp, AActor * Other, UPrimitiveComponent * OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult & Hit)
{
	if (Other->IsA(AItemActor::StaticClass()))
	{//충돌한 상대 액터가 ItemActor클래스 타입이라면
		AItemActor* item = Cast<AItemActor>(Other);
		//충돌한 상대 액터를 AItemActor클래스로 변환
		TakeItem(item);
	}
}