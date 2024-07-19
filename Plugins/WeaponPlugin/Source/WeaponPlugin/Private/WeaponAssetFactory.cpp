#include "WeaponAssetFactory.h"
#include "ProjectXZ/Weapon/XZDA_Weapon.h"

UWeaponAssetFactory::UWeaponAssetFactory()
{
	bCreateNew = true;

	// Factory.h�� SupportedClass�� ����. ���� �����Ϳ����� Ŭ���� Ÿ������ factory ����.
	SupportedClass = UXZDA_Weapon::StaticClass();
}

// ���� ������ ����.
UObject* UWeaponAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn, FName CallingContext)
{
	UXZDA_Weapon* DA_Weapon = NewObject<UXZDA_Weapon>(InParent, InClass, InName, Flags);
	if (IsValid(DA_Weapon)) return DA_Weapon;

	return nullptr;
}
