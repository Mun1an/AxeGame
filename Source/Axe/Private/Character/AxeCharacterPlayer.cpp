// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/AxeCharacterPlayer.h"

#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "ActionSystem/ActionCombatComponent.h"
#include "ActionSystem/ComboActionComponent.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "PlayerController/AxePlayerController.h"
#include "PlayerState/AxePlayerState.h"
#include "UI/HUD/AxeHUD.h"

// AAxeCharacter

AAxeCharacterPlayer::AAxeCharacterPlayer()
{
	bReplicates = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(GetMesh(), WeaponTipSocketName);
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponSecondary = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponSecondary"));
	WeaponSecondary->SetupAttachment(GetMesh(), WeaponSecondaryTipSocketName);
	WeaponSecondary->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// ComboActionComponent
	ComboActionComponent = CreateDefaultSubobject<UComboActionComponent>(TEXT("ComboActionComponent"));
	//
	ActionCombatComponent = CreateDefaultSubobject<UActionCombatComponent>(TEXT("ActionCombatComponent"));

	//
}

void AAxeCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// server
	InitAbility();
}

void AAxeCharacterPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// client
	InitAbility();
}


void AAxeCharacterPlayer::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;
}

void AAxeCharacterPlayer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AAxeCharacterPlayer::InitAbility()
{
	// 初始化 Ability
	AAxePlayerState* AxePlayerState = GetPlayerState<AAxePlayerState>();
	check(AxePlayerState);

	AxePlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AxePlayerState, this);
	// AbilitySystemComponent
	AbilitySystemComponent = AxePlayerState->GetAbilitySystemComponent();
	// AttributeSet
	AttributeSet = AxePlayerState->GetAttributeSet();
	if (HasAuthority())
	{
		// init default attributes
		InitDefaultAttributes();
		// Give Startup Abilities
		GiveStartupAbilities();
	}
	// init HUD
	AAxePlayerController* PlayerController = Cast<AAxePlayerController>(GetController());
	if (PlayerController)
	{
		AAxeHUD* AxeHUD = Cast<AAxeHUD>(PlayerController->GetHUD());
		if (AxeHUD)
		{
			AxeHUD->InitOverlay(PlayerController, AxePlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	// Over
	OnAbilityInitOverDelegate.Broadcast();
	bIsAbilityInitOver = true;
}
