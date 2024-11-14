// Fill out your copyright notice in the Description page of Project Settings.

#include "StatsComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UStatsComponent::UStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	level = 1;
	currentXP = 0.0f;
	nextLevelXP = 100;
	cleaningRange = 300.0f;
	cooldownSeconds = 2.0f;
	firepower = 100.0f;
	firepowerRegenPerSecond = 20.0f;
	firepowerPerShot = 1.0f;
	maxFirepower = 100.0f;
}

// Called when the game starts
void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}
// Called every frame
void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatsComponent::AddXP(float xpAmount)
{
	currentXP += xpAmount;

	if (currentXP >= 100.0f)
	{
		currentXP -= 100.0f;
		level += 1;
		UpdateStats(50.0f, 2.0f, 50.0f);
		OnLevelUpDelegate.Broadcast();
	}

	OnXPChangeDelegate.Broadcast();
}

void UStatsComponent::UpdateStats(float cleaningRangeChange, float firepowerPerSecondChange, float maxFirepowerChange)
{
	cleaningRange += cleaningRangeChange;
	firepowerRegenPerSecond += firepowerPerSecondChange;
	maxFirepower += maxFirepowerChange;

}



