#include "XZCombat.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/Character.h"
#include "ProjectXZ/Weapon/Attachment/XZAttachment.h"
#include "ProjectXZ/Weapon/Projectile/XZProjectile.h"

void UXZCombat::Init(AXZAttachment* InAttachment, ACharacter* InOwner, const TArray<FActionData>& InActionDatas)
{
	XZAttachment = InAttachment;
	OwnerCharacter = InOwner;
	ActionDatas = InActionDatas;
}

void UXZCombat::FireAction(const FVector& HitTarget)
{
	// ���������� ���ƿ��°ſ���

	// ĸó [ ] ���� ���޵Ǹ� ������? <- ĸó�� ����� �ּҰ��� �ƴϿ��� �Ѵ�. this ��� �� �����ؾ� �Ѵ�. 
	// ���� ����غ��� (ĸó�Ȱ� ��ȿ����! Ȯ���ϱ�!)

	HitTargetLocation = HitTarget;

	if (IsValid(ActionDatas[Idx].ActionMontage))
	{
		OwnerCharacter->PlayAnimMontage(ActionDatas[Idx].ActionMontage);

		FTimerHandle FireTimerHandle;
		FTimerDelegate FireTimerDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::OnFireBullet);
		//FTimerDelegate FireTimerDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::OnFireBullet, HitTarget);
		//FTimerDelegate FireTimerDelegate;
		//FireTimerDelegate.BindUFunction(this, FName("OnFireBullet"), HitTarget);
		//FTimerDelegate FireTimerDelegate = FTimerDelegate::CreateLambda([HitTarget]() { OnFireBullet(HitTarget); });
		OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, FireTimerDelegate, ActionDatas[Idx].Action_FrameTime, false);
	}
}

void UXZCombat::OnFireBullet()
{
	// ���� Nozzle���� �Ѿ� �߻�

    if (IsValid(XZAttachment) && IsValid(XZAttachment->GetWeaponMesh()))
    {
        const USkeletalMeshSocket* MuzzleFlashSocket = XZAttachment->GetWeaponMesh()->GetSocketByName(ActionDatas[Idx].MuzzleSocketName);
        if (MuzzleFlashSocket)
        {
            FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(XZAttachment->GetWeaponMesh());
            FRotator TargetRotation = (HitTargetLocation - SocketTransform.GetLocation()).Rotation();

            if (IsValid(OwnerCharacter) && IsValid(OwnerCharacter->GetWorld()) && IsValid(ActionDatas[Idx].ProjectileClass))
            {
                UE_LOG(LogTemp, Warning, TEXT("Spawning projectile at location: %s with rotation: %s"), *SocketTransform.GetLocation().ToString(), *TargetRotation.ToString());

                FActorSpawnParameters SpawnParams;
                SpawnParams.Owner = XZAttachment->GetOwner();
                SpawnParams.Instigator = Cast<APawn>(XZAttachment->GetOwner());
                SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

                SpawnedProjectile = OwnerCharacter->GetWorld()->SpawnActor<AXZProjectile>(ActionDatas[Idx].ProjectileClass, SocketTransform.GetLocation(), TargetRotation, SpawnParams);
                if (IsValid(SpawnedProjectile))
                {
                    UE_LOG(LogTemp, Warning, TEXT("Projectile spawned successfully"));
                }
            }
        }
    }
}
