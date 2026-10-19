#include "MonsterAIController.h"
#include "MonsterBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/PathFollowingComponent.h"

AMonsterAIController::AMonsterAIController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AMonsterAIController::BeginPlay()
{
    Super::BeginPlay();

    // 0.5초마다 플레이어 감지
    GetWorldTimerManager().SetTimer(
        DetectTimerHandle,
        this,
        &AMonsterAIController::DetectPlayer,
        0.5f,
        true
    );
}

void AMonsterAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMonsterAIController::DetectPlayer()
{
    AMonsterBase* Monster = Cast<AMonsterBase>(GetPawn());
    if (!Monster) return;

    // 플레이어 찾기
    ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (!Player) return;

    // 거리 계산
    float Distance = FVector::Dist(Monster->GetActorLocation(), Player->GetActorLocation());

    if (Distance <= DetectRange)
    {
        // 플레이어 발견 → 추적
        TargetPlayer = Player;
        Monster->SetMonsterState(EMonsterState::Chase);
        MoveToActor(Player, 150.f);
        UE_LOG(LogTemp, Warning, TEXT("Monster Detected Player! Distance: %f"), Distance);
    }
    else
    {
        // 플레이어 못 찾음 → 순찰
        TargetPlayer = nullptr;
        Monster->SetMonsterState(EMonsterState::Patrol);
    }
}