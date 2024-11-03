#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RaycastCarPawn.generated.h"

UCLASS()
class RAYCASTCARCONTROLLER_API ARaycastCarPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ARaycastCarPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CarBody;

	UPROPERTY(EditAnywhere, Category = "Suspension")
	float SuspensionMaxLength = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Suspension")
	float SuspensionStrength = 5000.0f;

	UPROPERTY(EditAnywhere, Category = "Suspension")
	float SuspensionDamping = 500.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float ForwardForce = 10000.0f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float TurnRate = 50.0f;

	// Wheel positions
	TArray<FVector> WheelOffsets;

	// Input functions
	void MoveForward(float Value);
	void Turn(float Value);

	// Current input values
	float CurrentForwardInput;
	float CurrentTurnInput;

	// Helper function for suspension
	void ApplySuspensionForce(FVector Start, FVector End, float DeltaTime);
};
