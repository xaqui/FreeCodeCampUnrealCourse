// Fill out your copyright notice in the Description page of Project Settings.


#include "Ball.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABall::ABall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Ball_SM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ball"));
	//RootComponent = Ball_SM;

	if (RootComponent) {
		Ball_SM->SetupAttachment(RootComponent);
	}

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	Ball_SM->SetSimulatePhysics(true);
	Ball_SM->SetEnableGravity(false);
	
	//These lines crash UE5 5.0.3 Editor on Startup (is this a bug?) Set these from the blueprint editor instead :)
	//Ball_SM->SetConstraintMode(EDOFMode::XZPlane);
	//Ball_SM->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//Ball_SM->SetCollisionProfileName(TEXT("PhysicsActor"));

	//ProjectileMovement->bShouldBounce = true;
	//ProjectileMovement->Bounciness = 1.1f;
	//ProjectileMovement->Friction = 0.0f;
	//ProjectileMovement->Velocity.X = 0.0f;
}


// Called when the game starts or when spawned
void ABall::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABall::Launch()
{


	if (!bBallLaunched) {
		Ball_SM->AddImpulse(FVector(140.0f, 0.0f, 130.0f), FName(), true);
		bBallLaunched = true;
	}

}

UStaticMeshComponent* ABall::GetBall()
{
	return Ball_SM;
}

