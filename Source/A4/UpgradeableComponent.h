// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UpgradeableComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelUp);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class A4_API UUpgradeableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUpgradeableComponent();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int initialCost;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int currentCost;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int costIncrement;

	UPROPERTY(BlueprintReadOnly)
	int currentLevel;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int maxLevel;

	UFUNCTION(BlueprintCallable)
	int Upgrade();

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnLevelUp OnLevelUpDelegate;

};
