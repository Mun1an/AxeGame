// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/AxeCharacterPlayer.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "ActionSystem/ActionCombatComponent.h"
#include "ActionSystem/ComboActionComponent.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Character/ModularCharacterComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Inventory/Component/InventoryComponent.h"
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

	RetargetCharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RetargetCharacterMesh"));
	RetargetCharacterMesh->SetupAttachment(GetMesh());
	RetargetCharacterMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(RetargetCharacterMesh, WeaponTipSocketName);
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponSecondary = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponSecondary"));
	WeaponSecondary->SetupAttachment(RetargetCharacterMesh, WeaponSecondaryTipSocketName);
	WeaponSecondary->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// ComboActionComponent
	ComboActionComponent = CreateDefaultSubobject<UComboActionComponent>(TEXT("ComboActionComponent"));
	// ActionCombatComponent
	ActionCombatComponent = CreateDefaultSubobject<UActionCombatComponent>(TEXT("ActionCombatComponent"));
	// InventoryComponent
	// InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	// ModularCharacterComponent
	ModularCharacterComponent = CreateDefaultSubobject<UModularCharacterComponent>(TEXT("ModularCharacterComponent"));

	ModularSkeletalMeshComponentMap = TMap<EAxeModularCharacterSM, TWeakObjectPtr<USkeletalMeshComponent>>();

	HandleModularSkeletalMeshComponent(ModularSM_Gender_Head,
	                                   "Gender_Head",
	                                   EAxeModularCharacterSM::Gender_Head);
	HandleModularSkeletalMeshComponent(ModularSM_Gender_Eyebrow,
	                                   "Gender_Eyebrow",
	                                   EAxeModularCharacterSM::Gender_Eyebrow);
	HandleModularSkeletalMeshComponent(ModularSM_Gender_FacialHair,
	                                   "Gender_FacialHair",
	                                   EAxeModularCharacterSM::Gender_FacialHair);
	HandleModularSkeletalMeshComponent(ModularSM_Gender_Torso,
	                                   "Gender_Torso",
	                                   EAxeModularCharacterSM::Gender_Torso);
	HandleModularSkeletalMeshComponent(ModularSM_Gender_ArmUpperRight,
	                                   "Gender_ArmUpperRight",
	                                   EAxeModularCharacterSM::Gender_ArmUpperRight);
	HandleModularSkeletalMeshComponent(ModularSM_Gender_ArmUpperLeft,
	                                   "Gender_ArmUpperLeft",
	                                   EAxeModularCharacterSM::Gender_ArmUpperLeft);
	HandleModularSkeletalMeshComponent(ModularSM_Gender_ArmLowerRight,
	                                   "Gender_ArmLowerRight",
	                                   EAxeModularCharacterSM::Gender_ArmLowerRight);
	HandleModularSkeletalMeshComponent(ModularSM_Gender_HandRight,
	                                   "Gender_HandRight",
	                                   EAxeModularCharacterSM::Gender_HandRight);
	HandleModularSkeletalMeshComponent(ModularSM_Gender_ArmLowerLeft,
	                                   "Gender_ArmLowerLeft",
	                                   EAxeModularCharacterSM::Gender_ArmLowerLeft);
	HandleModularSkeletalMeshComponent(ModularSM_Gender_HandLeft,
	                                   "Gender_HandLeft",
	                                   EAxeModularCharacterSM::Gender_HandLeft);
	HandleModularSkeletalMeshComponent(ModularSM_Gender_Hips,
	                                   "Gender_Hips",
	                                   EAxeModularCharacterSM::Gender_Hips);
	HandleModularSkeletalMeshComponent(ModularSM_Gender_LegRight,
	                                   "Gender_LegRight",
	                                   EAxeModularCharacterSM::Gender_LegRight);
	HandleModularSkeletalMeshComponent(ModularSM_Gender_LegLeft,
	                                   "Gender_LegLeft",
	                                   EAxeModularCharacterSM::Gender_LegLeft);
	HandleModularSkeletalMeshComponent(ModularSM_All_HeadCoverings_NoHair,
	                                   "All_HeadCoverings_NoHair",
	                                   EAxeModularCharacterSM::All_HeadCoverings_NoHair);
	HandleModularSkeletalMeshComponent(ModularSM_All_HeadCoverings_BaseHair,
	                                   "All_HeadCoverings_BaseHair",
	                                   EAxeModularCharacterSM::All_HeadCoverings_BaseHair);
	HandleModularSkeletalMeshComponent(ModularSM_All_HeadCoverings_NoFacialHair,
	                                   "All_HeadCoverings_NoFacialHair",
	                                   EAxeModularCharacterSM::All_HeadCoverings_NoFacialHair);
	HandleModularSkeletalMeshComponent(ModularSM_All_Hair,
	                                   "All_Hair",
	                                   EAxeModularCharacterSM::All_Hair);
	HandleModularSkeletalMeshComponent(ModularSM_All_HeadAttachment,
	                                   "All_HeadAttachment",
	                                   EAxeModularCharacterSM::All_HeadAttachment);
	HandleModularSkeletalMeshComponent(ModularSM_All_ChestAttachment,
	                                   "All_ChestAttachment",
	                                   EAxeModularCharacterSM::All_ChestAttachment);
	HandleModularSkeletalMeshComponent(ModularSM_All_BackAttachment,
	                                   "All_BackAttachment",
	                                   EAxeModularCharacterSM::All_BackAttachment);
	HandleModularSkeletalMeshComponent(ModularSM_All_ShoulderAttachment_Right,
	                                   "All_ShoulderAttachment_Right",
	                                   EAxeModularCharacterSM::All_ShoulderAttachment_Right);
	HandleModularSkeletalMeshComponent(ModularSM_All_ShoulderAttachment_Left,
	                                   "All_ShoulderAttachment_Left",
	                                   EAxeModularCharacterSM::All_ShoulderAttachment_Left);
	HandleModularSkeletalMeshComponent(ModularSM_All_ElbowAttachment_Right,
	                                   "All_ElbowAttachment_Right",
	                                   EAxeModularCharacterSM::All_ElbowAttachment_Right);
	HandleModularSkeletalMeshComponent(ModularSM_All_ElbowAttachment_Left,
	                                   "All_ElbowAttachment_Left",
	                                   EAxeModularCharacterSM::All_ElbowAttachment_Left);
	HandleModularSkeletalMeshComponent(ModularSM_All_HipsAttachment,
	                                   "All_HipsAttachment",
	                                   EAxeModularCharacterSM::All_HipsAttachment);
	HandleModularSkeletalMeshComponent(ModularSM_All_KneeAttachment_Right,
	                                   "All_KneeAttachment_Right",
	                                   EAxeModularCharacterSM::All_KneeAttachment_Right);
	HandleModularSkeletalMeshComponent(ModularSM_All_KneeAttachment_Left,
	                                   "All_KneeAttachment_Left",
	                                   EAxeModularCharacterSM::All_KneeAttachment_Left);
	HandleModularSkeletalMeshComponent(ModularSM_All_Extra,
	                                   "All_Extra",
	                                   EAxeModularCharacterSM::All_Extra);
}

void AAxeCharacterPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// server
	InitAbility();
	InitInventory();
}

void AAxeCharacterPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// client
	InitAbility();
	InitInventory();
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
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(AbilitySystemComponent);
	AxeASC->OnCharacterASCInitOverCallback();
	OnAbilityInitOverDelegate.Broadcast();
	bIsAbilityInitOver = true;
}

void AAxeCharacterPlayer::InitInventory()
{
	AAxePlayerState* AxePlayerState = GetPlayerState<AAxePlayerState>();
	check(AxePlayerState);
	InventoryComponent = AxePlayerState->GetInventoryComponent();
	InventoryComponent->SetOwnerActor(AxePlayerState);
	InventoryComponent->SetAvatarActor(this);
}

void AAxeCharacterPlayer::HandleModularSkeletalMeshComponent(TObjectPtr<USkeletalMeshComponent>& SMComp,
                                                             FName CompName,
                                                             EAxeModularCharacterSM SMEnum)
{
	SMComp = CreateDefaultSubobject<USkeletalMeshComponent>(CompName);
	SMComp->SetupAttachment(RetargetCharacterMesh);
	SMComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ModularSkeletalMeshComponentMap.Add(SMEnum, SMComp);
}
