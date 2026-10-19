#include "MagicWeapon.h"
#include "UObject/ConstructorHelpers.h"

AMagicWeapon::AMagicWeapon()
{
    WeaponType = EWeaponType::Magic;
    Damage = 25.f;

    // SM_Spear 메시 연결 (지팡이 대용)
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SpearMesh(
        TEXT("/Game/FreeMeleeWeaps/Static_Meshes/SM_Spear"));
    if (SpearMesh.Succeeded())
    {
        WeaponMesh->SetStaticMesh(SpearMesh.Object);
    }
}

void AMagicWeapon::Attack()
{
    UE_LOG(LogTemp, Warning, TEXT("Magic Attack! Type: %s, Damage: %f, Radius: %f"),
        *MagicType, Damage, MagicRadius);
}

void AMagicWeapon::BeginPlay()
{
    Super::BeginPlay();

    if (WeaponMesh)
    {
        WeaponMesh->SetRelativeRotation(
            FRotator(90.f, 0.f, 0.f));
    }
}
