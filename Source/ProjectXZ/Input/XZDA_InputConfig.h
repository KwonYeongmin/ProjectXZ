#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "XZDA_InputConfig.generated.h"

class UInputAction;

/** InputAciton�� GameplayTag�� ���ν����ֱ� ���� ����ϴ� Struct
 *
 */
USTRUCT(BlueprintType)
struct FXZInputAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag InputTag;
};


/**
 * 
 */
UCLASS()
class PROJECTXZ_API UXZDA_InputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UXZDA_InputConfig(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	const UInputAction* FindNativeInputActionForTag(const FGameplayTag& InputTag) const;
	UFUNCTION()
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag) const;

	UPROPERTY(EditDefaultsOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FXZInputAction> NativeInputActions;
	UPROPERTY(EditDefaultsOnly, Meta = (TitleProperty = "InputAction"))
	TArray<FXZInputAction> AbilityInputActions;
};
