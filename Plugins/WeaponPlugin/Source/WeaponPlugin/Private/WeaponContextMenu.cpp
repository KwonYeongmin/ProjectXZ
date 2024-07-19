#include "WeaponContextMenu.h"
#include "ProjectXZ/Weapon/XZDA_Weapon.h"

FWeaponContextMenu::FWeaponContextMenu(EAssetTypeCategories::Type InCategory)
{
	Category = InCategory;
}

FText FWeaponContextMenu::GetName() const
{
	return FText::FromString("DataAsset_Weapon");
}

UClass* FWeaponContextMenu::GetSupportedClass() const
{
	return UXZDA_Weapon::StaticClass(); // factory�� ������ Ŭ���� ����.
}

FColor FWeaponContextMenu::GetTypeColor() const
{
	return FColor::Cyan;
}

uint32 FWeaponContextMenu::GetCategories()
{
	return Category; // ��Ŭ���ؼ� ������ ī�װ��� ����.
}
