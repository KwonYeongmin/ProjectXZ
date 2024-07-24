#include "XZPawnExtensionComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "XZCharacter.h"
#include "ProjectXZ/Component/XZInputComponent.h"
#include "ProjectXZ/Component/XZWeaponComponent.h"
#include "ProjectXZ/GameplayTag/XZGameplayTags.h"

UXZPawnExtensionComponent::UXZPawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;

	SetIsReplicatedByDefault(true);

	//PawnData = nullptr;
}

TObjectPtr<AXZCharacter> UXZPawnExtensionComponent::GetXZCharacter()
{
	if (IsValid(XZCharacter)) return XZCharacter;

	XZCharacter = GetPawn<AXZCharacter>();
	return XZCharacter;
}

void UXZPawnExtensionComponent::CheckDefaultInitialization()
{
	// Before checking our progress, try progressing any other features we might depend on
	CheckDefaultInitializationForImplementers();

	//static const TArray<FGameplayTag> StateChain = { LyraGameplayTags::InitState_Spawned, LyraGameplayTags::InitState_DataAvailable, LyraGameplayTags::InitState_DataInitialized, LyraGameplayTags::InitState_GameplayReady };

	// This will try to progress from spawned (which is only set in BeginPlay) through the data initialization stages until it gets to gameplay ready
	//ContinueInitStateChain(StateChain);
}

void UXZPawnExtensionComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	if (IsValid(Subsystem))
	{
		Subsystem->AddMappingContext(DefaultIMC, 0);
	}

	UXZInputComponent* XZInputComponent = Cast<UXZInputComponent>(PlayerInputComponent);
	if (false == IsValid(XZInputComponent)) UE_LOG(LogTemp, Warning, TEXT("nullptr == XZInputComponent"));

	if (IsValid(XZInputComponent))
	{
		XZInputComponent->BindNativeActions(InputConfig, FXZTags::GetXZTags().InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
		XZInputComponent->BindNativeActions(InputConfig, FXZTags::GetXZTags().InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse);
		XZInputComponent->BindNativeActions(InputConfig, FXZTags::GetXZTags().InputTag_Crouch, ETriggerEvent::Triggered, this, &ThisClass::Input_Crouch);

		// Weapon
		XZInputComponent->BindNativeActions(InputConfig, FXZTags::GetXZTags().InputTag_1, ETriggerEvent::Triggered, this, &ThisClass::Input_EquipSlot1);
		XZInputComponent->BindNativeActions(InputConfig, FXZTags::GetXZTags().InputTag_2, ETriggerEvent::Triggered, this, &ThisClass::Input_EquipSlot2);
		XZInputComponent->BindNativeActions(InputConfig, FXZTags::GetXZTags().InputTag_Weapon_Fire, ETriggerEvent::Triggered, this, &ThisClass::Input_WeaponFire);
	}
}

void UXZPawnExtensionComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	if (IsValid(Controller))
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

		if (Value.X != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}
void UXZPawnExtensionComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	if (false == IsValid(Pawn)) return;

	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		Pawn->AddControllerPitchInput(Value.Y);
	}
}

void UXZPawnExtensionComponent::Input_Crouch(const FInputActionValue& InputActionValue)
{
	if (AXZCharacter* Character = GetPawn<AXZCharacter>())
	{
		// TODO: Crouch �ɱ� �����ϱ�
		//Character->ToggleCrouch();
	}
}

void UXZPawnExtensionComponent::Input_EquipSlot1(const FInputActionValue& InputActionValue)
{
	if (GetXZCharacter() && GetXZCharacter()->GetWeaponComponent())
	{
		// TODO : ���� Tag �ϵ��ڵ��Ѱ� ����¥��
		FGameplayTag Tag = FXZTags::GetXZTags().Weapon_Projectile_Pistol;
		GetXZCharacter()->GetWeaponComponent()->EquipWeapon(Tag);
	}
}

void UXZPawnExtensionComponent::Input_EquipSlot2(const FInputActionValue& InputActionValue)
{
	if (GetXZCharacter() && GetXZCharacter()->GetWeaponComponent())
	{
		// TODO : ���� Tag �ϵ��ڵ��Ѱ� ����¥��
		FGameplayTag Tag = FXZTags::GetXZTags().Weapon_Projectile_Rifle;
		GetXZCharacter()->GetWeaponComponent()->EquipWeapon(Tag);
	}
}

void UXZPawnExtensionComponent::Input_WeaponFire(const FInputActionValue& InputActionValue)
{
	if (GetXZCharacter() && GetXZCharacter()->GetWeaponComponent())
	{
		// TODO : ���� Tag �ϵ��ڵ��Ѱ� ����¥��
		FGameplayTag Tag = FXZTags::GetXZTags().Weapon_Projectile_Rifle;
		GetXZCharacter()->GetWeaponComponent()->Fire(Tag);
	}
}
