// Fill out your copyright notice in the Description page of Project Settings.
#include "CPP_ObsBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "BlueGravityGameMode.h"
#include "Kismet/KismetMaterialLibrary.h"
// Sets default values
ACPP_ObsBase::ACPP_ObsBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateAbstractDefaultSubobject<USceneComponent>(TEXT("pickuproot"));

	//Obstacle mesh creation
	ObsMesh = CreateDefaultSubobject<UStaticMeshComponent>("Obstacle Mesh");
	ObsMesh->SetupAttachment(RootComponent);

	//Obstacle goal collision
	GoalColission = CreateDefaultSubobject<UBoxComponent>(FName("Goal Collision"));
	GoalColission->SetupAttachment(ObsMesh);
	
	//Create collision for crash
	ErrorColission = CreateDefaultSubobject<UBoxComponent>(FName("Crash Collision"));
	ErrorColission->SetupAttachment(ObsMesh);


}

void ACPP_ObsBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	FName PropertyName = (PropertyChangedEvent.Property != nullptr)
		? PropertyChangedEvent.Property->GetFName() : NAME_None;

		if (ObsMesh != nullptr)
		{
			ObsMesh->SetStaticMesh(MeshToUse);
		}
	//Super::PostEditChangeProperty(PropertyChangedEvent);
}

// Called when the game starts or when spawned
void ACPP_ObsBase::BeginPlay()
{
	Super::BeginPlay();
	GoalColission->OnComponentBeginOverlap.AddUniqueDynamic(this, &ACPP_ObsBase::OnGoalBoxBeginOverlap);
	GoalColission->OnComponentBeginOverlap.AddUniqueDynamic(this, &ACPP_ObsBase::OnErrorBoxBeginOverlap);
	//GoalColission->OnComponentHit.AddUniqueDynamic(this, &ACPP_ObsBase::OnErrorBoxBeginOverlap);
}

// Called every frame
void ACPP_ObsBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//AddPoints on overlap obstacle Goal
void ACPP_ObsBase::OnGoalBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (!EnterGoalCol) {
		AddPoints();
		EnterGoalCol = true;
	}
}

void ACPP_ObsBase::OnErrorBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
}

void ACPP_ObsBase::AddPoints() {
	//Ref to game mode to add points as a global Var
	ABlueGravityGameMode* const GameModeRef = GetWorld()->GetAuthGameMode<ABlueGravityGameMode>();
	GameModeRef->AddGlobalPoints(PointsToEarn);
	GameModeRef->AddGlobalObstacleCompleted();

	//Change to green color
	UMaterialInterface* mat = ObsMesh->GetMaterial(0);
	UMaterialInstanceDynamic* dMat = UMaterialInstanceDynamic::Create(ObsMesh->GetMaterial(0), NULL);
	ObsMesh->SetMaterial(0, dMat);
	dMat->SetVectorParameterValue("Color", FLinearColor::Green);

}