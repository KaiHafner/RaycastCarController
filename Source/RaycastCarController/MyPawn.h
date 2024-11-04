// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

UCLASS()
class RAYCASTCARCONTROLLER_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	AMyPawn();

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
	float SuspensionRest = 100;
	UPROPERTY(EditAnywhere, Category = "Suspension")
	float SuspensionOffset = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Suspension")
	float SpringStrength = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Suspension")
	float SpringDamper = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Suspension")
	float SuspensionForce;

	// Helper function for suspension
	void ApplySuspensionForce(FVector Start, FVector End, float DeltaTime);
};
