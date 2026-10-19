// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MonsterHPWidget.generated.h"
class AMonsterBase;
/**
 * 
 */
UCLASS()
class MAZERPG_API UMonsterHPWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Monster")
	AMonsterBase* OwnerMonster;

};
