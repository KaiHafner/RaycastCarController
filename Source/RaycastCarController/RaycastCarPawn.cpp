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

    //Directly reference the existing wheel components placed in the Blueprint
    WheelFL = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("FL_Wheel")));
    WheelFR = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("FR_Wheel")));
    WheelRL = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("RL_Wheel")));
    WheelRR = Cast<USceneComponent>(GetDefaultSubobjectByName(TEXT("RR_Wheel")));
}

void ARaycastCarPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (WheelFL && WheelFR && WheelRL && WheelRR)
    {
        ApplySuspensionForce(WheelFL->GetComponentLocation(), WheelFL->GetComponentLocation() - FVector(0, 0, SuspensionRest), DeltaTime);
        ApplySuspensionForce(WheelFR->GetComponentLocation(), WheelFR->GetComponentLocation() - FVector(0, 0, SuspensionRest), DeltaTime);
        ApplySuspensionForce(WheelRL->GetComponentLocation(), WheelRL->GetComponentLocation() - FVector(0, 0, SuspensionRest), DeltaTime);
        ApplySuspensionForce(WheelRR->GetComponentLocation(), WheelRR->GetComponentLocation() - FVector(0, 0, SuspensionRest), DeltaTime);
    }
}


void ARaycastCarPawn::ApplySuspensionForce(FVector WheelLocation, FVector RayEnd, float DeltaTime)
{
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);  //Ignore the car itself

    //Perform raycast
    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, WheelLocation, RayEnd, ECC_Visibility, QueryParams);

    if (bHit)
    {
        float Compression = SuspensionRest - HitResult.Distance; //How much suspension is compressed
        float SpringForce = SuspensionStiffness * Compression; //hookes Law
        float Velocity = FVector::DotProduct(GetVelocity(), FVector(0, 0, 1)); //Velocity in Z
        float DampingForce = SuspensionDamping * Velocity;

        FVector Force = FVector(0, 0, SpringForce - DampingForce); //Net force

        CarBody->AddForceAtLocation(Force, WheelLocation);

        //Debug draw
        DrawDebugLine(GetWorld(), WheelLocation, HitResult.Location, FColor::Green, false, 0.1f, 0, 2);
        DrawDebugPoint(GetWorld(), HitResult.Location, 5, FColor::Red, false, 0.1f);

        DrawDebugDirectionalArrow(GetWorld(), WheelLocation, WheelLocation + (Force * 0.001f), 100.0f, FColor::Blue, false, 0.1f, 0, 4.0f);
    }
    else
    {
        //Debug: If no ground hit, draw a failed line in red
        DrawDebugLine(GetWorld(), WheelLocation, RayEnd, FColor::Red, false, 0.1f, 0, 2);
    }
}

void ARaycastCarPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
}
