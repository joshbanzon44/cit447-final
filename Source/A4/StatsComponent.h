// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class A4_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int level;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float currentXP;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	int nextLevelXP;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float cleaningRange;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float cooldownSeconds;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float firepower;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float firepowerRegenPerSecond;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float firepowerPerShot;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	float maxFirepower;

	UFUNCTION(BlueprintCallable)
	void AddXP(float xpAmount = 0);

protected:

	//Local function to change stats when leveling up
	void UpdateStats(float cleaningRangeChange = 0.0f, float firepowerPerSecondChange = 0.0f, float maxFirepowerChange = 0.0f);

};
