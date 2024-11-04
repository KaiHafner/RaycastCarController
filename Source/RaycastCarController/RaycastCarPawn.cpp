#include "RaycastCarPawn.h"
#include "Components/StaticMeshComponent.h"
#include "DrawDebugHelpers.h"

ARaycastCarPawn::ARaycastCarPawn()
{
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

void ARaycastCarPawn::BeginPlay()
{
    Super::BeginPlay();

    // Directly reference the existing wheel components placed in the Blueprint
    WheelFL = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("FL_Wheel"))); // Ensure names match what you have in the Blueprint
    WheelFR = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("FR_Wheel")));
    WheelRL = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("RL_Wheel")));
    WheelRR = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("RR_Wheel")));
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
    if (WheelFL && WheelFR && WheelRL && WheelRR)
    {
        ApplySuspensionForce(WheelFL->GetComponentLocation(), WheelFL->GetComponentLocation() - FVector(0, 0, SuspensionMaxLength), DeltaTime);
        ApplySuspensionForce(WheelFR->GetComponentLocation(), WheelFR->GetComponentLocation() - FVector(0, 0, SuspensionMaxLength), DeltaTime);
        ApplySuspensionForce(WheelRL->GetComponentLocation(), WheelRL->GetComponentLocation() - FVector(0, 0, SuspensionMaxLength), DeltaTime);
        ApplySuspensionForce(WheelRR->GetComponentLocation(), WheelRR->GetComponentLocation() - FVector(0, 0, SuspensionMaxLength), DeltaTime);
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
        // Calculate the distance from the start to the impact point
        float DistanceToGround = (Start - HitResult.ImpactPoint).Size();

        // Check if the raycast distance is less than the maximum suspension length
        if (DistanceToGround < SuspensionMaxLength)
        {
            // Normalize the distance to a value between 0 and 1
            float NormalizedDistance = FMath::Clamp(1.0f - (DistanceToGround / SuspensionMaxLength), 0.0f, 1.0f);

            // Calculate forces based on normalized distance
            float SpringForce = NormalizedDistance * SuspensionStrength; // Adjust the spring force according to normalized distance
            float DampingForce = SuspensionDamping * FVector::DotProduct(CarBody->GetComponentVelocity(), FVector::DownVector);
            float TotalForce = SpringForce - DampingForce;

            // Apply force at the hit location
            CarBody->AddForceAtLocation(FVector(0, 0, TotalForce), HitResult.ImpactPoint);

            // Draw debug line
            DrawDebugLine(GetWorld(), Start, HitResult.ImpactPoint, FColor::Green, false, -1, 0, 2);
        }
    }
    else
    {
        DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, -1, 0, 2);
    }
}
