// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBase.h"
#include "Components/WidgetComponent.h"
#include "MonsterHPWidget.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AMonsterBase::AMonsterBase()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	// AI Controller 연결
	AIControllerClass = AMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	HPWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWidgetComponent"));
	HPWidgetComponent->SetupAttachment(RootComponent);
	HPWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HPWidgetComponent->SetDrawSize(FVector2D(120.f, 20.f));
	HPWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 120.f));


}

// Called when the game starts or when spawned
void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	CurrentHP = MaxHP;

	if (HPWidgetComponent)
	{
		if (UMonsterHPWidget* HPWidget = Cast<UMonsterHPWidget>(HPWidgetComponent->GetUserWidgetObject()))
		{
			HPWidget->OwnerMonster = this;
		}
	}

}

// Called every frame
void AMonsterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMonsterBase::TakeDamage_Monster(float Damage)
{
	if (MonsterState == EMonsterState::Dead) return;

	CurrentHP -= Damage;
	CurrentHP = FMath::Clamp(CurrentHP, 0.f, MaxHP);

	UE_LOG(LogTemp, Warning, TEXT("Monster HP: %f"), CurrentHP);

	if (CurrentHP <= 0)
	{
		Die();
	}
}

void AMonsterBase::Die()
{
	SetMonsterState(EMonsterState::Dead);
	UE_LOG(LogTemp, Warning, TEXT("Monster Dead!"));

	// 3초 후 삭제
	SetLifeSpan(3.f);
}

void AMonsterBase::UseSkill()
{
	// 자식 클래스에서 구현
}

void AMonsterBase::SetMonsterState(EMonsterState NewState)
{
	MonsterState = NewState;
	UE_LOG(LogTemp, Warning, TEXT("Monster State Changed: %d"), (int32)NewState);
}

// Called to bind functionality to input
void AMonsterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

