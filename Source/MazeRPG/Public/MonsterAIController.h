// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class MAZERPG_API AMonsterAIController : public AAIController
{
	GENERATED_BODY()
	
public:
    AMonsterAIController();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // 플레이어 감지
    UFUNCTION()
    void DetectPlayer();

    // 감지 범위
    UPROPERTY(EditAnywhere, Category = "AI")
    float DetectRange = 600.f;

private:
    // 타겟 플레이어
    UPROPERTY()
    class ACharacter* TargetPlayer;

    // 감지 타이머
    FTimerHandle DetectTimerHandle;

};
