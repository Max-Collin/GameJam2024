// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GGGGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAM2024_API UGGGGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	 int32 Difficulty= 1;
};
