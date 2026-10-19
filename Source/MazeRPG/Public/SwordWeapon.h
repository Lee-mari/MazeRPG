#pragma once
#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "MonsterBase.h"
#include "SwordWeapon.generated.h"

UCLASS()
class MAZERPG_API ASwordWeapon : public AWeaponBase
{
    GENERATED_BODY()

public:
    ASwordWeapon();

    // 검 공격 범위
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    class UBoxComponent* HitBox;

    // 오버랩 함수
    UFUNCTION()
    void OnHitBoxOverlap(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);

    // 공격 오버라이드
    virtual void Attack() override;

    // 콤보 카운터
    UPROPERTY(BlueprintReadOnly, Category = "Weapon")
    int32 ComboCount = 0;

    // 콤보 최대 횟수
    UPROPERTY(EditAnywhere, Category = "Weapon")
    int32 MaxCombo = 3;

protected:
    virtual void BeginPlay() override;
};