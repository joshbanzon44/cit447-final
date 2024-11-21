// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FarmingStatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOutOfMoney);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNotEnoughGrain);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnChangeStat);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class A4_API UFarmingStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFarmingStatsComponent();

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int money;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int grain;

	UFUNCTION(BlueprintCallable)
	void ChangeMoney(int moneyAmt = 0);

	UFUNCTION(BlueprintCallable)
	void ChangeGrain(int grainAmt = 0);

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnOutOfMoney OnOutOfMoneyDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnNotEnoughGrain OnNotEnoughGrainDelegate;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnChangeStat OnChangeStatDelegate;
		
};
