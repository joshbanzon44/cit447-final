// Fill out your copyright notice in the Description page of Project Settings.

#include "VertexPaintableComponent.h"
#include "StaticMeshResources.h"
#include "StaticMeshComponentLODInfo.h"

// Sets default values for this component's properties
UVertexPaintableComponent::UVertexPaintableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UVertexPaintableComponent::BeginPlay()
{
    Super::BeginPlay();

    StaticMeshComponent = GetOwner()->FindComponentByClass<UStaticMeshComponent>();

    if (StaticMeshComponent != nullptr && IsValid(StaticMeshComponent))
    {
        InitialiseLODInfoAndBuffers();
    }

    FStaticMeshComponentLODInfo* InstanceMeshLODInfo = &StaticMeshComponent->LODData[0];
    FStaticMeshLODResources& LODModel = StaticMeshComponent->GetStaticMesh()->GetRenderData()->LODResources[0];

    //Array of all dirty vertices to calculate percent dirty
    TArray<FColor> DirtyVertices;

    for (auto i = 0; i < LODModel.GetNumVertices(); i++)
    {
        if (InstanceMeshLODInfo->OverrideVertexColors->VertexColor(i) != TargetBaseColor)
        {
            //Add to TArray if vertex color is not target
            DirtyVertices.Add(InstanceMeshLODInfo->OverrideVertexColors->VertexColor(i));
        }
    }

    //Calculate percent clean by num of clean vertices/total vertices
    PercentClean = (static_cast<float>(LODModel.GetNumVertices()) - DirtyVertices.Num()) / LODModel.GetNumVertices();
}

void UVertexPaintableComponent::PaintVertexAtLocation(FVector HitLocation, float PaintLerpProgress)
{
    if (broadcasted)
    {
        return;
    }
    
    // Init the buffers and LOD data
    InitialiseLODInfoAndBuffers();

    FStaticMeshComponentLODInfo* InstanceMeshLODInfo = &StaticMeshComponent->LODData[0];
    FStaticMeshLODResources& LODModel = StaticMeshComponent->GetStaticMesh()->GetRenderData()->LODResources[0];

    float BrushSize = 150.0f;
    auto LocalToWorld = StaticMeshComponent->GetComponentToWorld().ToMatrixWithScale();

    //Array of all dirty vertices to calculate percent dirty
    TArray<FColor> DirtyVertices;

    for (auto i = 0; i < LODModel.GetNumVertices(); i++)
    {
        if (InstanceMeshLODInfo->OverrideVertexColors->VertexColor(i) != TargetBaseColor)
        {
            //Add to TArray if vertex color is not target
            DirtyVertices.Add(InstanceMeshLODInfo->OverrideVertexColors->VertexColor(i));
        }
    }

    //Calculate percent clean by num of clean vertices/total vertices
    PercentClean = (static_cast<float>(LODModel.GetNumVertices()) - DirtyVertices.Num()) / LODModel.GetNumVertices();

    if (PercentClean >= 1.0f)
    {
        OnFullyCleanedDelegate.Broadcast();
        broadcasted = true;
        return;
    }

    for (auto i = 0; i < LODModel.GetNumVertices(); i++)
    {
        auto LocalVertexPosition = LODModel.VertexBuffers.PositionVertexBuffer.VertexPosition(i);
        auto WorldVertexPosition = LocalToWorld.TransformPosition(UE::Math::TVector4<double>(LocalVertexPosition.X, LocalVertexPosition.Y, LocalVertexPosition.Z, 1.0f));
        auto Distance = FVector::DistSquared(WorldVertexPosition, HitLocation);

        if (Distance <= BrushSize)
        {
            FLinearColor from = FLinearColor(InstanceMeshLODInfo->OverrideVertexColors->VertexColor(i));
            FLinearColor to = FLinearColor(TargetBaseColor);

            FColor& currentColor = InstanceMeshLODInfo->OverrideVertexColors->VertexColor(i);
            currentColor = FLinearColor::LerpUsingHSV(from, to, PaintLerpProgress).ToFColor(false);
        }
    }

    // Notify the render thread about the buffer change
    BeginUpdateResourceRHI(InstanceMeshLODInfo->OverrideVertexColors);
    StaticMeshComponent->MarkRenderStateDirty();

#if WITH_EDITORONLY_DATA
    StaticMeshComponent->CachePaintedDataIfNecessary();
#endif
}

int32 UVertexPaintableComponent::GetNearestVertIndex(FVector Position, FStaticMeshLODResources& LODModel)
{
    auto ShortestDistance = 0;
    auto NearestVertexIndex = -1;
    auto LocalToWorld = StaticMeshComponent->GetComponentToWorld().ToMatrixWithScale();

    for (auto i = 0; i < LODModel.GetNumVertices(); i++)
    {
        auto LocalVertexPosition = LODModel.VertexBuffers.PositionVertexBuffer.VertexPosition(i);
        auto WorldVertexPosition = LocalToWorld.TransformPosition(UE::Math::TVector4<double>(LocalVertexPosition.X, LocalVertexPosition.Y, LocalVertexPosition.Z, 1.0f));
        auto Distance = FVector::DistSquared(WorldVertexPosition, Position);

        if (Distance < ShortestDistance || ShortestDistance < 0)
        {
            ShortestDistance = Distance;
            NearestVertexIndex = i;
        }
    }

    return NearestVertexIndex;
}

void UVertexPaintableComponent::InitialiseLODInfoAndBuffers()
{
    if (StaticMeshComponent->LODData.Num() == 0)
    {
        StaticMeshComponent->SetLODDataCount(1, StaticMeshComponent->LODData.Num());
    }

    FStaticMeshLODResources& LODModel = StaticMeshComponent->GetStaticMesh()->GetRenderData()->LODResources[0];
    FStaticMeshComponentLODInfo* InstanceMeshLODInfo = &StaticMeshComponent->LODData[0];

    if (InstanceMeshLODInfo->OverrideVertexColors == nullptr)
    {
        InitialiseInstancedOverrideVertexColorBuffer(InstanceMeshLODInfo, LODModel);
    }
}

void UVertexPaintableComponent::InitialiseInstancedOverrideVertexColorBuffer(FStaticMeshComponentLODInfo* InstanceMeshLODInfo, FStaticMeshLODResources& LODModel)
{
    // Check that we don't already have an overridden vertex color buffer
    check(InstanceMeshLODInfo->OverrideVertexColors == nullptr);

    // Create a new buffer
    InstanceMeshLODInfo->OverrideVertexColors = new FColorVertexBuffer;

    if ((int32)LODModel.VertexBuffers.ColorVertexBuffer.GetNumVertices() >= LODModel.GetNumVertices())
    {
        // If the mesh already has vertex colours, initialise OverrideVertexColors from them
        InstanceMeshLODInfo->OverrideVertexColors->InitFromColorArray(&LODModel.VertexBuffers.ColorVertexBuffer.VertexColor(0), LODModel.GetNumVertices());
    }
    else
    {
        // If it doesn't, set all overridden vert colours to black
        InstanceMeshLODInfo->OverrideVertexColors->InitFromSingleColor(TargetBaseColor, LODModel.GetNumVertices());
    }

  

    BeginInitResource(InstanceMeshLODInfo->OverrideVertexColors);
}
