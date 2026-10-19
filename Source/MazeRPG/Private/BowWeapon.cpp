#include "BowWeapon.h"
#include "UObject/ConstructorHelpers.h"

ABowWeapon::ABowWeapon()
{
    WeaponType = EWeaponType::Bow;
    Damage = 20.f;

    // Static Mesh 숨기기
    WeaponMesh->SetVisibility(false);

    // Skeletal Mesh 보이게
    WeaponSkeletalMesh->SetVisibility(true);

    // 에셋 경로로 SK_Bow 불러오기
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> BowMesh(
        TEXT("/Game/FreeMeleeWeaps/Skeletal_Mesh/SK_Bow"));
    if (BowMesh.Succeeded())
    {
        WeaponSkeletalMesh->SetSkeletalMesh(BowMesh.Object);
    }
}

void ABowWeapon::Attack()
{
    UE_LOG(LogTemp, Warning, TEXT("Bow Attack!"));
    if (ProjectileClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("Projectile Spawned!"));
    }
}