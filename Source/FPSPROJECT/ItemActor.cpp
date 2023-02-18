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
	//�ݸ��� �������� Pawn���� ����
	
	MyMesh->SetMobility(EComponentMobility::Movable);
	SetActorEnableCollision(true); //�ݸ��� Ȱ��ȭ

}

void AItemActor::PickUp()
{
	//��Ȱ��ȭ �ϴ� ��� 3������ �� ���־�� ��
	SetActorTickEnabled(false); //ƽ �Լ� ��Ȱ��ȭ
	SetActorHiddenInGame(true); //���͸� ������
	SetActorEnableCollision(false); //�浹 ��Ȱ��ȭ
}

void AItemActor::PutDown(FTransform pos)
{
	SetActorTickEnabled(true); //ƽ�Լ� Ȱ��ȭ
	SetActorHiddenInGame(false); //���͸� ������
	SetActorEnableCollision(true); //�浹 Ȱ��ȭ
	SetActorLocation(pos.GetLocation());
}
