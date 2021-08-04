// Fill out your copyright notice in the Description page of Project Settings.


#include "Airplane.h"
#include "GameFramework/GameModeBase.h"

// Sets default values
AAirplane::AAirplane()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAirplane::BeginPlay()
{
	Super::BeginPlay();

	OnActorHit.AddDynamic(this, &AAirplane::OnCollision);

	startRotation = GetActorRotation();
	velocity = spawnVelocity;

	//UE_LOG(LogTemp, Display, TEXT("I just started running"));
}

// Called to bind functionality to input
void AAirplane::SetupPlayerInputComponent(UInputComponent* playerInputComponent)
{
	Super::SetupPlayerInputComponent(playerInputComponent);

	playerInputComponent->BindAxis("Vertical", this, &AAirplane::Vertical);
	playerInputComponent->BindAxis("Horizontal", this, &AAirplane::Horizontal);
	playerInputComponent->BindAxis("Throttle", this, &AAirplane::Throttle);
}

// Called every frame
void AAirplane::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (!gameOver)
	{
		// Player Input
		// No Yaw to prevent turning back, therefore Pitch also feels a bit weird because it needs to be independent of Roll
		pitch += vertical * sensitivity * deltaTime;
		pitch = FMath::Clamp(pitch, -90.0f, 90.0f);
		roll += horizontal * sensitivity * deltaTime;
		float adjustedThrottleInput = (throttleInput + 1.0f) / 2.0f;
		if (adjustedThrottleInput != 0.5f)
		{
			if (adjustedThrottleInput > throttle)
			{
				throttle += FMath::Min(throttleSensitivity * deltaTime, adjustedThrottleInput - throttle);
			}
			else
			{
				throttle -= FMath::Min(throttleSensitivity * deltaTime, throttle - adjustedThrottleInput);
			}
		}

		// Calculate Rotation
		FRotator rotation{ pitch, 0.0f, roll };
		rotation = startRotation + rotation;

		// Get Position
		FVector position = GetActorLocation();

		// Calculate Air Pressure
		float pressure = (float)FMath::Pow(E, -position.Z * airPressureFactor);

		// Throttle
		if (fuel > 0.0f)
		{
			// Possible Extension: Take Airplane Weight into Account to provide more Thrust when Tanks are almost empty
			velocity += rotation.RotateVector(FVector{ thrust * throttle * deltaTime, 0.0f, 0.0f });
			fuel -= throttle * fuelConsumption * deltaTime;
		}

		// Apply Gravity
		// Custom Gravity because Default Gravity would not be included in velocity-Variable for AoA-Calculation
		// 1 Unit == 1cm => 9.81m/s^2 == 981.0cm/s^2
		velocity += FVector{ 0.0f, 0.0f, -981.0f * deltaTime };

		// Get Speed in m/s
		float speed = velocity.SizeSquared() / (100.0f * 100.0f);

		// Apply Lift
		if (velocity.X > 0.01f)
		{
			FVector localVelocity = rotation.UnrotateVector(velocity);
			float aoa = -FMath::Atan(localVelocity.Z / localVelocity.X);
			// Update AoA-Display before calculating Stall
			aoaDisplay = FMath::RadiansToDegrees(aoa);

			// Stalls only depend on AoA: https://aviation.stackexchange.com/questions/2903/how-does-stall-depend-on-angle-of-attack-but-not-speed
			if (aoa < -0.2618f || aoa > 0.2618f)		// Stallangle == 15° == 0.2618 radians
			{
				aoa = 0.0f;
				stall = true;
			}
			else
			{
				// I believe Stalling would not actually push the Nose up or down, but lets do it cz it feels better
				pitch -= aoa * aoaAttitudeFactor * deltaTime;
				stall = false;
			}

			// Formula based on https://en.wikipedia.org/wiki/Lift_(force)#Lift_coefficient
			// If Yaw was allowed, a Yaw-AoA would probably need to be considered (the Portion of Airstream that hits the Wings from the Sides should not contribute to Lift)
			FVector lift = rotation.RotateVector(FVector{ 0.0f, 0.0f, 1.0f })
				* (liftFactor
				* pressure
				* speed
				* aoa
				* deltaTime);
			velocity += lift;
		}

		// Apply Transformations
		position += velocity * deltaTime;
		SetActorLocationAndRotation(position, rotation);

		// Apply Drag
		// Drag Formula is mostly made up, but takes the Square of the Speed into Account
		velocity -= velocity * ((((drag * deltaTime) * pressure) * speed));	// Factors sorted by expected Magnitude

		// Update GUI
		distanceDisplay = position.Size2D() / 100.0f;
		altitudeDisplay = position.Z / 100.0f;
		airspeedDisplay = velocity.Size() / 100.0f;
		pressureDisplay = pressure;
		fuelDisplay = FMath::Max(fuel, 0.0f);
		throttleDisplay = throttle * 100.0f;
	}
}

UFUNCTION()
void AAirplane::OnCollision(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	gameOver = true;
}

void AAirplane::Vertical(float axisValue)
{
	vertical = axisValue;
}

void AAirplane::Horizontal(float axisValue)
{
	horizontal = axisValue;
}

void AAirplane::Throttle(float axisValue)
{
	throttleInput = axisValue;
}
