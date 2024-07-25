#pragma once
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "WeaponLibrary.generated.h"

/**
 * 
 */

class AXZProjectile;
USTRUCT()
struct FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* EquipMontage;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> UnequipMontage;

	UPROPERTY(EditDefaultsOnly)
	float Equip_FrameTime = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	float Unequip_FrameTime = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	FName HolsterSocketName;

	UPROPERTY(EditDefaultsOnly)
	FName WeaponSocketName;
};

USTRUCT()
struct FActionData 
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* ActionMontage; // ���� ��Ÿ��(�߻�, �ܰ� ���� ��)

	UPROPERTY(EditDefaultsOnly)
	float Action_FrameTime = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	FName MuzzleSocketName = TEXT("MuzzleFlash");

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AXZProjectile> ProjectileClass;
};
