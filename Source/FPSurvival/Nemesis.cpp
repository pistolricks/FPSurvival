// Fill out your copyright notice in the Description page of Project Settings.


#include "Nemesis.h"
#include "FPSurvivalCharacter.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"

ANemesis::ANemesis()
{
	PrimaryActorTick.bCanEverTick = true;

	PlayerAttackCollisionDetection =
		CreateDefaultSubobject<USphereComponent>(TEXT("Player AttachCollision Detection"));
	PlayerAttackCollisionDetection->SetupAttachment(RootComponent);

	DamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Damage Collision"));
	DamageCollision->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
}

void ANemesis::BeginPlay()
{
	Super::BeginPlay();

	health = 100;

	NemesisAIC = Cast<AAIController>(GetController());
	NemesisAIC->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ANemesis::OnAIMovedCompleted);

	AnimInstance = GetMesh()->GetAnimInstance();

	PlayerAttackCollisionDetection->OnComponentBeginOverlap.AddDynamic(this,
	                                                                   &ANemesis::OnPlayerAttackOverlapBegin);

	PlayerAttackCollisionDetection->OnComponentEndOverlap.AddDynamic(this,
	                                                                 &ANemesis::OnPlayerAttackOverlapEnd);

	DamageCollision->OnComponentBeginOverlap.AddDynamic(this, &ANemesis::OnDealDamageOverlapBegin);

	SeekPlayer();
}

void ANemesis::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ANemesis::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ANemesis::TakeDamage(float Damage, const struct FDamageEvent& DamageEvent, AController* EventInstigator,
                           AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Enemy Damaged"));
	health -= Damage;
	if (health <= 0)
	{
		Destroy();
	}
	return Damage;
}

void ANemesis::OnAIMovedCompleted(struct FAIRequestID RequestID, const struct FPathFollowingResult& Result)
{
	if (CanAttackPlayer)
	{
		GetWorld()->GetTimerManager().ClearTimer(SeekPlayerTH);
		AnimInstance->Montage_Play(EnemyAttackAnimation);
	}
}

void ANemesis::AttackAnimationEnded()
{
	if (CanAttackPlayer)
	{
		AnimInstance->Montage_Play(EnemyAttackAnimation);
	}
}

void ANemesis::OnPlayerAttackOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor,
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

void ANemesis::OnPlayerAttackOverlapEnd(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor,
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

void ANemesis::OnDealDamageOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor,
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

void ANemesis::SeekPlayer()
{
	GetWorld()->GetTimerManager().SetTimer(SeekPlayerTH, this, &ANemesis::SeekPlayer, 0.25f, true);

	if (AnimInstance->Montage_IsPlaying(EnemyAttackAnimation))
	{
		UE_LOG(LogTemp, Warning, TEXT("Montage is still playing"));
	}
	else if (PlayerREF)
	{
		NemesisAIC->MoveToLocation(PlayerREF->GetActorLocation(), StoppingDistance, true);
	}
	else
	{
		PlayerREF = Cast<AFPSurvivalCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		SeekPlayer();
	}
}
