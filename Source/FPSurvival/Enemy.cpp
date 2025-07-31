// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "FPSurvivalCharacter.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerAttackCollisionDetection =
		CreateDefaultSubobject<USphereComponent>(TEXT("Player AttachCollision Detection"));
	PlayerAttackCollisionDetection->SetupAttachment(RootComponent);

	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));
	DamageCollision->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	EnemyAIC = Cast<AAIController>(GetController());
	EnemyAIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &AEnemy::OnAIMovedCompleted);

	AnimInstance = GetMesh()->GetAnimInstance();

	PlayerAttackCollisionDetection->OnComponentBeginOverlap.AddDynamic(this,
	                                                                   &AEnemy::OnPlayerAttackOverlapBegin);

	PlayerAttackCollisionDetection->OnComponentEndOverlap.AddDynamic(this,
	                                                                 &AEnemy::OnPlayerAttackOverlapEnd);

	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnDealDamageOverlapBegin);

	SeekPlayer();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::OnAIMovedCompleted(struct FAIRequestID RequestID, const struct FPathFollowingResult& Result)
{
	if (CanAttackPlayer)
	{
		GetWorld()->GetTimerManager().ClearTimer(SeekPlayerTH);
		AnimInstance->Montage_Play(EnemyAttackAnimation);
	}
}

void AEnemy::AttackAnimationEnded()
{
	if (CanAttackPlayer)
	{
		AnimInstance->Montage_Play(EnemyAttackAnimation);
	}
}

void AEnemy::OnPlayerAttackOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor,
                                        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                        const FHitResult& SweepResult)
{
	PlayerREF = Cast<AFPSurvivalCharacter>(OtherActor);

	if (PlayerREF)
	{
		CanAttackPlayer = true;
	}
	UE_LOG(LogTemp, Warning, TEXT("Overlap Begin"));
}

void AEnemy::OnPlayerAttackOverlapEnd(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor,
                                      class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	PlayerREF = Cast<AFPSurvivalCharacter>(OtherActor);

	if (PlayerREF)
	{
		CanAttackPlayer = false;

		UE_LOG(LogTemp, Warning, TEXT("Overlap End"));

		SeekPlayer();
	}
}

void AEnemy::OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor,
                                      class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	PlayerREF = Cast<AFPSurvivalCharacter>(OtherActor);
	if (PlayerREF && CanDealDamage)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Damaged"));
		PlayerREF->ModifyHealth(-25);
	}
}

void AEnemy::SeekPlayer()
{
	GetWorld()->GetTimerManager().SetTimer(SeekPlayerTH, this, &AEnemy::SeekPlayer, 0.25f, true);

	if (PlayerREF)
	{
		EnemyAIC->MoveToLocation(PlayerREF->GetActorLocation(), StoppingDistance, true);
	}
	else
	{
		PlayerREF = Cast<AFPSurvivalCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		SeekPlayer();
	}
}
