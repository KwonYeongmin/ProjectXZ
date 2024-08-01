#include "XZWeaponComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "ProjectXZ/Character/XZCharacter.h"
#include "ProjectXZ/Character/XZPlayerController.h"
#include "ProjectXZ/GameplayTag/XZGameplayTags.h"
#include "ProjectXZ/HUD/XZHUD.h"
#include "ProjectXZ/Weapon/XZDA_Weapon.h"
#include "ProjectXZ/Weapon/XZEquipment.h"
#include "ProjectXZ/Weapon/XZWeaponData.h"
#include "ProjectXZ/Weapon/Combat/XZCombat.h"

UXZWeaponComponent::UXZWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UXZWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(UXZWeaponComponent, EquippedWeaponTag, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME(UXZWeaponComponent, bIsAiming);
}

void UXZWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetXZCharacter() && GetXZCharacter()->IsLocallyControlled())
	{
		FHitResult HitResult;
		TraceUnderCrosshairs(HitResult); // Crosshair���� LineTrace�� ��� HitResult ���� ������Ʈ�Ѵ�.
		HitTarget = HitResult.ImpactPoint;

		SetHUDCrosshairs(DeltaTime);
		InterpFOV(DeltaTime);
	}
}

TObjectPtr<AXZCharacter> UXZWeaponComponent::GetXZCharacter()
{
	if (IsValid(OwnerCharacter)) return OwnerCharacter;

	OwnerCharacter = Cast<AXZCharacter>(GetOwner());
	return OwnerCharacter;
}

TObjectPtr<AXZPlayerController> UXZWeaponComponent::GetXZPlayerController()
{
	if (IsValid(XZPlayerController)) return XZPlayerController;

	XZPlayerController = Cast<AXZPlayerController>(GetXZCharacter()->GetController());
	return XZPlayerController;
}

TObjectPtr<AXZHUD> UXZWeaponComponent::GetXZHUD()
{
	if (XZHUD) return XZHUD;

	XZHUD = Cast<AXZHUD>(GetXZPlayerController()->GetHUD());
	return XZHUD;
}

void UXZWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ���� ���
	for (const TTuple<FGameplayTag, UXZDA_Weapon*>& Weapon : WeaponList)
	{
		UXZWeaponData* Temp = NewObject<UXZWeaponData>(this);
		Datas.Add(Weapon.Key, Temp);
	}

	// ���� ���� ����
	for (const FGameplayTag& Tag : Init_WeaponTags)
	{
		WeaponList[Tag]->CreateInstance(GetXZCharacter(), &Datas[Tag]);
	}
}

void UXZWeaponComponent::TraceUnderCrosshairs(FHitResult& TraceHitResult)
{
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	FVector2D CrosshairLocation(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f); // ȭ�� �߾�
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection
	); // DeprojectScreenToWorld �����ϸ� true, �����ϸ� false

	if (bScreenToWorld)
	{
		FVector Start = CrosshairWorldPosition;

		if (GetXZCharacter())
		{
			float DistanceToCharacter = (GetXZCharacter()->GetActorLocation() - Start).Size();
			Start += CrosshairWorldDirection * (DistanceToCharacter + 100.0f);
			//DrawDebugSphere(GetWorld(), Start, 4.0f, 12, FColor::Blue, false); // ������
		}

		FVector End = Start + CrosshairWorldDirection * 80000.0f; // TRACE_LENGTH 80000.0f

		GetWorld()->LineTraceSingleByChannel(TraceHitResult, 	Start, End, ECollisionChannel::ECC_Visibility);

		if (TraceHitResult.bBlockingHit == false) // �ϴ� ���� �浹�Ұ� ���°��� ��� ���
		{
			TraceHitResult.ImpactPoint = End; // �浹�ϴ°� ���ٸ� End ���� ImpactPoint������ ����.
		}
		
	}
}

void UXZWeaponComponent::EquipWeapon(const FGameplayTag& InTag)
{
	Server_EquipWeapon(InTag);
}

void UXZWeaponComponent::OnRep_EquippedChanged()
{

}

void UXZWeaponComponent::Server_EquipWeapon_Implementation(const FGameplayTag& InTag)
{
	EquippedWeaponTag = InTag;
	Multicast_EquipWeapon(InTag);
}

void UXZWeaponComponent::Multicast_EquipWeapon_Implementation(const FGameplayTag& InTag)
{
	// ���Կ� ��ϵ� ������ GameplayTag�� InTag�� ������
	
	if (UXZWeaponData** FoundData = Datas.Find(InTag))
	{
		if (*FoundData)
		{
			if (Datas[InTag]->GetEquipment()->Equip()) // �������� O
			{

			}
			else // �������� X (= Unequip ������ ���)
			{
				Datas[InTag]->GetEquipment()->Unequip();
				EquippedWeaponTag = FXZTags::GetXZTags().Fist;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("FoundData(=XZWeaponData) is nullptr"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon is not found. Check UXZWeaponComponent::EquipWeapon"));
	}
}


void UXZWeaponComponent::Fire()
{
	if (GetXZCharacter() && GetXZCharacter()->IsLocallyControlled())
	{
		Server_Fire(HitTarget);
	}
}

void UXZWeaponComponent::Server_Fire_Implementation(const FVector_NetQuantize& HitLocation)
{
	Multicast_Fire(EquippedWeaponTag, HitLocation);
}

void UXZWeaponComponent::Multicast_Fire_Implementation(const FGameplayTag& InTag, const FVector_NetQuantize& HitLocation)
{
	if (UXZWeaponData** FoundData = Datas.Find(InTag))
	{
		if (*FoundData)
		{
			if (Datas[InTag]->GetCombat()->GetBulletData().Ammo <= 0) // �Ѿ��� ���ٸ�
			{
				Reload(InTag); // ������
				return;
			}
			
			UE_LOG(LogTemp, Warning, TEXT("Ammo = %d"), Datas[InTag]->GetCombat()->GetBulletData().Ammo);
			Datas[InTag]->GetCombat()->FireAction(HitLocation);
			Datas[InTag]->GetCombat()->ConsumeAmmo();

			if (Datas[InTag]->GetCombat()->GetBulletData().Ammo <= 0) // �Ѿ��� ���ٸ�
			{
				Reload(InTag); // ������
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("FoundData(=XZWeaponData) is nullptr"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayTag is not found in Datas"));
	}
}

void UXZWeaponComponent::Reload(const FGameplayTag& InTag)
{
	if (Datas[InTag]->GetCombat()->GetBulletData().TotalAmmo <= 0) return; // �Ѿ� ���� ��� return

	if (UXZWeaponData** FoundData = Datas.Find(InTag))
	{
		if (*FoundData)
		{
			Datas[InTag]->GetCombat()->ReloadAction();

			// �Ѿ� ä���
			if (Datas[InTag]->GetCombat()->GetBulletData().TotalAmmo >= Datas[InTag]->GetCombat()->GetBulletData().MagCapacity)
			{
				Datas[InTag]->GetCombat()->GetBulletData().Ammo = Datas[InTag]->GetCombat()->GetBulletData().MagCapacity;
				Datas[InTag]->GetCombat()->GetBulletData().TotalAmmo -= Datas[InTag]->GetCombat()->GetBulletData().MagCapacity;
			}
			else
			{
				Datas[InTag]->GetCombat()->GetBulletData().Ammo = Datas[InTag]->GetCombat()->GetBulletData().TotalAmmo;
				Datas[InTag]->GetCombat()->GetBulletData().TotalAmmo = 0;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("FoundData(=XZWeaponData) is nullptr"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameplayTag is not found in Datas"));
	}
}

void UXZWeaponComponent::StartAiming()
{
	if (false == IsValid(GetXZCharacter())) return;

	GetXZCharacter()->GetCharacterMovement()->MaxWalkSpeed = AimWalkSpeed;
	bIsAiming = true;
}

void UXZWeaponComponent::EndAiming()
{
	if (false == IsValid(GetXZCharacter())) return;

	GetXZCharacter()->GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	bIsAiming = false;
}

void UXZWeaponComponent::InterpFOV(float InDeltaTime)
{
	if (false == EquippedWeaponTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Weapon"))))) return; // ���� ���� ���� �ƴ϶�� return

	if (bIsAiming)
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, ZoomedFOV, InDeltaTime, 20.0f);
	}
	else
	{
		CurrentFOV = FMath::FInterpTo(CurrentFOV, DefaultFOV, InDeltaTime, 20.0f);
	}

	GetXZCharacter()->GetFollowCamera()->SetFieldOfView(CurrentFOV);
}

void UXZWeaponComponent::SetHUDCrosshairs(float InDeltaTime)
{
	if (false == IsValid(GetXZCharacter()) || false == IsValid(GetXZPlayerController())) return;
	if (false == IsValid(GetXZHUD())) return;
	
	if (EquippedWeaponTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName(TEXT("Weapon")))))
	{
		// TODO: ���� �� Crosshair �����ϱ�
		GetXZHUD()->CrosshairTexture2D = CrosshairTexture2D;
		GetXZHUD()->DrawHUD();
	}
	else
	{
		GetXZHUD()->CrosshairTexture2D = nullptr;
	}
}
