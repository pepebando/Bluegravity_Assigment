// Copyright Epic Games, Inc. All Rights Reserved.

#include "BlueGravityCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
//////////////////////////////////////////////////////////////////////////
// ABlueGravityCharacter

ABlueGravityCharacter::ABlueGravityCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//Create skate
	Skate = CreateDefaultSubobject<USkeletalMeshComponent>("skate");
	Skate->SetupAttachment(RootComponent);

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ABlueGravityCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CalculateSkateRotation();
}
void ABlueGravityCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ABlueGravityCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);


		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABlueGravityCharacter::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ABlueGravityCharacter::MoveCompleted);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABlueGravityCharacter::Look);

	}

}
void ABlueGravityCharacter::MoveCompleted()
{
	UpKeyPressed = false;
}

void ABlueGravityCharacter::Move(const FInputActionValue& Value)
{

	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (MovementVector.Y > 0) {
		UpKeyPressed = true;
	}
	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	;

		// get forward vector of the skateboard
		const FVector ForwardDirection = Skate->GetForwardVector();
		// get right vector of the skateboard
		const FVector RightDirection = Skate->GetRightVector();
		//reduce de right movement 
		const float FinalValueX = MovementVector.X * 0.1;
		// add movement 
		if (MovementVector.Y > 0) {
			AddMovementInput(ForwardDirection, MovementVector.Y);
			GetCharacterMovement()->GroundFriction = 0.2;
		}
		else {
			/*Change the ground friction to brake*/
			GetCharacterMovement()->GroundFriction = 2;
		}
		AddMovementInput(RightDirection, FinalValueX);
	}
}

void ABlueGravityCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}

}
void ABlueGravityCharacter::GetSocketLocationForLegs(FVector& FL_out, FVector& BL_out) {
	/*Function that returns sockets locations for animbp*/
	FVector FLLocation = Skate->GetSocketLocation("FW");
	FVector BLLocation = Skate->GetSocketLocation("BW");
	FL_out = FLLocation;
	BL_out = BLLocation;
}

void ABlueGravityCharacter::CalculateSkateRotation() {
	/*Create 2 lines traces to the floor, to get the rotation of the surface and send it to the skateboard*/
	FVector ForwardSocketSkateLocation = Skate->GetSocketLocation("FW");
	FVector BackwardSocketSkateLocation = Skate->GetSocketLocation("BW");
	FVector StartLineTraceFW = ForwardSocketSkateLocation + FVector(0, 0, 30);
	FVector EndLineTraceFW = ForwardSocketSkateLocation + FVector(0, 0, -30);
	FVector StartLineTraceBW = BackwardSocketSkateLocation + FVector(0, 0, 30);
	FVector EndLineTraceBW = BackwardSocketSkateLocation + FVector(0, 0, -30);
	FHitResult HitResultFW;
	FHitResult HitResultBW;
	FCollisionQueryParams Params;
	FVector FWHIT;
	FVector BWHIT;

	Params.AddIgnoredActor(this);
	if (GetWorld()->LineTraceSingleByChannel(HitResultFW, StartLineTraceFW, EndLineTraceFW, ECollisionChannel::ECC_Camera, Params, FCollisionResponseParams())) {
		FWHIT = HitResultFW.Location;
	}
	else {
		FWHIT = ForwardSocketSkateLocation;
	}

	if (GetWorld()->LineTraceSingleByChannel(HitResultBW, StartLineTraceBW, EndLineTraceBW, ECollisionChannel::ECC_Camera, Params, FCollisionResponseParams())) {
		BWHIT = HitResultBW.Location;
	}
	else {
		BWHIT = BackwardSocketSkateLocation;
	}

	FRotator FirstSkateRotator = UKismetMathLibrary::FindLookAtRotation(BWHIT, FWHIT);
	FRotator FinalSkateRotator = UKismetMathLibrary::RInterpTo(Skate->GetComponentRotation(), FirstSkateRotator, GetWorld()->GetDeltaSeconds(), 20);
	Skate->SetWorldRotation(FinalSkateRotator);

	/*DEBUG LINES FOR THE SKATE ROTATION*/
	//DrawDebugLine(GetWorld(), StartLineTraceFW, EndLineTraceFW, FColor::Green, true, 1.0f);
	//DrawDebugLine(GetWorld(), StartLineTraceBW, EndLineTraceBW, FColor::Green, true, 1.0f);
}