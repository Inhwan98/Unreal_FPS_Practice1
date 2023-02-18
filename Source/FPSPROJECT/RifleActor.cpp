// Fill out your copyright notice in the Description page of Project Settings.

#include "CPlayer.h"

#include "Undead.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Classes/Components/StaticMeshComponent.h"
#include "IRifle.h"
#include "Engine/StaticMeshActor.h"
#include "DrawDebugHelpers.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine.h"
#include "Components/BoxComponent.h"
#include "Global.h"
#include "RifleActor.h"

//팩토리 구조
//스폰한다.
ARifleActor::ARifleActor()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset
	(TEXT("SkeletalMesh'/Game/Weapons/Meshes/AR4/SK_AR4.SK_AR4'"));
	Mesh->SetSkeletalMesh(MeshAsset.Object);

	ConstructorHelpers::FObjectFinder<UAnimMontage> GrabMtgAsset
	(TEXT("AnimMontage'/Game/Character/Montages/Rifle_Grab_Montage.Rifle_Grab_Montage'"));
	GrabMontage = GrabMtgAsset.Object;

	ConstructorHelpers::FObjectFinder<UAnimMontage> UnGrabMtgAsset
	(TEXT("AnimMontage'/Game/Character/Montages/Rifle_UnGrab_Montage.Rifle_UnGrab_Montage'"));
	UnGrabMontage = UnGrabMtgAsset.Object;

	ConstructorHelpers::FObjectFinder<UAnimMontage> FireMtgAsset
	(TEXT("AnimMontage'/Game/Character/Montages/Rifle_Fire_Montage.Rifle_Fire_Montage'"));
	FireMontage = FireMtgAsset.Object;

	ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset
	(TEXT("ParticleSystem'/Game/Weapons/RifleFire.RifleFire'"));
	HitFX = ParticleAsset.Object;

	ConstructorHelpers::FObjectFinder<UParticleSystem> FireFX_ParticleAsset
	(TEXT("ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
	FireFX = FireFX_ParticleAsset.Object;

	static ConstructorHelpers::FObjectFinder<USoundCue> FireSoundCueObject(TEXT("SoundCue'/Game/Audio/Fire_Sound_Cue.Fire_Sound_Cue'"));
	if (FireSoundCueObject.Succeeded())
	{
		FireSoundCue = FireSoundCueObject.Object;

		//UAudioComponent 인스턴스인 BGMAudioComponent에 값 입력
		FireAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("FireAudioComponent"));
		FireAudioComponent->SetupAttachment(RootComponent);
	}

	
}

void ARifleActor::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	
	AttachToComponent(OwnerCharacter->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), HolsterSocket);
	//EAttachmentRule : 컴포넌트 상속 규칙
	//KeepRelative : 현재의 상대변환(위치, 회전, 스케일)을 모두 부모의 상대변환에 맞춤

	if (FireAudioComponent && FireSoundCue)
	{
		FireAudioComponent->SetSound(FireSoundCue);
		FireAudioComponent->SetPitchMultiplier(FMath::RandRange(1.0f, 1.3f));
	}

}

void ARifleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if (bAiming)
	{
		IIRifle* rifle = Cast<IIRifle>(OwnerCharacter);
		if (!!rifle)
		{
			FVector start, end, direction;
			rifle->GetLocationAndDirection(start, end, direction);

			//DrawDebugLine(GetWorld(), start, end, FColor::Green, false, 3.0f);
			
			FCollisionQueryParams params;
			params.AddIgnoredActor(this);
			params.AddIgnoredActor(OwnerCharacter);
			FHitResult hitResult;
			if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_WorldDynamic, params))
			{
				AActor* undeadActor = Cast<AActor>(hitResult.GetActor());
				if (hitResult.GetActor()->IsA(AUndead::StaticClass()))
				{
					UCapsuleComponent* meshComponent = Cast<UCapsuleComponent>(undeadActor->GetRootComponent());
					rifle->OnFocus();
					return;

				}
				
				AStaticMeshActor* staticMeshActor = Cast<AStaticMeshActor>(hitResult.GetActor());
				if (!!staticMeshActor)
				{
					UStaticMeshComponent* meshComponent = Cast<UStaticMeshComponent>(staticMeshActor->GetRootComponent());
					if (!!meshComponent && meshComponent->BodyInstance.bSimulatePhysics)
					{
						
						rifle->OnFocus();
						return;
					}
				}
			}
			rifle->OffFocus();
		}
	}
}

ARifleActor* ARifleActor::Spawn(UWorld* InWord, ACharacter* InOwner)
{
	FActorSpawnParameters params;
	params.Owner = InOwner;
	return InWord->SpawnActor<ARifleActor>(params);
}

void ARifleActor::Equip()
{
	if (!bEquipped && !bEquipping)
	{
		bEquipping = true;
		OwnerCharacter->PlayAnimMontage(GrabMontage);
	}
}

void ARifleActor::BeginEquip()
{
	bEquipped = true;
	AttachToComponent(OwnerCharacter->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), HandSocket);
	
}

void ARifleActor::EndEquip()
{
	bEquipping = false;
}

void ARifleActor::Unequip()
{
	if (bEquipped && !bEquipping)
	{
		bEquipping = true;
		OwnerCharacter->PlayAnimMontage(UnGrabMontage);
	}
}
void ARifleActor::BeginUnequip()
{
	bEquipped = false;
	AttachToComponent(OwnerCharacter->GetMesh(),
		FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), HolsterSocket);
}

void ARifleActor::EndUnEquip()
{
	bEquipping = false;
}

void ARifleActor::BeginAiming()
{
	bAiming = true;
}

void ARifleActor::EndAiming()
{
	bAiming = false;
}

void ARifleActor::BeginFire()
{
	if (bEquipped && !bEquipping && bAiming && !bFiring)
	{
		Firing();
	}
}

void ARifleActor::Firing()
{
	IIRifle* rifle = Cast<IIRifle>(OwnerCharacter);
	if (!!rifle)
	{
		FVector start, end, direction;
		rifle->GetLocationAndDirection(start, end, direction);
		
		OwnerCharacter->PlayAnimMontage(FireMontage);

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitFX, OwnerCharacter->GetMesh()->GetSocketLocation(GunSocket));
		//파티클 생성

		FCollisionQueryParams params;
		FActorSpawnParameters SpawnParams;
		params.AddIgnoredActor(this);
		params.AddIgnoredActor(OwnerCharacter);
		FHitResult hitResult;
		if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_WorldDynamic, params))
		{
			FireAudioComponent->Play(0.f);
			AActor* undeadActor = Cast<AActor>(hitResult.GetActor());
			if (hitResult.GetActor()->IsA(AUndead::StaticClass()))
			{
				UGameplayStatics::ApplyDamage(hitResult.GetActor(), 10.f, NULL, this, UDamageType::StaticClass());
				//데미지 전달

				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireFX, undeadActor->GetActorLocation());
				//파티클 생성

				direction = undeadActor->GetActorLocation() - OwnerCharacter->GetActorLocation();
				
				/*meshComponent->AddImpulseAtLocation(direction * meshComponent->GetMass() * 1, OwnerCharacter->GetActorLocation());*/

				return;
				
			}

			AStaticMeshActor* staticMeshActor = Cast<AStaticMeshActor>(hitResult.GetActor());
			if (!!staticMeshActor)
			{
				UStaticMeshComponent* meshComponent = Cast<UStaticMeshComponent>(staticMeshActor->GetRootComponent());
				if (!!meshComponent && meshComponent->BodyInstance.bSimulatePhysics )
				{
					

					direction = staticMeshActor->GetActorLocation() - OwnerCharacter->GetActorLocation();
					//액터가 바라보는 방향이 나온다.
					
					meshComponent->AddImpulseAtLocation(direction * meshComponent->GetMass() * 1, OwnerCharacter->GetActorLocation());
					return;
					//
				}
			}
		}
	}
}

void ARifleActor::EndFire()
{
}

void ARifleActor::DieOwner()
{
	Mesh->BodyInstance.SetCollisionEnabled(ECollisionEnabled::NoCollision);
	this->SetActorHiddenInGame(true); //액터를 감춰줌
}

