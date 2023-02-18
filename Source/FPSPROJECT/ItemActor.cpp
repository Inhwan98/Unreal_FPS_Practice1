// Fill out your copyright notice in the Description page of Project Settings.

#include "Engine/CollisionProfile.h"
#include "ItemActor.h"

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UStaticMeshComponent* MyMesh;
	MyMesh = CreateDefaultSubobject<UStaticMeshComponent>("MyMesh");
	auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>
		(TEXT("StaticMesh'/Game/ResearchMegaPack/ResearchFacility/Meshes/SM_Monitor_5.SM_Monitor_5'"));
	if (MeshAsset.Succeeded())
	{
		MyMesh->SetStaticMesh(MeshAsset.Object);
	}

	MyMesh->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	//콜리전 프리셋을 Pawn으로 변경
	
	MyMesh->SetMobility(EComponentMobility::Movable);
	SetActorEnableCollision(true); //콜리젼 활성화

}

void AItemActor::PickUp()
{
	//비활성화 하는 경우 3가지를 다 써주어야 함
	SetActorTickEnabled(false); //틱 함수 비활성화
	SetActorHiddenInGame(true); //액터를 감춰줌
	SetActorEnableCollision(false); //충돌 비활성화
}

void AItemActor::PutDown(FTransform pos)
{
	SetActorTickEnabled(true); //틱함수 활성화
	SetActorHiddenInGame(false); //액터를 보여줌
	SetActorEnableCollision(true); //충돌 활성화
	SetActorLocation(pos.GetLocation());
}
