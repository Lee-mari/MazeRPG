#pragma once
#include "CoreMinimal.h"

#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
    Sword,   // 검
    Bow,     // 활
    Magic    // 마법
};

UCLASS()
class MAZERPG_API AWeaponBase : public AActor
{
    GENERATED_BODY()

public:
    AWeaponBase();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // 무기 종류
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
    EWeaponType WeaponType;

    // 무기 데미지
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
    float Damage = 10.f;

    // 무기 메시 (static)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    UStaticMeshComponent* WeaponMesh;

    // 활용 Skeletal 메시 추가
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
    USkeletalMeshComponent* WeaponSkeletalMesh;

    // 공격 함수 (자식 클래스에서 각각 구현)
    UFUNCTION(BlueprintCallable, Category = "Weapon")
    virtual void Attack();
};