// Fill out your copyright notice in the Description page of Project Settings.

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Characters/CombatCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ACombatCharacter::ACombatCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(FName("CameraComponent"));
	Camera->SetupAttachment(SpringArm);


	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = CastChecked<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(CombatContext, 0);
		}
	}
}

void ACombatCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller)
		return;
	
	FVector2D MovementVector = Value.Get<FVector2D>();
	MovementVector.Normalize();

	FVector WorldMovementDirection = GetActorForwardVector() * MovementVector.Y;
	WorldMovementDirection += GetActorRightVector() * MovementVector.X;
	AddMovementInput(WorldMovementDirection, 50 * GetWorld()->GetDeltaSeconds());
}

void ACombatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACombatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &ACombatCharacter::Move);
	}

}

