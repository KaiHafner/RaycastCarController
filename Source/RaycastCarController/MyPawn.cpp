// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up the car body mesh
	CarBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarBody"));
	RootComponent = CarBody;

	// Initialize wheel components to nullptr
	WheelFL = nullptr;
	WheelFR = nullptr;
	WheelRL = nullptr;
	WheelRR = nullptr;
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

	// Directly reference the existing wheel components placed in the Blueprint
	WheelFL = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("FL_Wheel"))); // Ensure names match what you have in the Blueprint
	WheelFR = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("FR_Wheel")));
	WheelRL = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("RL_Wheel")));
	WheelRR = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("RR_Wheel")));
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Apply suspension at each wheel
	if (WheelFL && WheelFR && WheelRL && WheelRR)
	{
		ApplySuspensionForce(WheelFL->GetComponentLocation(), WheelFL->GetComponentLocation() - FVector(0, 0, SuspensionRest), DeltaTime);
		ApplySuspensionForce(WheelFR->GetComponentLocation(), WheelFR->GetComponentLocation() - FVector(0, 0, SuspensionRest), DeltaTime);
		ApplySuspensionForce(WheelRL->GetComponentLocation(), WheelRL->GetComponentLocation() - FVector(0, 0, SuspensionRest), DeltaTime);
		ApplySuspensionForce(WheelRR->GetComponentLocation(), WheelRR->GetComponentLocation() - FVector(0, 0, SuspensionRest), DeltaTime);
	}
}

void AMyPawn::ApplySuspensionForce(FVector Start, FVector End, float DeltaTime)
{
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	// Perform the raycast
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params);

	if (bHit)
	{
		FVector Velocity = CarBody->GetComponentVelocity();

		// Calculate the suspension force only if the ray hits something
		float DistanceToGround = (Start - HitResult.ImpactPoint).Size(); 
		SuspensionOffset = SuspensionRest - DistanceToGround;
		SuspensionForce = (SuspensionOffset * SpringStrength) - (SpringDamper * Velocity.Z);

		CarBody->AddForceAtLocation(FVector(0, 0, SuspensionForce), HitResult.ImpactPoint);

		// Draw a green debug line to indicate a successful hit
		DrawDebugLine(GetWorld(), Start, HitResult.ImpactPoint, FColor::Green, false, -1, 0, 2);
	}
	else
	{
		// Draw a red debug line to indicate a miss
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, -1, 0, 2);
	}
}


// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
