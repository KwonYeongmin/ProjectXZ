#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XZProjectile.generated.h"

class UBoxComponent;

UCLASS()
class PROJECTXZ_API AXZProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AXZProjectile();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void StartDestroyTimer();
	void EndDestroyTimer();


	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> CollisionBox;

	UPROPERTY(EditAnywhere)
	float InitialSpeed = 15000.0f; // ProjectileBullet(=�Ѿ�) �ӵ�

	UPROPERTY(EditAnywhere)
	float DestroyTime = 2.5f; // �浹���� �ʾ����� �Ҹ���� �ɸ��� �ð�
	FTimerHandle DestroyTimer;

};
