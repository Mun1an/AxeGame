// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/AxeCharacterPlayer.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AxeAbilitySystemComponent.h"
#include "ActionSystem/ComboActionComponent.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "PlayerState/AxePlayerState.h"
#include "MotionWarpingComponent.h"

// AAxeCharacter

AAxeCharacterPlayer::AAxeCharacterPlayer()
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

	WeaponComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	WeaponComponent->SetupAttachment(GetMesh(), WeaponTipSocketName);
	WeaponComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponSecondaryComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSecondary"));
	WeaponSecondaryComponent->SetupAttachment(GetMesh(), WeaponSecondaryTipSocketName);
	WeaponSecondaryComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// ComboActionComponent
	ComboActionComponent = CreateDefaultSubobject<UComboActionComponent>(TEXT("ComboActionComponent"));

	// MotionWarpingComponent
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
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

	// UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(AbilitySystemComponent);
	// if (AxeASC)
	// {
	// 	TMap<EAxeAbilityActivationGroup, TArray<FGameplayAbilitySpecHandle>> ActivationGroupMap = AxeASC->
	// 		GetActivationGroupMap();
	//
	// 	FString Msg = "";
	// 	for (TTuple<EAxeAbilityActivationGroup, TArray<FGameplayAbilitySpecHandle>>
	// 	     GroupMap : ActivationGroupMap)
	// 	{
	// 		Msg += FString::Printf(TEXT("Group: %d"), GroupMap.Key);
	//
	// 		if (GroupMap.Value.Num() > 0)
	// 		{
	// 			for (FGameplayAbilitySpecHandle SpecHandle : GroupMap.Value)
	// 			{
	// 				UGameplayAbility* GameplayAbility = AxeASC->FindAbilitySpecFromHandle(SpecHandle)->Ability;
	// 				Msg += FString::Printf(TEXT("  %s  "), *GameplayAbility->GetName());
	// 			}
	// 		}
	// 		Msg += "\n";
	// 	}
	// 	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Orange, Msg);
	// }
}

void AAxeCharacterPlayer::InitAbility()
{
	// 初始化 Ability
	AAxePlayerState* AxePlayerState = GetPlayerState<AAxePlayerState>();
	check(AxePlayerState);

	AxePlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AxePlayerState, this);
	// AbilitySystemComponent
	AbilitySystemComponent = AxePlayerState->GetAbilitySystemComponent();

	// Give Startup Abilities
	if (HasAuthority())
	{
		UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(AbilitySystemComponent);
		for (TSubclassOf<UGameplayAbility> Ability : StartupAbilities)
		{
			AxeASC->GiveAbilityByAbilityAndLevel(Ability, 1);
		}
	}
	// AttributeSet
	AttributeSet = AxePlayerState->GetAttributeSet();

	// init default attributes
	InitDefaultAttributes();

	// Over
	OnAbilityInitOverDelegate.Broadcast();
	bIsAbilityInitOver = true;
}
