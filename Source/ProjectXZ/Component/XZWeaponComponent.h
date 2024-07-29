#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "ProjectXZ/GameplayTag/XZGameplayTags.h"
#include "XZWeaponComponent.generated.h"

class UXZWeaponData;
class UXZDA_Weapon;
class AXZCharacter;
class AXZPlayerController;
class AXZHUD;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTXZ_API UXZWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UXZWeaponComponent();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	TObjectPtr<AXZCharacter> GetXZCharacter();
	TObjectPtr<AXZPlayerController> GetXZPlayerController();
	TObjectPtr<AXZHUD> GetXZHUD();

	// ���
	void EquipWeapon(const FGameplayTag& InTag);
	void Fire();
	void Reload(const FGameplayTag& InTag);
	void StartAiming();
	void EndAiming();

protected:
	virtual void BeginPlay() override;

private:
	void TraceUnderCrosshairs(FHitResult& TraceHitResult);
	void SetHUDCrosshairs(float InDeltaTime);
	void InterpFOV(float InDeltaTime);

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Data", meta = (AllowPrivateAccess = true))
	TMap<FGameplayTag, UXZDA_Weapon*> WeaponList; // ��ü ���� ���(�����Ϳ��� ���)

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Data", meta = (AllowPrivateAccess = true))
	TArray<FGameplayTag> Init_WeaponTags; // ���� �� ������ �ִ� ����

	UPROPERTY()
	TMap<FGameplayTag, UXZWeaponData*> Datas; // ���� ������ �ִ� ����� ������

	UPROPERTY()
	FGameplayTag EquippedWeaponTag = FXZTags::GetXZTags().Fist; // ���� ���� ���� ����

	TObjectPtr<AXZCharacter> OwnerCharacter;
	FVector HitTarget; // �Ѿ��� �߻�Ǽ� �浹�ϰ� �� ����

	//***************************************************************
	//** Crosshairs
	TObjectPtr<AXZHUD> XZHUD;
	TObjectPtr<AXZPlayerController> XZPlayerController;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> CrosshairTexture2D;
	//***************************************************************

	//***************************************************************
	//** Aiming
	bool bIsAiming = false;

	float CurrentFOV = 90.0f; // ���� FOV ��
	UPROPERTY(EditDefaultsOnly, Category = "Aiming Data", meta = (AllowPrivateAccess = true))
	float ZoomedFOV = 30.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Aiming Data", meta = (AllowPrivateAccess = true))
	float DefaultFOV = 90.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Aiming Data", meta = (AllowPrivateAccess = true))
	float AimWalkSpeed = 200.0f;
	UPROPERTY(EditDefaultsOnly, Category = "Aiming Data", meta = (AllowPrivateAccess = true))
	float MaxWalkSpeed = 600.0f;
	//***************************************************************

};
