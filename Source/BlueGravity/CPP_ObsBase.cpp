// Fill out your copyright notice in the Description page of Project Settings.
#include "CPP_ObsBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "BlueGravityGameMode.h"
// Sets default values
ACPP_ObsBase::ACPP_ObsBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateAbstractDefaultSubobject<USceneComponent>(TEXT("pickuproot"));

	ObsMesh = CreateDefaultSubobject<UStaticMeshComponent>("Obstacle Mesh");
	ObsMesh->SetupAttachment(RootComponent);

	GoalColission = CreateDefaultSubobject<UBoxComponent>(FName("Goal Collision"));
	GoalColission->SetupAttachment(ObsMesh);

	ErrorColission = CreateDefaultSubobject<UBoxComponent>(FName("Crash Collision"));
	ErrorColission->SetupAttachment(ObsMesh);


}

// Called when the game starts or when spawned
void ACPP_ObsBase::BeginPlay()
{
	Super::BeginPlay();
	GoalColission->OnComponentBeginOverlap.AddUniqueDynamic(this, &ACPP_ObsBase::OnGoalBoxBeginOverlap);
	GoalColission->OnComponentBeginOverlap.AddUniqueDynamic(this, &ACPP_ObsBase::OnErrorBoxBeginOverlap);
}

// Called every frame
void ACPP_ObsBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_ObsBase::OnGoalBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	AddPoints();

}

void ACPP_ObsBase::OnErrorBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
}

void ACPP_ObsBase::AddPoints() {
	ABlueGravityGameMode* const GameModeRef = GetWorld()->GetAuthGameMode<ABlueGravityGameMode>();
	GameModeRef->AddGlobalPoints(PointsToEarn);
	
}


