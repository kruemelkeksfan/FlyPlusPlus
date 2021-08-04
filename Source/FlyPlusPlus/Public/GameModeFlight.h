// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Airplane.h"
#include "GameModeFlight.generated.h"

/**
 * 
 */
UCLASS()
class FLYPLUSPLUS_API AGameModeFlight : public AGameModeBase
{
	GENERATED_BODY()
public:
	AGameModeFlight()
	{
		DefaultPawnClass = AAirplane::StaticClass();
	}
};
