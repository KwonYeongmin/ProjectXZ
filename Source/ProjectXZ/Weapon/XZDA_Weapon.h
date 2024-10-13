#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponLibrary.h"
#include "XZDA_Weapon.generated.h"

class UXZWeaponData;
class AXZAttachment;
class UXZEquipment;
class UXZAim;
class UXZCombat;

/** 무기 관련 정보를 기록하는 DataAsset 클래스
 * 
 */

UCLASS()
class PROJECTXZ_API UXZDA_Weapon : public UDataAsset
{
	GENERATED_BODY()

public:
	UXZDA_Weapon();
	void CreateInstance(ACharacter* InOwner, UXZWeaponData** OutWeaponData);


#if WITH_EDITOR //Editor 내에서만 수행
	void PostEditChangeChainProperty(struct FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif

	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AXZAttachment> AttachmentClass;
	UPROPERTY()
	TObjectPtr<AXZAttachment> Attachment;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UXZEquipment> EquipmentClass;
	UPROPERTY(EditAnywhere)
	FEquipmentData EquipmentData;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UXZAim> AimClass;
	UPROPERTY(EditAnywhere)
	FAimData AimData;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UXZCombat> CombatClass;
	UPROPERTY(EditAnywhere)
	TArray<FActionData> ActionData;
	UPROPERTY(EditAnywhere)
	FBulletData BulletData;
};
