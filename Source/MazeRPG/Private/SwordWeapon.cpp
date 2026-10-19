#include "SwordWeapon.h"
#include "Components/BoxComponent.h"

ASwordWeapon::ASwordWeapon()
{
    // 히트박스 생성
    HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
    //HitBox->SetupAttachment(WeaponMesh);
    //HitBox->SetBoxExtent(FVector(50.f, 10.f, 50.f));
    //HitBox->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
    HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    WeaponType = EWeaponType::Sword;
    Damage = 15.f;

    // SM_Sword 메시 연결
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SwordMesh(
        TEXT("/Game/FreeMeleeWeaps/Static_Meshes/SM_Sword"));
    if (SwordMesh.Succeeded())
    {
        WeaponMesh->SetStaticMesh(SwordMesh.Object);
    }
}

void ASwordWeapon::OnHitBoxOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

    // 일단 뭐든 닿으면 로그!
    UE_LOG(LogTemp, Warning, TEXT("HitBox Overlap! Actor: %s"),
        *OtherActor->GetName());

    AMonsterBase* Monster =
        Cast<AMonsterBase>(OtherActor);
    if (Monster)
    {
        Monster->TakeDamage_Monster(Damage);
    }
}

void ASwordWeapon::Attack()
{
    // 콤보 카운터 증가
    ComboCount++;

    // 콤보 최대치 넘으면 리셋
    if (ComboCount > MaxCombo)
    {
        ComboCount = 1;
    }

    // 콤보 최대치 도달 시 크리티컬
    if (ComboCount == MaxCombo)
    {
        float CriticalDamage = Damage * 2.f;
        UE_LOG(LogTemp, Warning, TEXT("Critical! Demage: %f"), CriticalDamage);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Nomal Attack! Demage: %f, Combo: %d"), Damage, ComboCount);
    }

    // 히트박스 잠깐 켜기
    HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ASwordWeapon::BeginPlay()
{
    Super::BeginPlay();

    if (HitBox && WeaponMesh)
    {
        HitBox->AttachToComponent(
            WeaponMesh,
            FAttachmentTransformRules::SnapToTargetNotIncludingScale);
        HitBox->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
        HitBox->SetBoxExtent(FVector(200.f, 200.f, 200.f));
        HitBox->SetCollisionProfileName(TEXT("OverlapAll"));
        HitBox->SetGenerateOverlapEvents(true);

        // 중복 방지 후 등록!
        HitBox->OnComponentBeginOverlap.Clear();
        HitBox->OnComponentBeginOverlap.AddDynamic(
            this, &ASwordWeapon::OnHitBoxOverlap);

        HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

        UE_LOG(LogTemp, Warning, TEXT("SwordWeapon BeginPlay!"));
    }

}
