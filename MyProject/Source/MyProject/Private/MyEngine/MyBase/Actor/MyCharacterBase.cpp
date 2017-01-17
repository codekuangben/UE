#include "MyProject.h"
#include "MyCharacterBase.h"
#include "Common.h"
#include "UIFormId.h"
#include "UITestCanvas.h"
#include "MyAnimInstance.h"

//////////////////////////////////////////////////////////////////////////
// AMyCharacter

AMyCharacterBase::AMyCharacterBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraBoom"));
	//CameraBoom->AttachTo(RootComponent);
	CameraBoom->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FollowCamera"));
	//FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->AttachToComponent(CameraBoom, FAttachmentTransformRules::KeepRelativeTransform, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input
void AMyCharacterBase::SetupPlayerInputComponent(class UInputComponent* InputComponentParam)
{
	// Set up gameplay key bindings
	check(InputComponentParam);
	InputComponentParam->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponentParam->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	InputComponent->BindAxis("MoveForward", this, &AMyCharacter::MoveForward);
	InputComponentParam->BindAxis("MoveRight", this, &AMyCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponentParam->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponentParam->BindAxis("TurnRate", this, &AMyCharacter::TurnAtRate);
	InputComponentParam->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponentParam->BindAxis("LookUpRate", this, &AMyCharacter::LookUpAtRate);

	// handle touch devices
	InputComponentParam->BindTouch(IE_Pressed, this, &AMyCharacter::TouchStarted);
	InputComponentParam->BindTouch(IE_Released, this, &AMyCharacter::TouchStopped);
}

void AMyCharacterBase::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		Jump();
	}
}

void AMyCharacterBase::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		StopJumping();
	}
}

void AMyCharacterBase::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyCharacterBase::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMyCharacterBase::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMyCharacterBase::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AMyCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	GEngineData->setMainActor(this);

	// 各种初始化
	//EngineApi::showCursor();

	// 各种 UI 测试
	//TestUI();

	// 赋值控制 Player ，逻辑开始运行
	GCtx->beginPlay();
}

void AMyCharacterBase::TestUI()
{
	// Test 加载 UIPack
	//GUiMgr->loadForm<UUIPack>(eUIPack);
	GUiMgr->loadForm<UUITestCanvas>(eUITestCanvas);
}

void AMyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (TObjectIterator<UMyAnimInstance> Anim; Anim; ++Anim)
	{
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Pitch, 0);
		Anim->NeckSpring = YawRotation;
	}
}

bool AMyCharacterBase::IsAlive()
{
	return true;
}

const UMyAnimInstanceBase* const AMyCharacterBase::GetAnimInstance()
{
	UMyAnimInstanceBase* AnimInstance = nullptr;

	if (nullptr != GetMesh())
	{
		AnimInstance = Cast<UMyAnimInstanceBase>(GetMesh()->GetAnimInstance());
	}

	return AnimInstance;
}