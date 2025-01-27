#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CustomPawn.generated.h"

UCLASS()
class TASK7_API ACustomPawn : public APawn
{
	GENERATED_BODY()

public:
	ACustomPawn();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCameraComponent* Camera;

	virtual void BeginPlay() override;
	
};
