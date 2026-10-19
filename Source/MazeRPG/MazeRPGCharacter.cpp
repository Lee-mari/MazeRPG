// Copyright Epic Games, Inc. All Rights Reserved.


#include "MazeRPGCharacter.h"
#include "WeaponBase.h"
#include "MagicWeapon.h"
#include "SwordWeapon.h"
#include "MonsterBase.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AMazeRPGCharacter

AMazeRPGCharacter::AMazeRPGCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AMazeRPGCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 디버그 로그
	UE_LOG(LogTemp, Warning, TEXT("RollAction: %s"), RollAction ? TEXT("Set") : TEXT("NULL"));


	// 활 숨기기
	TArray<USceneComponent*> Components;
	GetMesh()->GetChildrenComponents(true, Components);
	for (USceneComponent* Comp : Components)
	{
		if (Comp->GetName().Contains(TEXT("bow")) ||
			Comp->GetName().Contains(TEXT("Bow")))
		{
			Comp->SetVisibility(false, true);
		}
	}


	// Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMazeRPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMazeRPGCharacter::Move);
		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMazeRPGCharacter::Look);
		//Roll
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Started, this, &AMazeRPGCharacter::Roll);

		// 무기 교체 추가!
		EnhancedInputComponent->BindAction(Weapon1Action, ETriggerEvent::Started, this, &AMazeRPGCharacter::SwitchWeapon1);
		EnhancedInputComponent->BindAction(Weapon2Action, ETriggerEvent::Started, this, &AMazeRPGCharacter::SwitchWeapon2);
		EnhancedInputComponent->BindAction(Weapon3Action, ETriggerEvent::Started, this, &AMazeRPGCharacter::SwitchWeapon3);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AMazeRPGCharacter::PlayerAttack);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component!"), *GetNameSafe(this));
	}
}

void AMazeRPGCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMazeRPGCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMazeRPGCharacter::StartSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void AMazeRPGCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AMazeRPGCharacter::EquipWeapon(int32 Index)
{
	// 이전 무기 제거
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	// 새 무기 스폰
	if (WeaponClasses.IsValidIndex(Index) && WeaponClasses[Index])
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn; // 충돌 무시하고 스폰

		CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(
			WeaponClasses[Index],
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParams);

		if (CurrentWeapon)
		{
			// 충돌 끄기
			CurrentWeapon->SetActorEnableCollision(false);

			// 소켓 이름 수정!
			CurrentWeapon->AttachToComponent(
				GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				TEXT("Weapon_Root_R")); // ← 여기 수정!

				// Magic 소켓 크기, 위치 조정
				if (AMagicWeapon* Magic = Cast<AMagicWeapon>(CurrentWeapon))
				{
					Magic->WeaponMesh->SetRelativeRotation(FRotator(90.f, 80.f, 0.f));
					Magic->WeaponMesh->SetRelativeScale3D(FVector(1.f, 1.f, 0.8f));
				}

				// 검 위치 조정 
				if (ASwordWeapon* Sword = Cast<ASwordWeapon>(CurrentWeapon))
				{
					Sword->WeaponMesh->SetRelativeRotation(FRotator(53.f, 0.f, 0.f));
					/*Sword->WeaponMesh->SetRelativeLocation(FVector(30.f, 0.f, 0.f));*/
				}

			CurrentWeaponIndex = Index;
			UE_LOG(LogTemp, Warning, TEXT("Weapon Equipped: %d"), Index);
		}
	}
}

void AMazeRPGCharacter::SwitchWeapon1()
{
	EquipWeapon(0);
}

void AMazeRPGCharacter::SwitchWeapon2()
{
	EquipWeapon(1);
}

void AMazeRPGCharacter::SwitchWeapon3()
{
	EquipWeapon(2);
}

void AMazeRPGCharacter::Roll()
{
	UE_LOG(LogTemp, Warning, TEXT("Roll Called!")); // 추가!

	bIsRolling = true;

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		if (RollMontage)
		{
			AnimInstance->Montage_Play(RollMontage);
		}
	}

	FTimerHandle RollTimer;
	GetWorldTimerManager().SetTimer(RollTimer, [this]()
		{
			bIsRolling = false;
		}, 1.0f, false);
}

void AMazeRPGCharacter::PlayerAttack()
{
	if (CurrentWeaponIndex != 0) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance || !AttackMontage) return;

	// 공격 중에 Z 누르면 콤보!
	if (bIsAttacking && bCanCombo)
	{
		ComboCount++;
		if (ComboCount > 2) ComboCount = 0;
		bCanCombo = false;

		FName NextSection;
		switch (ComboCount)
		{
		case 1: NextSection = FName("Attack2"); break;
		case 2: NextSection = FName("Attack3"); break;
		default: NextSection = FName("Attack1"); break;
		}
		AnimInstance->Montage_JumpToSection(
			NextSection, AttackMontage);
		return;
	}

	if (bIsAttacking) return;

	// 첫 공격!
	bIsAttacking = true;
	bCanCombo = false;
	ComboCount = 0;
	AnimInstance->Montage_Play(AttackMontage);
	AnimInstance->Montage_JumpToSection(
		FName("Attack1"), AttackMontage);

	// 히트박스
	FHitResult HitResult;
	FVector Start = GetActorLocation();
	FVector End = Start + GetActorForwardVector() * 150.f;
	FCollisionShape Box = FCollisionShape::MakeBox(FVector(50.f));

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult, Start, End, FQuat::Identity,
		ECollisionChannel::ECC_Visibility, Box);

	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"),
			*HitResult.GetActor()->GetName());
		AMonsterBase* Monster = Cast<AMonsterBase>(HitResult.GetActor());
		if (Monster)
		{
			Monster->TakeDamage_Monster(10.f);
			UE_LOG(LogTemp, Warning, TEXT("Hit Monster!!!!"));
		}
	}
}
