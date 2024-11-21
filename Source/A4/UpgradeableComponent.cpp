// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeableComponent.h"
#include "FarmingStatsComponent.h"

// Sets default values for this component's properties
UUpgradeableComponent::UUpgradeableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	initialCost = 100;
	costIncrement = 100;
	currentLevel = 1;
	currentCost = initialCost;
	maxLevel = 5;

}

//Return 0 if complete
//Return 1 if not enough money
//Return 2 if already at max level
int UUpgradeableComponent::Upgrade() 
{
	UFarmingStatsComponent* statComp = GetWorld()->GetFirstPlayerController()->GetPawn()->GetComponentByClass<UFarmingStatsComponent>();
	
	if (currentLevel == maxLevel)
	{
		return 2;
	}

	if (statComp->money < currentCost)
	{
		return 1;
	}
	
	statComp->ChangeMoney(currentCost);
	currentLevel++;
	currentCost += costIncrement;

	//Event Dispatcher
	OnLevelUpDelegate.Broadcast();

	return 0;
}


