// Fill out your copyright notice in the Description page of Project Settings.


#include "FarmingStatsComponent.h"

// Sets default values for this component's properties
UFarmingStatsComponent::UFarmingStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	money = 100;
	grain = 0;
}

void UFarmingStatsComponent::ChangeMoney(int moneyAmt)
{
	money += moneyAmt;
	if (money < 0)
	{
		OnOutOfMoneyDelegate.Broadcast();
	}
	OnChangeStatDelegate.Broadcast();
}

void UFarmingStatsComponent::ChangeGrain(int grainAmt)
{
	if (grain+grainAmt < 0)
	{
		OnNotEnoughGrainDelegate.Broadcast();
	}
	else
	{
		grain += grainAmt;
	}

	OnChangeStatDelegate.Broadcast();
}






