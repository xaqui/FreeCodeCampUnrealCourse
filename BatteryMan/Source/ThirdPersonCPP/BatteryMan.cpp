// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryMan.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABatteryMan::ABatteryMan()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent() -> InitCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Allow the character movement to rotate in the direction it's moving
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	// Attach the camera at the end of the boom and the let the boom adjust to match controller rotation input
	FollowCamera -> SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Camera does not rotate relative to the arm
	FollowCamera ->bUsePawnControlRotation = false;

	bDead = false;
	Power = 100.0f;
}




// Called when the game starts or when spawned
void ABatteryMan::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABatteryMan::OnBeginOverlap);
	if (PlayerPowerWidgetClass != nullptr) {
		PlayerPowerWidget = CreateWidget(GetWorld(), PlayerPowerWidgetClass);
		PlayerPowerWidget->AddToViewport();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("NULL IN UI"));
	}
}

// Called every frame
void ABatteryMan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Power -= DeltaTime * PowerThreshold;
	if (Power <= 0) {
		if (!bDead) {
			bDead = true;

			//ragdoll?
			GetMesh()->SetSimulatePhysics(true);

			FTimerHandle UnusedHandle;
			// Call restart after 3 seconds, dont repeat after 3 seconds
			GetWorldTimerManager().SetTimer(
			UnusedHandle, this, &ABatteryMan::RestartGame, 3.0f, false);
		}
	}

}

// Called to bind functionality to input
void ABatteryMan::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABatteryMan::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ABatteryMan::StopJumping);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ABatteryMan::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ABatteryMan::MoveRight);
}

void ABatteryMan::Jump() {
	if (bDead) { return; }
	ACharacter::Jump();
}

void ABatteryMan::StopJumping() {
	if (bDead) { return; }
	ACharacter::StopJumping();
}

void ABatteryMan::MoveForward(float Axis)
{
	if (bDead) { return; }
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// Calculate the Forward direction of YawRotation
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Axis);
}

void ABatteryMan::MoveRight(float Axis)
{
	if (bDead) { return; }
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// Calculate the Forward direction of YawRotation
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Axis);
}

void ABatteryMan::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag("Recharge")) {
		//UE_LOG(LogTemp, Warning, TEXT("Collided with"));
		Power += 10.0f;
		if (Power > 100.0f) {
			Power = 100.0f;
		}
		OtherActor->Destroy();
	}
}

void ABatteryMan::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}