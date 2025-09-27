// Fill out your copyright notice in the Description page of Project Settings.


#include "ISMManagerComp.h"
#include "Components/InstancedStaticMeshComponent.h"


// Sets default values for this component's properties
UISMManagerComp::UISMManagerComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UISMManagerComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

FISMHandle UISMManagerComp::AddNewMesh(TSoftObjectPtr<UStaticMesh> InStaticMesh, FTransform InTransform, bool IsWorldSpace)
{
	FPrimitiveInstanceId index;
	if (auto findPair = MeshToComps.Find(InStaticMesh))
	{
		index = (*findPair)->AddInstanceById(InTransform, IsWorldSpace);
	}
	else
	{
		auto comp = Cast<UInstancedStaticMeshComponent>(GetOwner()->AddComponentByClass(ISMCompClass, false, FTransform(), false));
		comp->SetStaticMesh(Cast<UStaticMesh>(InStaticMesh.ToSoftObjectPath().TryLoad()));
		OnISMCompInit.Broadcast(comp);
		comp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		index = comp->AddInstanceById(InTransform, IsWorldSpace);
		MeshToComps.Emplace(InStaticMesh, comp);
	}
	return FISMHandle(InStaticMesh, index);
}

void UISMManagerComp::RemoveByHandle(const FISMHandle& RemoveHandle)
{
	if (auto findPair = MeshToComps.Find(RemoveHandle.StaticMesh))
	{
		(*findPair)->RemoveInstanceById(RemoveHandle.InstanceId);
	}
}
