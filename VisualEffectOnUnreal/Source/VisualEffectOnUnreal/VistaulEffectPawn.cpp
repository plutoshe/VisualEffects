// Fill out your copyright notice in the Description page of Project Settings.


#include "VistaulEffectPawn.h"

// Sets default values
AVistaulEffectPawn::AVistaulEffectPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVistaulEffectPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVistaulEffectPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVistaulEffectPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

