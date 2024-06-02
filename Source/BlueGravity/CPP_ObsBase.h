// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"

#include "CPP_ObsBase.generated.h"


UCLASS()
class BLUEGRAVITY_API ACPP_ObsBase : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Editables, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* GoalColission = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Editables, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* ErrorColission = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Editables, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ObsMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Score, meta = (AllowPrivateAccess = "true"))
	float PointsToEarn;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Score, meta = (AllowPrivateAccess = "true"))
	float PointsToRemove;

public:	
	// Sets default values for this actor's properties
	ACPP_ObsBase();
	bool EnterGoalCol = false;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnGoalBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnErrorBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void AddPoints();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
