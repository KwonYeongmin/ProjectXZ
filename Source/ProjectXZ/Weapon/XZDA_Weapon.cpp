#include "XZDA_Weapon.h"
#include "Attachment/XZAttachment.h"
#include "XZEquipment.h"
#include "XZWeaponData.h"
#include "GameFramework/Character.h"

UXZDA_Weapon::UXZDA_Weapon()
{
	AttachmentClass = AXZAttachment::StaticClass();
	EquipmentClass = UXZEquipment::StaticClass();
}

// ��ü�� �����ؼ� �����Ѵ�. ���� �����̱� ������ ���� �����͸� ���
void UXZDA_Weapon::CreateInstance(ACharacter* InOwner, UXZWeaponData** OutWeaponData)
{
	//XZWeaponData.h�� �������� �̵��Ͽ��� XZAttachment, XZEquipment ��ü ���� ����.

	AXZAttachment* XZAttachment = nullptr;
	if (IsValid(AttachmentClass))//AttachmentClass�� ���õǾ� �ִٸ�
	{
		FActorSpawnParameters Params;
		Params.Owner = InOwner;

		XZAttachment = InOwner->GetWorld()->SpawnActor<AXZAttachment>(AttachmentClass, Params);
	}

	UXZEquipment* XZEquipment = nullptr;
	if (IsValid(EquipmentClass))
	{
		XZEquipment = NewObject<UXZEquipment>(this, EquipmentClass);
		XZEquipment->Init(XZAttachment, InOwner, EquipmentData);
		
		//if (IsValid(attachment))//Attachment�� �ִٸ�
		//{
		//	equipment->OnEquipmentBeginEquip.AddDynamic(attachment, &ACAttachment::OnBeginEquip);
		//	equipment->OnEquipmentUnequip.AddDynamic(attachment, &ACAttachment::OnUnequip);
		//}
	}

	*OutWeaponData = NewObject<UXZWeaponData>();
	(*OutWeaponData)->Attachment = XZAttachment;
	(*OutWeaponData)->Equipment = XZEquipment;
}

#if WITH_EDITOR //Editor �������� ����
void UXZDA_Weapon::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
	Super::PostEditChangeChainProperty(PropertyChangedEvent);
	if (FApp::IsGame()) return; // ������ �������̸� return
	
}
#endif