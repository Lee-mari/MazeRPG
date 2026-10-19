// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MonsterAIController.h"
#include "MonsterBase.generated.h"
class UWidgetComponent;
UENUM(BlueprintType)
enum class EMonsterState : uint8
{
	Patrol,   // 순찰
	Chase,    // 추적
	Attack,   // 공격
	Dead      // 사망
};


UCLASS()
class MAZERPG_API AMonsterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMonsterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    // 체력
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
    float MaxHP = 100.f;

    UPROPERTY(BlueprintReadOnly, Category = "Monster")
    float CurrentHP;

    // 공격력
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
    float AttackDamage = 10.f;

    // 감지 범위
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
    float DetectRange = 600.f;

    // 공격 범위
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster")
    float AttackRange = 150.f;

    // 현재 상태
    UPROPERTY(BlueprintReadOnly, Category = "Monster")
    EMonsterState MonsterState = EMonsterState::Patrol;

    // HP 위젯
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
    UWidgetComponent* HPWidgetComponent;

    // 데미지 받기
    UFUNCTION(BlueprintCallable, Category = "Monster")
    virtual void TakeDamage_Monster(float Damage);

    // 사망 처리
    UFUNCTION(BlueprintCallable, Category = "Monster")
    virtual void Die();

    // 스킬 사용 (자식 클래스에서 구현)
    UFUNCTION(BlueprintCallable, Category = "Monster")
    virtual void UseSkill();

    // 상태 변경
    UFUNCTION(BlueprintCallable, Category = "Monster")
    void SetMonsterState(EMonsterState NewState);


	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
