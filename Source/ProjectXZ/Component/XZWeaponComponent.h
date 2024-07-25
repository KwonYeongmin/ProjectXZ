#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "XZWeaponComponent.generated.h"

class UXZWeaponData;
class UXZDA_Weapon;
class AXZCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTXZ_API UXZWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UXZWeaponComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	TObjectPtr<AXZCharacter> GetXZCharacter();

	// ���
	void EquipWeapon(const FGameplayTag& InTag);
	void Fire(const FGameplayTag& InTag);

protected:
	virtual void BeginPlay() override;


private:
	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Data", meta = (AllowPrivateAccess = true))
	TMap<FGameplayTag, UXZDA_Weapon*> WeaponList; // ���� ���

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Data", meta = (AllowPrivateAccess = true))
	TArray<FGameplayTag> Init_WeaponTags; // ���� �� ������ �ִ� ����


	UPROPERTY()
	TMap<FGameplayTag, UXZWeaponData*> Datas; // ���� ���


	TObjectPtr<AXZCharacter> OwnerCharacter;

	FVector HitTarget; // �Ѿ��� �߻�Ǽ� �浹�ϰ� �� ����
};
