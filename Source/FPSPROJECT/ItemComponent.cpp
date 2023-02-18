// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemActor.h"
#include "ItemComponent.h"

// Sets default values for this component's properties
UItemComponent::UItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int UItemComponent::AddToItem(AItemActor * item)
{
	return CurrentItem.Add(item); //아이템을 배열에 추가 및 반환
}

void UItemComponent::RemoveItem(AItemActor * item)
{
	CurrentItem.Remove(item); //아이템을 배열에서 제거
}

