#include "RaycastCarPawn.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

ARaycastCarPawn::ARaycastCarPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set up the car body mesh
	CarBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CarBody"));
	RootComponent = CarBody;

	// Set up wheel offsets (relative to the car body)
	WheelOffsets.Add(FVector(50, 150, 50));   // Front-right
	WheelOffsets.Add(FVector(50, -150, 50));  // Front-left
	WheelOffsets.Add(FVector(-50, 150, 50));  // Rear-right
	WheelOffsets.Add(FVector(-50, -150, 50)); // Rear-left
}

void ARaycastCarPawn::BeginPlay()
{
	Super::BeginPlay();
}

void ARaycastCarPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Apply movement based on input
	FVector ForwardForceVector = CarBody->GetForwardVector() * CurrentForwardInput * ForwardForce;
	CarBody->AddForce(ForwardForceVector);

	// Apply turning
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += CurrentTurnInput * TurnRate * DeltaTime;
	SetActorRotation(NewRotation);

	// Apply suspension at each wheel
	for (const FVector& Offset : WheelOffsets)
	{
		FVector Start = CarBody->GetComponentLocation() + Offset;
		FVector End = Start - FVector(0, 0, SuspensionMaxLength);
		ApplySuspensionForce(Start, End, DeltaTime);
	}
}

void ARaycastCarPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind input functions
	PlayerInputComponent->BindAxis("MoveForward", this, &ARaycastCarPawn::MoveForward);
	PlayerInputComponent->BindAxis("Turn", this, &ARaycastCarPawn::Turn);
}

void ARaycastCarPawn::MoveForward(float Value)
{
	CurrentForwardInput = Value;
}

void ARaycastCarPawn::Turn(float Value)
{
	CurrentTurnInput = Value;
}

void ARaycastCarPawn::ApplySuspensionForce(FVector Start, FVector End, float DeltaTime)
{
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	// Perform the raycast
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		float DistanceToGround = (Start - HitResult.ImpactPoint).Size();
		FString DistanceMessage = FString::Printf(TEXT("Distance to Ground: %.2f"), DistanceToGround);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, DistanceMessage);

		float CompressionRatio = 100.0f - (DistanceToGround / SuspensionMaxLength);

		// Calculate forces
		float SpringForce = CompressionRatio * SuspensionStrength;
		float DampingForce = SuspensionDamping * FVector::DotProduct(CarBody->GetComponentVelocity(), FVector::DownVector);
		float TotalForce = SpringForce - DampingForce;

		float Weight = 1600.0f * 980.0f;
		float TotalSuspensionForce = TotalForce + (Weight / 4); // Divide weight by number of wheels

		// Apply force at the hit location
		CarBody->AddForceAtLocation(FVector(0, 0, TotalSuspensionForce), HitResult.ImpactPoint);

		// Draw debug line
		DrawDebugLine(GetWorld(), Start, HitResult.ImpactPoint, FColor::Green, false, -1, 0, 2);
	}
	else
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, -1, 0, 2);
	}
}
