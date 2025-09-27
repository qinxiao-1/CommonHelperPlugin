// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "ISMManagerComp.generated.h"

// 重排序有问题
USTRUCT(BlueprintType)
struct FISMHandle
{
	GENERATED_BODY()
	FISMHandle() = default;
	FISMHandle(const TSoftObjectPtr<UStaticMesh>& StaticMesh, const FPrimitiveInstanceId& InstanceId)
		: StaticMesh(StaticMesh),
		  InstanceId(InstanceId)
	{
	}

	UPROPERTY()
	TSoftObjectPtr<UStaticMesh> StaticMesh = nullptr;
	UPROPERTY()
	FPrimitiveInstanceId InstanceId = {};
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class INSTANCEDSTATICMESHHELPER_API UISMManagerComp : public UActorComponent
{
	GENERATED_BODY()

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnISMCompInit, UInstancedStaticMeshComponent*, ISMComp);
public:
	// Sets default values for this component's properties
	UISMManagerComp();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	FISMHandle AddNewMesh(TSoftObjectPtr<UStaticMesh> InStaticMesh, FTransform InTransform, bool IsWorldSpace = false);
	UFUNCTION(BlueprintCallable)
	void RemoveByHandle(const FISMHandle& RemoveHandle);

protected:
	// ---------------------------------- Data ---------------------------------- //
	UPROPERTY(BlueprintReadOnly)
	TMap<TSoftObjectPtr<UStaticMesh>, TObjectPtr<UInstancedStaticMeshComponent>> MeshToComps = {};

	UPROPERTY(BlueprintAssignable)
	FOnISMCompInit OnISMCompInit;

	// ---------------------------------- Config ---------------------------------- //
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Config)
	TSubclassOf<UInstancedStaticMeshComponent> ISMCompClass = UInstancedStaticMeshComponent::StaticClass();
};