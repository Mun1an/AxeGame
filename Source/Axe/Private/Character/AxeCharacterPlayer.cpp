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
#include "GameplayTag/AxeGameplayTags.h"
#include "Inventory/Component/InventoryComponent.h"
#include "Item/WeaponTypeDataAsset.h"
#include "Item/Instance/ItemDefinition.h"
#include "Item/Instance/ItemInstance.h"
#include "Item/ItemFragment/ItemFragment_CommonInfo.h"
#include "Item/ItemFragment/ItemFragment_ModularCharacterMesh.h"
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

	WeaponSMComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	WeaponSMComponent->SetupAttachment(RetargetCharacterMesh, WeaponTipSocketName);
	WeaponSMComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	WeaponSecondarySMComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponSecondary"));
	WeaponSecondarySMComponent->SetupAttachment(RetargetCharacterMesh, WeaponSecondaryTipSocketName);
	WeaponSecondarySMComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
	HandleModularSkeletalMeshComponent(ModularSM_Gender_Head_NoElements,
	                                   "Gender_Head_NoElements",
	                                   EAxeModularCharacterSM::Gender_Head_NoElements);
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
	HandleModularSkeletalMeshComponent(ModularSM_All_HeadAttachmentHair,
	                                   "All_HeadAttachmentHair",
	                                   EAxeModularCharacterSM::All_HeadAttachmentHair);
	HandleModularSkeletalMeshComponent(ModularSM_All_HeadAttachmentHelmet,
	                                   "All_HeadAttachmentHelmet",
	                                   EAxeModularCharacterSM::All_HeadAttachmentHelmet);
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

void AAxeCharacterPlayer::SetLinkedAnimLayerClass(TSubclassOf<UAnimInstance>& InAnimInstanceClass)
{
	LinkedAnimLayerClass = InAnimInstanceClass;
	OnLinkedAnimLayerClassChanged();
}

void AAxeCharacterPlayer::OnLinkedAnimLayerClassChanged()
{
	GetMesh()->LinkAnimClassLayers(LinkedAnimLayerClass);
}

void AAxeCharacterPlayer::SetCurrentWeaponType(EAxePlayerWeaponType InWeaponType)
{
	const EAxePlayerWeaponType OldWeaponType = CurrentWeaponType;
	CurrentWeaponType = InWeaponType;
	OnCurrentWeaponTypeChanged(InWeaponType, OldWeaponType);
}

void AAxeCharacterPlayer::OnCurrentWeaponTypeChanged(const EAxePlayerWeaponType NewWeaponType,
                                                     const EAxePlayerWeaponType OldWeaponType)
{
	UWeaponTypeDataAsset* NewWeaponTypeDataAsset = WeaponTypeDataAssetMap.FindRef(NewWeaponType);
	UWeaponTypeDataAsset* OldWeaponTypeDataAsset = WeaponTypeDataAssetMap.FindRef(OldWeaponType);
	if (!NewWeaponTypeDataAsset || !OldWeaponTypeDataAsset)
	{
		return;
	}
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	UAxeAbilitySystemComponent* AxeASC = Cast<UAxeAbilitySystemComponent>(ASC);
	
	if (HasAuthority() && OldWeaponTypeDataAsset->WeaponAbilities.Num() > 0)
	{
		// Remove Old Weapon Abilities
		for (const TSubclassOf<UGameplayAbility>& Ability : OldWeaponTypeDataAsset->WeaponAbilities)
		{
			FGameplayTag AbilityGameplayTag = Ability.GetDefaultObject()->AbilityTags.First();
			FGameplayAbilitySpecHandle Handle = AxeASC->GetAbilityHandleByAbilityTag(AbilityGameplayTag);
			AxeASC->ClearAbility(Handle);
		}
	}
	if (HasAuthority() && NewWeaponTypeDataAsset->WeaponAbilities.Num() > 0)
	{
		// Add New Weapon Abilities
		for (const TSubclassOf<UGameplayAbility>& Ability : NewWeaponTypeDataAsset->WeaponAbilities)
		{
			AxeASC->GiveAbilityByAbilityAndLevel(Ability, 1);
		}
	}

	if (NewWeaponTypeDataAsset->WeaponAnimLayerClass)
	{
		SetLinkedAnimLayerClass(NewWeaponTypeDataAsset->WeaponAnimLayerClass);
	}

	if (NewWeaponTypeDataAsset->WeaponComboDataAsset)
	{
		GetComboActionComponent()->InitComboAbilityTree(NewWeaponTypeDataAsset->WeaponComboDataAsset);
	}
}

void AAxeCharacterPlayer::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;

	SetLinkedAnimLayerClass(LinkedAnimLayerClass);
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

	OnInventoryInitOverDelegate.Broadcast();
	bIsInventoryInitOver = true;

	// Init Over
	InventoryComponent->OnEquipmentItemChangedDelegate.AddDynamic(
		this, &AAxeCharacterPlayer::OnEquipmentItemChanged
	);
	InventoryComponent->RefreshInventoryItemEntryChange();
}

void AAxeCharacterPlayer::OnEquipmentItemChanged(int32 SlotIndex, UItemInstance* NewItemInstance,
                                                 UItemInstance* OldItemInstance, FGameplayTagContainer SlotTags)
{
	const FAxeGameplayTags AxeGameplayTags = FAxeGameplayTags::Get();

	// Weapon
	if (SlotTags.HasTag(AxeGameplayTags.Inventory_Entry_Equipment_Weapon))
	{
		if (NewItemInstance)
		{
			const UItemDefinition* NewItemDef = GetDefault<UItemDefinition>(NewItemInstance->GetItemDef());
			const UItemFragment_ModularCharacterMesh* MeshFragment = NewItemDef->FindFragment<
				UItemFragment_ModularCharacterMesh>();

			// Set Weapon Mesh
			WeaponSMComponent->SetStaticMesh(MeshFragment->CharacterWeaponMeshInfo.WeaponMeshStaticMesh);
			WeaponSMComponent->SetRelativeLocation(MeshFragment->CharacterWeaponMeshInfo.WeaponOffset);
			WeaponSMComponent->SetRelativeRotation(MeshFragment->CharacterWeaponMeshInfo.WeaponRotation);
			WeaponSMComponent->SetRelativeScale3D(MeshFragment->CharacterWeaponMeshInfo.WeaponScale);
			
			WeaponSecondarySMComponent->SetStaticMesh(MeshFragment->CharacterWeaponMeshInfo.WeaponSecondaryStaticMesh);
			WeaponSecondarySMComponent->SetRelativeLocation(MeshFragment->CharacterWeaponMeshInfo.WeaponOffset);
			WeaponSecondarySMComponent->SetRelativeRotation(MeshFragment->CharacterWeaponMeshInfo.WeaponRotation);
			WeaponSecondarySMComponent->SetRelativeScale3D(MeshFragment->CharacterWeaponMeshInfo.WeaponScale);

			// Set Weapon Type
			const UItemFragment_CommonInfo* CommonInfo = NewItemDef->FindFragment<UItemFragment_CommonInfo>();
			if (CommonInfo)
			{
				const EAxePlayerWeaponType NewWeaponType = AxeGameplayTags.GetWeaponTypeByTag(
					CommonInfo->ItemTypeTag);
				SetCurrentWeaponType(NewWeaponType);
			}
		}
		else
		{
			WeaponSMComponent->SetStaticMesh(nullptr);
			WeaponSecondarySMComponent->SetStaticMesh(nullptr);

			SetCurrentWeaponType(EAxePlayerWeaponType::None);
		}
	}
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
