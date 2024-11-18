// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AxeItemSubsystem.generated.h"

struct FAxeItemTableRowDefine;
struct FItemTableRowFragmentArray;
class UItemInstance;
/**
 * 
 */
UCLASS()
class AXE_API UAxeItemSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	//
	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = Outer), Category = "Item")
	UItemInstance* CreateItemInstance(const FItemTableRowFragmentArray& ItemInfo, UObject* Outer);

	UFUNCTION(BlueprintCallable, Category = "Item")
	UDataTable* GetItemDefDataTable() const;

	UFUNCTION(BlueprintCallable, Category = "Item")
	void FindItemDefRowDefine(FName RowName, FAxeItemTableRowDefine& OutItemDef);
};
