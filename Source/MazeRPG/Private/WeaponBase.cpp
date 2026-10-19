#include "WeaponBase.h"

AWeaponBase::AWeaponBase()
{
    PrimaryActorTick.bCanEverTick = true;

    // Static Mesh (칼, 창 등)
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
    RootComponent = WeaponMesh;

    // Skeletal Mesh (활)
    WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeletalMesh"));
    WeaponSkeletalMesh->SetupAttachment(RootComponent);
    WeaponSkeletalMesh->SetVisibility(false); // 기본은 숨김
}

void AWeaponBase::BeginPlay()
{
    Super::BeginPlay();
}

void AWeaponBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AWeaponBase::Attack()
{
    // 자식 클래스에서 구현
}