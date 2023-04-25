// Fill out your copyright notice in the Description page of Project Settings.


#include "PaddlePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Paddle.h"
#include "Ball.h"

APaddle* Paddle_P;

APaddlePlayerController::APaddlePlayerController() 
{
}

void APaddlePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	EnableInput(this);
	InputComponent->BindAxis("MoveHorizontal", this, &APaddlePlayerController::MoveHorizontal);
	InputComponent->BindAction("Launch",IE_Pressed, this, &APaddlePlayerController::Launch );

}

void APaddlePlayerController::BeginPlay()
{
	TArray<AActor*> CameraActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), CameraActors);

	FViewTargetTransitionParams Params;
	SetViewTarget(CameraActors[0], Params);
	Paddle_P = Cast<APaddle>(GetPawn());

	SpawnNewBall();
}

void APaddlePlayerController::MoveHorizontal(float AxisValue)
{
	if (Paddle_P!=nullptr) {
		Paddle_P->MoveHorizontal(AxisValue);
	}
}

void APaddlePlayerController::Launch()
{
	if (MyBall != nullptr) {
		MyBall->Launch();
	}
}

void APaddlePlayerController::SpawnNewBall()
{
	if (!MyBall) {
		MyBall = nullptr;
	}
	if (BallObj) {
		MyBall = GetWorld()->SpawnActor<ABall>(BallObj, SpawnLocation, SpawnRotation, SpawnInfo);
	}

}
