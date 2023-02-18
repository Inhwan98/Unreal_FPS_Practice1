 // Fill out your copyright notice in the Description page of Project Settings.

#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CPlayer.h"
#include "Components/SphereComponent.h"

#include "Undead.h"

// Sets default values
AUndead::AUndead()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	currentHp = 30.0f;

	HitCollision = CreateDefaultSubobject<USphereComponent>(TEXT("HitCollision"));
	HitCollision->AttachTo(GetMesh(), "Head");
	//RightHand소켓에 부착
	HitCollision->OnComponentBeginOverlap.AddDynamic(this, &AUndead::MyBeginOverlap);

	
	static ConstructorHelpers::FObjectFinder<USoundCue> UndeadSoundCueObject(TEXT("SoundCue'/Game/Audio/Female_Monster_Cue.Female_Monster_Cue'"));
	if (UndeadSoundCueObject.Succeeded())
	{
		UndeadSoundCue = UndeadSoundCueObject.Object;
		
		//UAudioComponent 인스턴스인 BGMAudioComponent에 값 입력
		UndeadAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("UndeadAudioComponent"));
		UndeadAudioComponent->SetupAttachment(RootComponent);
	}


}

// Called when the game starts or when spawned
void AUndead::BeginPlay()
{
	Super::BeginPlay();

	if (UndeadAudioComponent && UndeadSoundCue)
	{
		UndeadAudioComponent->SetSound(UndeadSoundCue);
		UndeadAudioComponent->Play();
	}
	
}

// Called every frame
void AUndead::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AUndead::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AUndead::MeeleAttack_Implementation()
{
	PlayAnimMontage(AttackMontage);
	isAttack = false;
}

float AUndead::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	isDamage = true;

	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	PlayAnimMontage(HitMontage);

	currentHp -= Damage;
	if (currentHp <= 0)
	{
		isDie = true;
		PlayAnimMontage(DieMontage);

		FTimerHandle UnusedHandle;
		
		GetWorldTimerManager().SetTimer(UnusedHandle, this, &AUndead::Die, 3.f, false);
	}

	return Damage;
}

void AUndead::MyBeginOverlap(UPrimitiveComponent * HitComp, AActor * OtherActor,
	UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor->IsA(ACPlayer::StaticClass()) && OtherActor != this)
	{
		if (isAttack == false || isDie == false)
		{
			UGameplayStatics::ApplyDamage(OtherActor, 10.f, NULL, this, UDamageType::StaticClass());
			//충돌한 상대 액터에게 10의 데미지 전달

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("EnemyDamage!!"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("EnemyDamage!!"));
			isAttack = true;
		}
	}
}

void AUndead::Die()
{
	/*GetCapsuleComponent()->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();*/

	this->Destroy();
}


