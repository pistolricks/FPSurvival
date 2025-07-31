// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class FPSURVIVAL_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool CanAttackPlayer;
	
	class AFPSurvivalCharacter* PlayerREF;
	class AAIController* EnemyAIC;
	
	UPROPERTY(editAnywhere)
	float StoppingDistance = 100.0f;

	UPROPERTY(EditAnywhere)
	class USphereComponent* PlayerAttackCollisionDetection;
	
	UPROPERTY(EditAnywhere)
	class UAnimMontage* EnemyAttackAnimation;

	class UAnimInstance* AnimInstance;
	
	FTimerHandle SeekPlayerTH;

	void OnAIMovedCompleted(struct FAIRequestID RequestID, const struct FPathFollowingResult& Result);
	
	UFUNCTION()
	void OnPlayerAttackOverlapBegin(class UPrimitiveComponent* OverlappedComponent,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerAttackOverlapEnd(class UPrimitiveComponent* OverlappedComponent,
		class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
	UFUNCTION()
	void SeekPlayer();
};
