// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VertexPaintableComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFullyCleaned);


/**
Channel used for spreading vertex colors.
*/
UENUM()
namespace AVertexColorSpreadChannel
{
	enum Channel
	{
		Red,
		Green,
		Blue,
		Alpha,
	};
}


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class A4_API UVertexPaintableComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UVertexPaintableComponent();

	UFUNCTION(BlueprintCallable, Category = VertexPainting)
	void PaintVertexAtLocation(FVector HitLocation, float PaintLerpProgress = 1.0f);

	UPROPERTY(BLueprintCallable, BlueprintAssignable)
	FOnFullyCleaned OnFullyCleanedDelegate;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = VertexPainting)
	FColor TargetBaseColor = FColor::White;

	UPROPERTY(BlueprintReadOnly, Category = VertexPainting)
	float PercentClean = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = VertexPainting)
	bool broadcasted = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Create LOD info entries, and ensure the override vertex color buffer exists.
	void InitialiseLODInfoAndBuffers();

	//Create a new override vertex color buffer.
	void InitialiseInstancedOverrideVertexColorBuffer(FStaticMeshComponentLODInfo* InstanceMeshLODInfo,
		FStaticMeshLODResources& LODModel);

	//Get the intensity of the selected channel (see Channel)
	int32 GetNearestVertIndex(FVector Position, FStaticMeshLODResources& LODModel);

	//Get the intensity of the selected channel (see Channel)
	//uint8 GetIntensity(FColor Color);

	// Set the intensity of the selected channel to the given value (see Channel)
	//void SetIntensity(FColor* Color, uint8 Intensity);

	UPROPERTY(BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComponent;
};