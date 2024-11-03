// Fill out your copyright notice in the Description page of Project Settings.


#include "RaycastCarPawn.h"

// Sets default values
ARaycastCarPawn::ARaycastCarPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARaycastCarPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARaycastCarPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARaycastCarPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

