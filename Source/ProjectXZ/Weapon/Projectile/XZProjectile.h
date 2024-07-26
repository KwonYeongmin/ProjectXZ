#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XZProjectile.generated.h"

class USoundCue;
class UBoxComponent;
class UProjectileMovementComponent;

USTRUCT()
struct FProjectileData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	float InitialSpeed = 15000.0f; // ProjectileBullet(=�Ѿ�) �ӵ�

	UPROPERTY(EditDefaultsOnly)
	float GravityScale = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UParticleSystem> ImpactBlood; // �浹 �� �� Ƣ��

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UParticleSystem> ImpactParticles; // �浹 �� ��ƼŬ

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USoundCue> ImpactSound;  // �浹 �� ����
};

UCLASS()
class PROJECTXZ_API AXZProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AXZProjectile();

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void StartDestroyTimer();
	void EndDestroyTimer();


	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ProjectileMesh;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Projectile Information")
	float DestroyTime = 3.0f; // �浹���� �ʾ����� �Ҹ���� �ɸ��� �ð�
	FTimerHandle DestroyTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile Information")
	FProjectileData ProjectileData;
};
