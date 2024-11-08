#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RaycastCarPawn.generated.h"

UCLASS()
class RAYCASTCARCONTROLLER_API ARaycastCarPawn : public APawn
{
    GENERATED_BODY()

public:
    ARaycastCarPawn();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CarBody;

	// Declare USceneComponent pointers for existing wheel locations
	UPROPERTY(VisibleAnywhere, Category = "Locations")
	USceneComponent* WheelFL; // Front Left Wheel
	UPROPERTY(VisibleAnywhere, Category = "Locations")
	USceneComponent* WheelFR; // Front Right Wheel
	UPROPERTY(VisibleAnywhere, Category = "Locations")
	USceneComponent* WheelRL; // Rear Left Wheel
	UPROPERTY(VisibleAnywhere, Category = "Locations")
	USceneComponent* WheelRR; // Rear Right Wheel

	UPROPERTY(EditAnywhere, Category = "Suspension")
	float SuspensionRest = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Suspension")
	float SpringStrength = 30000.0f;
	UPROPERTY(EditAnywhere, Category = "Suspension")
	float SpringDamper = 3000.0f;
	UPROPERTY(EditAnywhere, Category = "Suspension")
	float SuspensionForce;
	UPROPERTY(EditAnywhere, Category = "Suspension")
	float SuspensionOffset = 0.0f; //Wheel Offset (Wheel size)

	// Helper function for suspension
	void ApplySuspensionForce(FVector Start, FVector End, float DeltaTime);
};
