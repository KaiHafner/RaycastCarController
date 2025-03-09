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

	//Suspension
	UPROPERTY(EditAnywhere, Category = "Suspension")
	float SuspensionRest = 70.0f; //Rest length of suspension

	UPROPERTY(EditAnywhere, Category = "Suspension")
	float SuspensionStiffness = 50000.0f; //Higher = stiffer suspension

	UPROPERTY(EditAnywhere, Category = "Suspension")
	float SuspensionDamping = 1000.0f; //Higher = less bounce

	//Wheels
	UPROPERTY(VisibleAnywhere, Category = "Locations")
	USceneComponent* WheelFL; //Front Left Wheel
	UPROPERTY(VisibleAnywhere, Category = "Locations")
	USceneComponent* WheelFR; //Front Right Wheel
	UPROPERTY(VisibleAnywhere, Category = "Locations")
	USceneComponent* WheelRL; //Rear Left Wheel
	UPROPERTY(VisibleAnywhere, Category = "Locations")
	USceneComponent* WheelRR; //Rear Right Wheel

	UPROPERTY(VisibleAnywhere, Category = "Grounded")
	bool isGrounded;


	//Helper function for suspension
	void ApplySuspensionForce(FVector Start, float DeltaTime);
};
