#pragma once
#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "BowWeapon.generated.h"

UCLASS()
class MAZERPG_API ABowWeapon : public AWeaponBase
{
    GENERATED_BODY()

public:
    ABowWeapon();

    // 투사체 클래스 (나중에 투사체 만들면 연결)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
    TSubclassOf<AActor> ProjectileClass;

    // 발사 속도
    UPROPERTY(EditAnywhere, Category = "Weapon")
    float ProjectileSpeed = 3000.f;

    // 공격 오버라이드
    virtual void Attack() override;
};