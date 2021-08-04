// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Airplane.generated.h"

UCLASS()
class FLYPLUSPLUS_API AAirplane : public APawn
{
	GENERATED_BODY()
private:
	const float E = 2.71828;
	UPROPERTY(EditAnywhere)
		float sensitivity = 30.0f;
	UPROPERTY(EditAnywhere)
		float throttleSensitivity = 0.5f;
	UPROPERTY(EditAnywhere)
		float fuel = 20.0f;
	UPROPERTY(EditAnywhere)
		float fuelConsumption = 2.0f;
	UPROPERTY(EditAnywhere)
		double drag = 0.000001;
	UPROPERTY(EditAnywhere)
		float thrust = 400.0f;
	UPROPERTY(EditAnywhere)
		float aoaAttitudeFactor = 50.0f;
	UPROPERTY(EditAnywhere)
		float liftFactor = 2.0f;
	UPROPERTY(EditAnywhere)
		float airPressureFactor = 0.00002f;
	UPROPERTY(EditAnywhere)
		FVector spawnVelocity {
		10000.0f, 10000.0f, 0.0f
	};

	FRotator startRotation;
	float vertical = 0.0f;
	float horizontal = 0.0f;
	float throttleInput = 0.0f;
	float pitch = 0.0f;
	float roll = 0.0f;
	float throttle = 1.0f;
	FVector velocity = FVector::ZeroVector;

public:
	UPROPERTY(BlueprintReadOnly)
		float distanceDisplay = 0.0f;
	UPROPERTY(BlueprintReadOnly)
		float altitudeDisplay = 0.0f;
	UPROPERTY(BlueprintReadOnly)
		float airspeedDisplay = 0.0f;
	UPROPERTY(BlueprintReadOnly)
		float aoaDisplay = 0.0f;
	UPROPERTY(BlueprintReadOnly)
		float pressureDisplay = 0.0f;
	UPROPERTY(BlueprintReadOnly)
		float fuelDisplay = 0.0f;
	UPROPERTY(BlueprintReadOnly)
		float throttleDisplay = 0.0f;
	UPROPERTY(BlueprintReadOnly)
		bool stall = false;
	UPROPERTY(BlueprintReadOnly)
		bool gameOver = false;

	// Sets default values for this pawn's properties
	AAirplane();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* playerInputComponent) override;

	// Called every frame
	virtual void Tick(float deltaTime) override;

	UFUNCTION()
		void OnCollision(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	void Vertical(float axisValue);
	void Horizontal(float axisValue);
	void Throttle(float axisValue);
};
