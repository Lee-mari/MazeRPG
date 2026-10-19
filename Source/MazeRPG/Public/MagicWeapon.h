#pragma once
#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "MagicWeapon.generated.h"

UCLASS()
class MAZERPG_API AMagicWeapon : public AWeaponBase
{
    GENERATED_BODY()

public:
    AMagicWeapon();

    // 마법 범위
    UPROPERTY(EditAnywhere, Category = "Weapon")
    float MagicRadius = 200.f;

    // 마법 타입
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
    FString MagicType = TEXT("Fire");

    // 공격 오버라이드
    virtual void Attack() override;

protected:
    virtual void BeginPlay() override;
};