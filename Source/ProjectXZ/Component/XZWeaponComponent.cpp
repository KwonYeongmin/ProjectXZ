#include "XZWeaponComponent.h"
#include "ProjectXZ/Character/XZCharacter.h"
#include "ProjectXZ/GameplayTag/XZGameplayTags.h"
#include "ProjectXZ/Weapon/XZDA_Weapon.h"
#include "ProjectXZ/Weapon/XZEquipment.h"
#include "ProjectXZ/Weapon/XZWeaponData.h"

UXZWeaponComponent::UXZWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

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
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	for (const FGameplayTag& Tag : Init_WeaponTags)
	{
		WeaponList[Tag]->CreateInstance(OwnerCharacter, &Datas[Tag]);
	}
}

void UXZWeaponComponent::EquipWeapon(const FGameplayTag& InTag)
{
	// ���Կ� ��ϵ� ������ GameplayTag�� InTag�� ������

	if (UXZWeaponData** FoundData = Datas.Find(InTag))
	{
		if (*FoundData)
		{
			(*FoundData)->GetEquipment()->Equip();
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

void UXZWeaponComponent::Fire(const FGameplayTag& InTag)
{
	if (UXZWeaponData** FoundData = Datas.Find(InTag))
	{
		if (*FoundData)
		{
			// 
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
