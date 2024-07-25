#include "XZProjectile.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"

AXZProjectile::AXZProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	CollisionBox->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

}

void AXZProjectile::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		CollisionBox->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
	}

}

void AXZProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ACharacter* HittedCharacter = Cast<ACharacter>(Hit.GetActor());
	if (IsValid(HittedCharacter))
	{
		// TODO: �� Ƣ��� ȿ�� + ����
	}
	else
	{
		// TODO: �Ѿ� �ٴ�/���� ��� ���� ȿ�� + ���� 		
	}

	Destroy();
}

void AXZProjectile::StartDestroyTimer()
{
	GetWorldTimerManager().SetTimer(DestroyTimer, this, &ThisClass::EndDestroyTimer, DestroyTime);
}

void AXZProjectile::EndDestroyTimer()
{
	Destroy();
}
