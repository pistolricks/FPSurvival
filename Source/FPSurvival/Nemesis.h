// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Nemesis.generated.h"
/**
 * 
 */
UCLASS()
class FPSURVIVAL_API ANemesis : public ACharacter
{
	GENERATED_BODY()

public:
	ANemesis();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool CanAttackPlayer;
	int health;

	class AFPSurvivalCharacter* PlayerREF;
	class AAIController* NemesisAIC;

	UPROPERTY(editAnywhere)
	float StoppingDistance = 100.0f;

	UPROPERTY(EditAnywhere)
	class USphereComponent* PlayerAttackCollisionDetection;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* EnemyAttackAnimation;

	UPROPERTY(BlueprintReadWrite)
	bool CanDealDamage;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* DamageCollision;

	class UAnimInstance* AnimInstance;

	FTimerHandle SeekPlayerTH;

	virtual float TakeDamage
	(
		float Damage,
		const struct FDamageEvent& DamageEvent,
		AController* EventInstigator,
		AActor* DamageCauser
	) override;

	void OnAIMovedCompleted(struct FAIRequestID RequestID, const struct FPathFollowingResult& Result);

	UFUNCTION(BlueprintCallable)
	void AttackAnimationEnded();

	UFUNCTION()
	void OnPlayerAttackOverlapBegin(class UPrimitiveComponent* OverlappedComponent,
	                                class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
	                                int32 OtherBodyIndex,
	                                bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPlayerAttackOverlapEnd(class UPrimitiveComponent* OverlappedComponent,
	                              class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComponent,
	                              class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                              bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SeekPlayer();
};
