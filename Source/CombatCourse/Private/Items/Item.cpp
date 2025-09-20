// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "DrawDebugHelpers.h"
#include "FrameTypes.h"
#include "MovieSceneObjectBindingID.h"
#include "CombatCourse/DebugMacros.h"
using namespace std;

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
}

float AItem::TransformSine() const
{
	return Amplitude * FMath::Sin(ElapsedTime * Frequency);
}

float AItem::TransformCosine() const
{
	return Amplitude * FMath::Cos(ElapsedTime * Frequency);
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	const FVector Location = GetActorLocation();

	ElapsedTime += DeltaTime;
	const float DesiredHeightOffset = TransformSine();
	AddActorWorldOffset(FVector(0, 0, DesiredHeightOffset) * DeltaTime);
}



