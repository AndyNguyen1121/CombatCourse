// Fill out your copyright notice in the Description page of Project Settings.


#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;

	// Collider
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(FName("Capsule"));
	SetRootComponent(Capsule);

	// Mesh
	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>("BirdMesh");
	BirdMesh->SetupAttachment(Capsule);

	// Spring Arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(Capsule);
	SpringArm->TargetArmLength = 300.f;

	// Camera
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void ABird::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(BirdMappingContext, 0);
		}
	}
}

void ABird::Move(const FInputActionValue& Value)
{
	if (!GetController())
		return;
	
	FVector2D MovementInput = Value.Get<FVector2D>();

	if (!MovementInput.IsNearlyZero())
	{
		MovementInput.Normalize();
		AddMovementInput(GetActorForwardVector(), MovementInput.Y);
		AddMovementInput(GetActorRightVector(), MovementInput.X);
	}
	
	GEngine->AddOnScreenDebugMessage(0, 3, FColor::Red, FString::Printf(TEXT("%s"), *MovementInput.ToString()));
}

void ABird::Turn(const FInputActionValue& Value)
{
	if (!GetController())
		return;
	
	FVector2D RotationInput = Value.Get<FVector2D>() * 3;

	ControlRotation = Controller->GetControlRotation();

	ControlRotation.Pitch = FMath::Clamp(ControlRotation.Pitch + RotationInput.Y, -75.f, 75.f);
	ControlRotation.Yaw += RotationInput.X;

	Controller->SetControlRotation(ControlRotation);
	
	GEngine->AddOnScreenDebugMessage(1, 3, FColor::Red, FString::Printf(TEXT("%s"), *RotationInput.ToString()));
}

void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FRotator SlerpRotation = FMath::RInterpTo(
		GetActorRotation(),
		ControlRotation,
		GetWorld()->GetDeltaSeconds(),
		10.f);
	
	SetActorRotation(SlerpRotation);
}

void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::Move);
		EnhancedInputComponent->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ABird::Turn);
	}

}

