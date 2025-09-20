// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class COMBATCOURSE_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AItem();
	virtual void Tick(float DeltaTime) override;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float Amplitude = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float Frequency = 0.5f;

	UFUNCTION(BlueprintPure)
	float TransformSine() const;

	UFUNCTION(BlueprintPure)
	float TransformCosine() const;

	template<typename T>
	T Avg(T First, T Second);
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = "Sine Parameters")
	float ElapsedTime;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemMesh;

	
};

template <typename T>
T AItem::Avg(T First, T Second)
{
	return (First + Second) / 2;
}
